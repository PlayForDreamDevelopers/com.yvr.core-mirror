#include "YVRSpatialAnchorMgr.h"
#include "../Utilities/YVRLog.h"
#include "../YVRUnityPlugin.h"
#include "../Utilities/YVRCoordinateConverter.h"

YVRSpatialAnchorMgr::YVRSpatialAnchorMgr() {}

void YVRSpatialAnchorMgr::createSpatialAnchorFB(XrVector3f position, XrQuaternionf rotation, uint64_t* createRequest)
{
    XrSpatialAnchorCreateInfoFB anchorCreateInfo = {XR_TYPE_SPATIAL_ANCHOR_CREATE_INFO_FB};
    anchorCreateInfo.space = plugin.inputMgr->appSpace;
    anchorCreateInfo.poseInSpace.position = position;
    anchorCreateInfo.poseInSpace.orientation = rotation;
    anchorCreateInfo.time = plugin.inputMgr->getTime(false);
    OpenXRAPI(xrCreateSpatialAnchorFB(plugin.openxrMgr->program->session, &anchorCreateInfo, createRequest));
}

bool YVRSpatialAnchorMgr::getSpatialAnchorPose(uint64_t space, XrVector3f* position, XrQuaternionf* rotation,XrSpaceLocationFlags* spaceLocationFlags)
{
    XrSpaceLocation anchorSpaceLocation = {XR_TYPE_SPACE_LOCATION};
    const XrResult result = xrLocateSpace(reinterpret_cast<XrSpace>(space), plugin.inputMgr->appSpace, plugin.inputMgr->getTime(false),
                                          &anchorSpaceLocation);
    *spaceLocationFlags = anchorSpaceLocation.locationFlags;
    *position = anchorSpaceLocation.pose.position;
    *rotation = anchorSpaceLocation.pose.orientation;
    return result == XR_SUCCESS;
}

void YVRSpatialAnchorMgr::saveSpatialAnchor(YVRSpaceAnchorSaveInfo spatialAnchorSaveInfo, uint64_t* requestId)
{
    XrSpaceSaveInfoFB saveInfo =
    {
        XR_TYPE_SPACE_SAVE_INFO_FB,
        nullptr,
        spatialAnchorSaveInfo.space,
        spatialAnchorSaveInfo.storageLocation,
        XR_SPACE_PERSISTENCE_MODE_INDEFINITE_FB
    };

    OpenXRAPI(xrSaveSpaceFB(plugin.openxrMgr->program->session, &saveInfo, requestId));
}

void YVRSpatialAnchorMgr::getSpatialAnchor(YVRSpaceQueryInfo info, uint64_t* requestId)
{
    XrSpaceQueryInfoFB queryInfo
    {
        XR_TYPE_SPACE_QUERY_INFO_FB,
        nullptr,
        XR_SPACE_QUERY_ACTION_LOAD_FB,
        info.maxResultCount,
        info.timeOut,
        nullptr,
        nullptr
    };

    std::vector<XrUuidEXT> querySpaceIds{};
    for (size_t i = 0; i < info.numIds; i++)
    {
        string str = info.uuids[i].uuid;
        string sub = str.substr(0, UUIDSize);
        uint8_t* uuidData = HexStringToUint8(sub);
        XrUuidEXT uuidEXT{};
        memcpy(uuidEXT.data, uuidData, XR_UUID_SIZE_EXT);
        querySpaceIds.push_back(uuidEXT);
    }

    XrSpaceStorageLocationFilterInfoFB locationFilterInfo = {XR_TYPE_SPACE_STORAGE_LOCATION_FILTER_INFO_FB, nullptr, info.storageLocation};
    XrSpaceUuidFilterInfoFB uuidFilterInfo{};
    uuidFilterInfo.type = XR_TYPE_SPACE_UUID_FILTER_INFO_FB;
    uuidFilterInfo.next = &locationFilterInfo;
    uuidFilterInfo.uuidCount = querySpaceIds.size();
    uuidFilterInfo.uuids = querySpaceIds.data();

    XrSpaceComponentFilterInfoFB componentFilterInfo = {XR_TYPE_SPACE_COMPONENT_FILTER_INFO_FB, &locationFilterInfo, info.component};
    componentFilterInfo.next = &uuidFilterInfo;
    queryInfo.filter = (XrSpaceFilterInfoBaseHeaderFB*)&componentFilterInfo;
    OpenXRAPI(xrQuerySpacesFB(plugin.openxrMgr->program->session, (XrSpaceQueryInfoBaseHeaderFB*)&queryInfo, requestId));
}

void YVRSpatialAnchorMgr::retrieveSpaceQueryResults(uint64_t requestId)
{
    AnnounceCallingFunc();
    XrSpaceQueryResultsFB queryResults{XR_TYPE_SPACE_QUERY_RESULTS_FB};
    queryResults.resultCapacityInput = 0;
    queryResults.resultCountOutput = 0;
    queryResults.results = nullptr;
    //第一次请求获取结果数量
    OpenXRAPI(xrRetrieveSpaceQueryResultsFB(plugin.openxrMgr->program->session, requestId, &queryResults));
    //根据结果数量申请空间
    std::vector<XrSpaceQueryResultFB> results(queryResults.resultCountOutput);
    queryResults.resultCapacityInput = results.size();
    queryResults.results = results.data();
    //第二次请求获取结果
    OpenXRAPI(xrRetrieveSpaceQueryResultsFB(plugin.openxrMgr->program->session, requestId, &queryResults));
    if(spaceMap.count(requestId) == 0)
    {
        std::vector<YVRSpaceAnchorResult> yvrAnchorResults;
        spaceMap[requestId] = yvrAnchorResults;
    }

    for (uint32_t i = 0; i < queryResults.resultCountOutput; ++i)
    {
        YVRSpaceAnchorResult anchorResult;
        anchorResult.space = queryResults.results[i].space;
        string uuidStr = Uint8ToHexString(queryResults.results[i].uuid.data, XR_UUID_SIZE_EXT);
        memcpy(anchorResult.uuid, uuidStr.c_str(), uuidStr.length());
        anchorResult.requestId = requestId;
        spaceMap[requestId].push_back(anchorResult);
    }
}

void YVRSpatialAnchorMgr::destroyAnchor(uint64_t space, XrSpaceStorageLocationFB storageLocation, uint64_t* requestId)
{
    AnnounceCallingFunc();
    XrSpaceEraseInfoFB eraseInfo = {XR_TYPE_SPACE_ERASE_INFO_FB, nullptr, (XrSpace)space, storageLocation};
    OpenXRAPI(xrEraseSpaceFB(plugin.openxrMgr->program->session, &eraseInfo, requestId));
    AnnounceCalledFunc();
}

void YVRSpatialAnchorMgr::enumerateSpaceSupported(uint64_t space,YVRSpaceSupportedComponent* supportedComponent)
{
    AnnounceCallingFunc();
    uint32_t numSupported = 0;
    OpenXRAPI(xrEnumerateSpaceSupportedComponentsFB((XrSpace)space, 0, &numSupported, nullptr));
    std::vector<XrSpaceComponentTypeFB> components(numSupported);
    OpenXRAPI(xrEnumerateSpaceSupportedComponentsFB((XrSpace)space, numSupported, &numSupported, components.data()));
    supportedComponent->numComponents = numSupported;
    memcpy(supportedComponent->components, components.data(), sizeof(XrSpaceComponentTypeFB) * numSupported);
}

bool YVRSpatialAnchorMgr::setSpaceComponentStatus(uint64_t space,YVRSpaceComponentStatusSetInfo componentStatusSetInfo, uint64_t* requestId)
{
    AnnounceCallingFunc();
    XrSpaceComponentStatusSetInfoFB request{};
    request.type = XR_TYPE_SPACE_COMPONENT_STATUS_SET_INFO_FB;
    request.next = nullptr;
    request.componentType = componentStatusSetInfo.component;
    request.enabled = componentStatusSetInfo.enable;
    request.timeout = componentStatusSetInfo.timeout;
    XrResult result = OpenXRAPI(xrSetSpaceComponentStatusFB((XrSpace)space, &request, requestId));
    switch (result)
    {
    case XR_ERROR_SPACE_COMPONENT_STATUS_PENDING_FB:
        YError("Set component status failed, component status is pending, wait for the other settings to complete");
        break;
    case XR_ERROR_SPACE_COMPONENT_NOT_SUPPORTED_FB:
        YError("This anchor not supported %d component, please call GetSpatialAnchorEnumerateSupported query support component",componentStatusSetInfo.component);
        break;
    default:
        break;
    }

    AnnounceCalledFunc();
    return result == XR_SUCCESS;
}

void YVRSpatialAnchorMgr::getSpaceComponentStatus(uint64_t space,XrSpaceComponentTypeFB component, YVRSpaceComponentStatus* componentStatus)
{
    XrSpaceComponentStatusFB status{};
    status.type = XR_TYPE_SPACE_COMPONENT_STATUS_FB;
    status.next = nullptr;
    OpenXRAPI(xrGetSpaceComponentStatusFB((XrSpace)space, component, &status));
    componentStatus->enable = status.enabled;
    componentStatus->changePending = status.changePending;
}

void YVRSpatialAnchorMgr::createSpaceUser(uint64_t userId, XrSpaceUserFB* spaceUser)
{
    AnnounceCallingFunc();
    XrSpaceUserCreateInfoFB createInfo;
    createInfo.type = XR_TYPE_SPACE_USER_CREATE_INFO_FB;
    createInfo.next = nullptr;
    createInfo.userId = userId;
    OpenXRAPI(xrCreateSpaceUserFB(plugin.openxrMgr->program->session, &createInfo, spaceUser));
    AnnounceCalledFunc();
}

void YVRSpatialAnchorMgr::shareSpace(YVRSpaceShareInfo yvrShareInfo, uint64_t* requestId)
{
    AnnounceCallingFunc();
    XrSpaceShareInfoFB shareInfo{};
    shareInfo.type = XR_TYPE_SPACE_SHARE_INFO_FB;
    shareInfo.next = nullptr;
    shareInfo.spaceCount = yvrShareInfo.spaceCount;
    shareInfo.spaces = yvrShareInfo.spaces;
    shareInfo.userCount = yvrShareInfo.userCount;
    shareInfo.users = yvrShareInfo.users;
    OpenXRAPI(xrShareSpacesFB(plugin.openxrMgr->program->session, &shareInfo, requestId));
    AnnounceCalledFunc();
}

void YVRSpatialAnchorMgr::saveSpatialAnchorList(YVRSpaceListSaveInfo saveInfo, uint64_t* requestId)
{
    std::vector<XrSpace> spaceList;
    for (size_t i = 0; i < saveInfo.spaceCount; i++)
    {
        spaceList.push_back(saveInfo.spaces[i]);
    }

    XrSpaceListSaveInfoFB listSaveInfo{XR_TYPE_SPACE_LIST_SAVE_INFO_FB, nullptr, static_cast<uint32_t>(spaceList.size()), spaceList.data(),
                                       saveInfo.location};
    OpenXRAPI(xrSaveSpaceListFB(plugin.openxrMgr->program->session, &listSaveInfo, requestId));

}

void YVRSpatialAnchorMgr::getSpaceUserId(XrSpaceUserFB spaceUser, uint64_t* userId)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrGetSpaceUserIdFB(spaceUser, userId));
    AnnounceCalledFunc();
}

void YVRSpatialAnchorMgr::getSpaceUuid(uint64_t spaceHandle, YVRSpatialAnchorUUID* yvrUUID)
{
    XrUuidEXT uuid;
    XrResult result = OpenXRAPI(xrGetSpaceUuidFB((XrSpace)spaceHandle, &uuid));
    if (result == XR_SUCCESS)
    {
        string uuidStr = Uint8ToHexString(uuid.data, XR_UUID_SIZE_EXT);
        memcpy(yvrUUID->uuid, uuidStr.c_str(), uuidStr.length());
    }
    else
    {
        YError("get spatial anchor uuid failed result:%d",result);
    }
}