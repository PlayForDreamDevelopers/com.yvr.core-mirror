#include "YVRSceneMgr.h"
#include "../Utilities/YVRLog.h"
#include "../YVRUnityPlugin.h"
#include "../Utilities/YVRCoordinateConverter.h"

YVRSceneMgr::YVRSceneMgr() {}
YVRSceneMgr::~YVRSceneMgr() {}
const std::string recognizedLabels = "TABLE,COUCH,FLOOR,CEILING,WALL_FACE,WINDOW_FRAME,DOOR_FRAME,STORAGE,BED,SCREEN,LAMP,PLANT,MESH_BOUNDARY,SAFETY_BOUNDARY,OTHER";
XrResult YVRSceneMgr::getSpaceBoundingBox2D(XrSpace space, XrRect2Df* boundingBox2DOutput)
{
    AnnounceCallingFunc();
    XrResult result = OpenXRAPI(xrGetSpaceBoundingBox2DFB(plugin.openxrMgr->program->session, space, boundingBox2DOutput));
    boundingBox2DOutput->offset.x = 0;
    boundingBox2DOutput->offset.y = 0;
    return result;
}

XrResult YVRSceneMgr::getSpaceBoundingBox3D(XrSpace space, XrRect3DfFB* boundingBox3DOutput)
{
    AnnounceCallingFunc();
    XrResult result = OpenXRAPI(xrGetSpaceBoundingBox3DFB(plugin.openxrMgr->program->session, space, boundingBox3DOutput));
    boundingBox3DOutput->offset.x = 0;
    boundingBox3DOutput->offset.y = 0;
    boundingBox3DOutput->offset.z = 0;
    return result;
}

XrResult YVRSceneMgr::getSpaceSemanticLabels(XrSpace space, YVRAnchorSemanticLabel* semanticLabelsOutput)
{
    AnnounceCallingFunc();
    const XrSemanticLabelsSupportInfoFB semanticLabelsSupportInfo = {
        XR_TYPE_SEMANTIC_LABELS_SUPPORT_INFO_FB, nullptr,
        XR_SEMANTIC_LABELS_SUPPORT_MULTIPLE_SEMANTIC_LABELS_BIT_FB | XR_SEMANTIC_LABELS_SUPPORT_ACCEPT_DESK_TO_TABLE_MIGRATION_BIT_FB,
        recognizedLabels.c_str()};

    XrSemanticLabelsFB labels = {XR_TYPE_SEMANTIC_LABELS_FB, &semanticLabelsSupportInfo, 0};
    XrResult result = OpenXRAPI(xrGetSpaceSemanticLabelsFB(plugin.openxrMgr->program->session, space, &labels));
    if (semanticLabelsOutput->byteCapacityInput == 0)
    {
        semanticLabelsOutput->byteCountOutput = labels.bufferCountOutput;
        return result;
    }
    std::vector<char> labelData(labels.bufferCountOutput);
    labels.bufferCapacityInput = labelData.size();
    labels.buffer = labelData.data();
    result = OpenXRAPI(xrGetSpaceSemanticLabelsFB(plugin.openxrMgr->program->session, space, &labels));
    memcpy(semanticLabelsOutput->labels, labels.buffer, sizeof(char) * labels.bufferCountOutput);
    return result;
}

XrResult YVRSceneMgr::getSpaceBoundary2D(XrSpace space, YVRBoundary2D* boundary2DOutput)
{
    AnnounceCallingFunc();

    XrBoundary2DFB boundary2D = {XR_TYPE_BOUNDARY_2D_FB, nullptr, 0};
    XrResult result = OpenXRAPI(xrGetSpaceBoundary2DFB(plugin.openxrMgr->program->session, space, &boundary2D));
    YInfo("getSpaceBoundary2D: 1st call result=%d, vertexCountOutput=%u", result, boundary2D.vertexCountOutput);
    if (result != XR_SUCCESS)
    {
        YError("getSpaceBoundary2D: 1st call failed, result=%d", result);
        return result;
    }
    std::vector<XrVector2f> vertices(boundary2D.vertexCountOutput);
    boundary2D.vertexCapacityInput = vertices.size();
    boundary2D.vertices = vertices.data();
    if (boundary2DOutput->vertexCapacityInput == 0)
    {
        boundary2DOutput->vertexCountOutput = boundary2D.vertexCountOutput;
        return result;
    }

    result = OpenXRAPI(xrGetSpaceBoundary2DFB(plugin.openxrMgr->program->session, space, &boundary2D));

    if (result != XR_SUCCESS)
    {
        YError("getSpaceBoundary2D: 2nd call failed, result=%d", result);
        return result;
    }

    if (boundary2D.vertices == nullptr || boundary2D.vertexCountOutput == 0)
    {
        YError("getSpaceBoundary2D: vertices is null or count is 0 after 2nd call, skip memcpy");
        return XR_ERROR_VALIDATION_FAILURE;
    }

    memcpy(boundary2DOutput->vertices, boundary2D.vertices, sizeof(XrVector2f) * boundary2D.vertexCountOutput);
    return result;
}

XrResult YVRSceneMgr::getSpaceRoomLayout(XrSpace space, YVRRoomLayout* roomLayoutOutput)
{
    AnnounceCallingFunc();
    XrRoomLayoutFB roomLayout = {XR_TYPE_ROOM_LAYOUT_FB};
    XrResult result = OpenXRAPI(xrGetSpaceRoomLayoutFB(plugin.openxrMgr->program->session, space, &roomLayout));

    std::vector<XrUuidEXT> wallUuids;

    if (roomLayout.wallUuidCountOutput != 0)
    {
        if (roomLayoutOutput->wallUuidCapacityInput == 0)
        {
            roomLayoutOutput->wallUuidCountOutput = roomLayout.wallUuidCountOutput;
            return XR_SUCCESS;
        }

        wallUuids.resize(roomLayout.wallUuidCountOutput);
        roomLayout.wallUuidCapacityInput = wallUuids.size();
        roomLayout.wallUuids = wallUuids.data();
        // Second call
        result = OpenXRAPI(xrGetSpaceRoomLayoutFB(plugin.openxrMgr->program->session, space, &roomLayout));
    }
    if (isValid(roomLayout.floorUuid))
    {
        string floorUuid = Uint8ToHexString(roomLayout.floorUuid.data, XR_UUID_SIZE_EXT);
        memcpy(roomLayoutOutput->floorUuid.uuid, floorUuid.c_str(), floorUuid.length());
    }

    if (isValid(roomLayout.ceilingUuid))
    {
        string ceilingUuid = Uint8ToHexString(roomLayout.ceilingUuid.data, XR_UUID_SIZE_EXT);
        memcpy(roomLayoutOutput->ceilingUuid.uuid, ceilingUuid.c_str(), ceilingUuid.length());
    }
    for (uint32_t i = 0; i < roomLayoutOutput->wallUuidCountOutput; i++)
    {
        string wallUuid = Uint8ToHexString(roomLayout.wallUuids[i].data, XR_UUID_SIZE_EXT);
        memcpy(roomLayoutOutput->wallUuids[i].uuid, wallUuid.c_str(), wallUuid.length());
    }

    return result;
}

XrResult YVRSceneMgr::getSpaceContainer(XrSpace space, YVRSceneAnchorContainer* spaceContainersOutput)
{
    AnnounceCallingFunc();
    std::vector<std::string> uuidSet;
    XrSpaceContainerFB spaceContainer = {XR_TYPE_SPACE_CONTAINER_FB};
    XrResult result = OpenXRAPI(xrGetSpaceContainerFB(plugin.openxrMgr->program->session, space, &spaceContainer));
    if (spaceContainer.uuidCountOutput == 0)
    {
        return result;
    }
    if (spaceContainersOutput->anchorCapacityInput == 0)
    {
        spaceContainersOutput->anchorCountOutput = spaceContainer.uuidCountOutput;
        return result;
    }

    std::vector<XrUuidEXT> uuids(spaceContainer.uuidCountOutput);
    spaceContainer.uuidCapacityInput = uuids.size();
    spaceContainer.uuids = uuids.data();
    result = OpenXRAPI(xrGetSpaceContainerFB(plugin.openxrMgr->program->session, space, &spaceContainer));
    for (uint32_t i = 0; i < spaceContainer.uuidCountOutput; i++)
    {
        string containerUuid = Uint8ToHexString(spaceContainer.uuids[i].data, XR_UUID_SIZE_EXT);
        memcpy(spaceContainersOutput->anchors[i].uuid, containerUuid.c_str(), containerUuid.length());
    }

    return result;
}

XrResult YVRSceneMgr::requestSceneCapture(YVRSceneCaptureRequest* request, uint64_t* requestId)
{
    AnnounceCallingFunc();
    XrSceneCaptureRequestInfoFB sceneCaptureRequest = {XR_TYPE_SCENE_CAPTURE_REQUEST_INFO_FB};
    sceneCaptureRequest.requestByteCount = 0;
    sceneCaptureRequest.request = request->request;
    return OpenXRAPI(xrRequestSceneCaptureFB(plugin.openxrMgr->program->session, &sceneCaptureRequest, requestId));
}

void YVRSceneMgr::createGlobalAnchor(XrVector3f position, XrQuaternionf rotation, uint64_t* createRequest)
{
    AnnounceCallingFunc();
    XrSpatialAnchorCreateInfoFB anchorCreateInfo = {XR_TYPE_SPATIAL_ANCHOR_CREATE_INFO_FB};
    anchorCreateInfo.space = plugin.inputMgr->appSpace;
    anchorCreateInfo.poseInSpace.position = position;
    anchorCreateInfo.poseInSpace.orientation = rotation;
    anchorCreateInfo.time = plugin.inputMgr->getTime(false);
    XrSpatialAnchorCreateInfoYVR extentAnchorCreateInfo = {static_cast<XrStructureType>(XR_TYPE_SPATIAL_ANCHOR_CREATE_INFO_YVR)};
    extentAnchorCreateInfo.global = true;
    anchorCreateInfo.next = &extentAnchorCreateInfo;
    OpenXRAPI(xrCreateSpatialAnchorFB(plugin.openxrMgr->program->session, &anchorCreateInfo, createRequest));
}

XrResult YVRSceneMgr::setSpaceBoundingBox2D(XrSpace space, XrRect2Df* boundingBox2DInput)
{
    AnnounceCallingFunc();
    return OpenXRAPI(xrSetSpaceBoundingBox2DYVR(plugin.openxrMgr->program->session, space, boundingBox2DInput));
}

XrResult YVRSceneMgr::setSpaceBoundingBox3D(XrSpace space, XrRect3DfFB* boundingBox3DInput)
{
    AnnounceCallingFunc();
    return OpenXRAPI(xrSetSpaceBoundingBox3DYVR(plugin.openxrMgr->program->session, space, boundingBox3DInput));
}

XrResult YVRSceneMgr::setSpaceSemanticLabels(XrSpace space, YVRAnchorSemanticLabel* semanticLabelsInput)
{
    AnnounceCallingFunc();
    XrSemanticLabelsFB labels = {XR_TYPE_SEMANTIC_LABELS_FB, nullptr, 0};
    labels.bufferCapacityInput = semanticLabelsInput->byteCapacityInput;
    string label = semanticLabelsInput->labels;
    labels.buffer = semanticLabelsInput->labels;
    string label2 = labels.buffer;

    return OpenXRAPI(xrSetSpaceSemanticLabelsYVR(plugin.openxrMgr->program->session, space, &labels));
}

XrResult YVRSceneMgr::setSpaceBoundary2D(XrSpace space, YVRBoundary2D* boundary2DInput)
{
    AnnounceCallingFunc();
    XrBoundary2DFB boundary2D = {XR_TYPE_BOUNDARY_2D_FB, nullptr, 0};
    boundary2D.vertexCapacityInput = boundary2DInput->vertexCapacityInput;
    boundary2D.vertices = boundary2DInput->vertices;

    XrResult result = OpenXRAPI(xrSetSpaceBoundary2DYVR(plugin.openxrMgr->program->session, space, &boundary2D));
    return result;
}

XrResult YVRSceneMgr::setSpaceRoomLayout(XrSpace space, YVRRoomLayout* roomLayoutInput)
{
    AnnounceCallingFunc();
    XrRoomLayoutFB roomLayout = {XR_TYPE_ROOM_LAYOUT_FB};

    string floorUuid = roomLayoutInput->floorUuid.uuid;
    if (floorUuid.length() > 0)
    {
        memcpy(roomLayout.floorUuid.data, HexStringToUint8(floorUuid.substr(0,UUIDSize)), XR_UUID_SIZE_EXT);
    }
    string ceilingUuid = roomLayoutInput->ceilingUuid.uuid;
    if (ceilingUuid.length() > 0)
    {
        memcpy(roomLayout.ceilingUuid.data, HexStringToUint8(ceilingUuid.substr(0,UUIDSize)), XR_UUID_SIZE_EXT);
    }

    std::vector<XrUuidEXT> uuids(roomLayoutInput->wallUuidCountOutput);
    if (roomLayoutInput->wallUuidCountOutput != 0)
    {
        for (uint32_t i = 0; i < roomLayoutInput->wallUuidCountOutput; i++)
        {
            string wallUuid = roomLayoutInput->wallUuids[i].uuid;
            if (wallUuid.length() > 0)
            {
                memcpy(uuids[i].data, HexStringToUint8(wallUuid.substr(0,UUIDSize)), XR_UUID_SIZE_EXT);
            }
        }

        roomLayout.wallUuidCapacityInput = uuids.size();
        roomLayout.wallUuidCountOutput = uuids.size();
        roomLayout.wallUuids = uuids.data();
    }

    return OpenXRAPI(xrSetSpaceRoomLayoutYVR(plugin.openxrMgr->program->session, space, &roomLayout));

}

XrResult YVRSceneMgr::setSpaceContainer(XrSpace space, YVRSceneAnchorContainer* spaceContainerInput)
{
    AnnounceCallingFunc();
    std::vector<XrUuidEXT> uuids(spaceContainerInput->anchorCountOutput);
    for (uint32_t i = 0; i < spaceContainerInput->anchorCountOutput; i++)
    {
        string uuidStr = spaceContainerInput->anchors[i].uuid;
        if (uuidStr.length() > 0)
        {
            memcpy(uuids[i].data, HexStringToUint8(uuidStr.substr(0,UUIDSize)), XR_UUID_SIZE_EXT);
        }
    }

    XrSpaceContainerFB spaceContainer = {XR_TYPE_SPACE_CONTAINER_FB};
    spaceContainer.uuidCapacityInput = uuids.size();
    spaceContainer.uuids = uuids.data();
    return OpenXRAPI(xrSetSpaceContainerYVR(plugin.openxrMgr->program->session, space, &spaceContainer));
}

bool YVRSceneMgr::isValid(const XrUuidEXT& uuid)
{
    for (int i = 0; i < XR_UUID_SIZE_EXT; ++i)
    {
        if (uuid.data[i] > 0)
        {
            return true;
        }
    }
    return false;
}

bool YVRSceneMgr::isComponentEnabled(XrSpace space, XrSpaceComponentTypeFB type)
{
    XrSpaceComponentStatusFB status = {XR_TYPE_SPACE_COMPONENT_STATUS_FB, nullptr};
    OpenXRAPI(xrGetSpaceComponentStatusFB(space, type, &status));
    return (status.enabled && !status.changePending);
}

XrResult YVRSceneMgr::getSceneStatus(uint32_t* status)
{
    AnnounceCallingFunc();
    XrSceneStatusYVR sceneStatus = {(XrStructureType)XR_TYPE_SCENE_STATUS_YVR};
    XrResult result = OpenXRAPI(xrGetSceneStatusYVR(plugin.openxrMgr->program->session, &sceneStatus));
    *status = sceneStatus.sceneCount;
    return result;
}

XrResult YVRSceneMgr::getLocateRoom(XrLocateRoomInfoYVR* roomInfo)
{
    AnnounceCallingFunc();
    if (roomInfo == nullptr)
    {
        YError("getLocateRoom failed: roomInfo is null.");
        return XR_ERROR_VALIDATION_FAILURE;
    }

    roomInfo->type = static_cast<XrStructureType>(XR_TYPE_LOCATE_ROOM_INFO_YVR);
    return OpenXRAPI(xrGetLocateRoomYVR(plugin.openxrMgr->program->session, roomInfo));
}

XrResult YVRSceneMgr::resetScene()
{
    AnnounceCallingFunc();
    XrResetSceneInfoYVR resetSceneYVR = {(XrStructureType)XR_TYPE_RESET_SCENE_INFO_YVR};
    XrResult result = OpenXRAPI(xrResetSceneYVR(plugin.openxrMgr->program->session, &resetSceneYVR));
    return result;
}

XrResult YVRSceneMgr::setLocateRoom(XrLocateRoomInfoYVR* roomInfo)
{
    AnnounceCallingFunc();
    if (roomInfo == nullptr)
    {
        YError("setLocateRoom failed: roomInfo is null.");
        return XR_ERROR_VALIDATION_FAILURE;
    }

    roomInfo->type = static_cast<XrStructureType>(XR_TYPE_LOCATE_ROOM_INFO_YVR);
    return OpenXRAPI(xrSetLocateRoomYVR(plugin.openxrMgr->program->session, roomInfo));
}

bool YVRSceneMgr::setSpaceTriangleMeshYVR(XrSpace space, SpaceTriangleMesh* spaceTriangleMesh)
{
    AnnounceCallingFunc();

    if (spaceTriangleMesh == nullptr)
    {
        YError("setSpaceTriangleMeshYVR failed: spaceTriangleMesh is null.");
        return false;
    }

    if (spaceTriangleMesh->vertices == nullptr || spaceTriangleMesh->indices == nullptr)
    {
        YError("setSpaceTriangleMeshYVR failed: vertices or indices is null.");
        return false;
    }

    if (spaceTriangleMesh->vertexCountOutput == 0 || spaceTriangleMesh->indexCountOutput == 0)
    {
        YError("setSpaceTriangleMeshYVR failed: vertex/index count is zero.");
        return false;
    }

    if (spaceTriangleMesh->vertexCapacityInput < spaceTriangleMesh->vertexCountOutput ||
        spaceTriangleMesh->indexCapacityInput < spaceTriangleMesh->indexCountOutput)
    {
        YError("setSpaceTriangleMeshYVR failed: vertex/index capacity is smaller than count.");
        return false;
    }

    // SpaceTriangleMesh from Unity uses Unity coordinates; convert to OpenXR before submit.
    std::vector<XrVector3f> convertedVertices(spaceTriangleMesh->vertexCountOutput);
    for (uint32_t i = 0; i < spaceTriangleMesh->vertexCountOutput; ++i)
    {
        convertedVertices[i] = ConvertPoseBetweenOpenXRandUnity(spaceTriangleMesh->vertices[i]);
    }

    XrSpaceTriangleMeshMETA triangleMesh = {
        XR_TYPE_SPACE_TRIANGLE_MESH_META,
        nullptr,
        static_cast<uint32_t>(convertedVertices.size()),
        static_cast<uint32_t>(convertedVertices.size()),
        convertedVertices.data(),
        spaceTriangleMesh->indexCapacityInput,
        spaceTriangleMesh->indexCountOutput,
        spaceTriangleMesh->indices
    };

    XrResult result = OpenXRAPI(xrSetSpaceTriangleMeshYVR(plugin.openxrMgr->program->session, space, &triangleMesh));
    if (result != XR_SUCCESS)
    {
        YError("setSpaceTriangleMeshYVR failed: xr result=%d", result);
    }

    return result == XR_SUCCESS;
}

XrResult YVRSceneMgr::getSpaceTriangleMeshMETA(XrSpace space, SpaceTriangleMesh* spaceTriangleMesh)
{
    const XrSpaceTriangleMeshGetInfoMETA getInfo = {XR_TYPE_SPACE_TRIANGLE_MESH_GET_INFO_META};
    XrSpaceTriangleMeshMETA triangleMesh = {XR_TYPE_SPACE_TRIANGLE_MESH_META};
    XrResult result = OpenXRAPI(xrGetSpaceTriangleMeshMETA(space, &getInfo, &triangleMesh));
    if (result != XR_SUCCESS)
    {
        YError("Failed getting triangle mesh! result:%d", result);
        return result;
    }
    if (spaceTriangleMesh->vertexCountOutput == 0 || spaceTriangleMesh->indexCountOutput == 0)
    {
        spaceTriangleMesh->indexCountOutput = triangleMesh.indexCountOutput;
        spaceTriangleMesh->vertexCountOutput = triangleMesh.vertexCountOutput;
        return result;
    }

    std::vector<XrVector3f> vertices(triangleMesh.vertexCountOutput);
    std::vector<uint32_t> indices(triangleMesh.indexCountOutput);
    triangleMesh.vertexCapacityInput = vertices.size();
    triangleMesh.vertices = vertices.data();
    triangleMesh.indexCapacityInput = indices.size();
    triangleMesh.indices = indices.data();
    result = OpenXRAPI(xrGetSpaceTriangleMeshMETA(space, &getInfo, &triangleMesh));

    if (result != XR_SUCCESS)
    {
        YError("Failed getting triangle mesh! result:%d", result);
        return result;
    }

    for (uint32_t i = 0; i < triangleMesh.vertexCountOutput; i++)
    {
        triangleMesh.vertices[i] = ConvertPoseBetweenOpenXRandUnity(triangleMesh.vertices[i]);
    }

    memcpy(spaceTriangleMesh->indices, triangleMesh.indices, sizeof(uint32_t) * triangleMesh.indexCountOutput);
    memcpy(spaceTriangleMesh->vertices, triangleMesh.vertices, sizeof(XrVector3f) * triangleMesh.vertexCountOutput);
    return result;
}
