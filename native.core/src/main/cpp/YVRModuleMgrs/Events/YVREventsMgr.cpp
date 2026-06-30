#include "YVREventsMgr.h"
#include "../../YVRUnityPlugin.h"
#include "../../YVRUnityXR/YVRMeshingProvider.h"
#include <string>
#include "YVREventType.h"
#include "../../Utilities/YVRCoordinateConverter.h"

YVREventsMgr::YVREventsMgr()
    : eventDataBuffer(),
      onEventCallback(nullptr)
{}

void YVREventsMgr::pollEvents()
{
    while (const XrEventDataBaseHeader* event = TryReadNextEvent())
    {
        handleEvent(event);
    }
}

void YVREventsMgr::handleEvent(const XrEventDataBaseHeader* event)
{
    switch (static_cast<int>(event->type))
    {
        case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING:
        {
            const auto& instanceLossPending = *reinterpret_cast<const XrEventDataInstanceLossPending*>(event);
            YWarn("XrEventDataInstanceLossPending by %lld", instanceLossPending.lossTime);
            return;
        }
        case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED:
        {
            auto sessionStateChangedEvent = *reinterpret_cast<const XrEventDataSessionStateChanged*>(event);
            handleSessionStateChangedEvent(sessionStateChangedEvent);
            break;
        }
        case XR_TYPE_EVENT_DATA_RECENTER_OCCURED_YVR:
        {
            triggerBaseEvents(kRecenter);
            break;
        }
        case XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING: //空间变化
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_REFERENCE_SPACE_CHANGE_PENDING event");
            break;
        }
        case XR_TYPE_EVENT_DATA_SPACE_SET_STATUS_COMPLETE_FB: //设置锚点状态完成
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SPACE_SET_STATUS_COMPLETE_FB event");
            auto enableResult = *reinterpret_cast<const XrEventDataSpaceSetStatusCompleteFB*>(event);
            handleSpaceSetStatusCompleteCallback(enableResult);
            break;
        }
        case XR_TYPE_EVENT_DATA_SPATIAL_ANCHOR_CREATE_COMPLETE_FB: //创建锚点完成
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SPATIAL_ANCHOR_CREATE_COMPLETE_FB event");
            auto spaceAnchorCreateCompleteEvent = *reinterpret_cast<const XrEventDataSpatialAnchorCreateCompleteFB*>(event);
            handleSpatialAnchorCreateCompleteEvent(spaceAnchorCreateCompleteEvent);
            break;
        }
        case XR_TYPE_EVENT_DATA_SPACE_SAVE_COMPLETE_FB: //保存锚点完成
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SPACE_SAVE_COMPLETE_FB event");
            auto saveResult = *reinterpret_cast<const XrEventDataSpaceSaveCompleteFB*>(event);
            handleSpaceSaveCompleteCallback(saveResult);
            break;
        }
        case XR_TYPE_EVENT_DATA_SPACE_LIST_SAVE_COMPLETE_FB:  // 批量保存锚点完成
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SPACE_LIST_SAVE_COMPLETE_FB");
            auto saveListResult = *reinterpret_cast<const XrEventDataSpaceListSaveCompleteFB*>(event);
            handleSpaceListSaveCompleteCallback(saveListResult);
            break;
        }
        case XR_TYPE_EVENT_DATA_SPACE_QUERY_RESULTS_AVAILABLE_FB: //查询到锚点
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SPACE_QUERY_RESULTS_AVAILABLE_FB");
            auto queryResult = *reinterpret_cast<const XrEventDataSpaceQueryResultsAvailableFB*>(event);
            uint64_t requestId = queryResult.requestId;
            plugin.spatialAnchorMgr->retrieveSpaceQueryResults(requestId);
            break;
        }
        case XR_TYPE_EVENT_DATA_SPACE_QUERY_COMPLETE_FB: //查询锚点结束
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SPACE_QUERY_COMPLETE_FB");
            auto queryCompleteResult = *reinterpret_cast<const XrEventDataSpaceQueryCompleteFB*>(event);
            if (queryCompleteResult.result == XR_SUCCESS)
            {
                handleSpatialAnchorQueryCompleteEvent(queryCompleteResult);
            }
            else
            {
                YWarn("xrPollEvent: query complete failed!");
            }

            break;
        }
        case XR_TYPE_EVENT_DATA_SPACE_ERASE_COMPLETE_FB: //删除锚点
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SPACE_ERASE_COMPLETE_FB");
            auto eraseResult = *reinterpret_cast<const XrEventDataSpaceEraseCompleteFB*>(event);
            handleErasedSpatialAnchorEvent(eraseResult);
            break;
        }
        case XR_TYPE_EVENT_DATA_SPACE_SHARE_COMPLETE_FB: //分享锚点完成
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SPACE_SHARE_COMPLETE_FB");
            auto shareResult = *reinterpret_cast<const XrEventDataSpaceShareCompleteFB*>(event);
            if (onSpaceShareCompleteCallback != nullptr)
            {
                onSpaceShareCompleteCallback(shareResult.result == XR_SUCCESS, shareResult.requestId);
            }

            break;
        }
        case XR_TYPE_EVENT_DATA_SCENE_CAPTURE_COMPLETE_FB:
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SCENE_CAPTURE_COMPLETE_FB");
            auto captureResult = *reinterpret_cast<const XrEventDataSceneCaptureCompleteFB*>(event);
            if (onSceneCaptureCompleteCallback != nullptr)
            {
                onSceneCaptureCompleteCallback(captureResult.requestId, captureResult.result == XR_SUCCESS);
            }

            break;
        }
        case XR_TYPE_EVENT_DATA_MESH_BLOCK_UPDATE_YVR: // Mesh 更新
        {
            auto meshBlockUpdate = *reinterpret_cast<const XrEventDataMeshBlockUpdateYVR*>(event);
            if (plugin.meshMgr->meshDetectorRuning)
            {
                if (OnMeshBlockUpdateCallback != nullptr)
                {
                    OnMeshBlockUpdateCallback(meshBlockUpdate.meshBlockSpace, meshBlockUpdate.changeState);
                }

                if (plugin.unityXRProviderMgr != nullptr && plugin.unityXRProviderMgr->meshProvider != nullptr)
                {
                    plugin.unityXRProviderMgr->meshProvider->meshBlockUpdate(meshBlockUpdate.meshBlockSpace, meshBlockUpdate.changeState);
                }
            }
            break;
        }
        case XR_TYPE_EVENT_DATA_ROOM_SETUP_UPDATE_YVR: //空间标定进度
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_ROOM_SETUP_UPDATE_YVR");
            auto roomSetupUpdate = *reinterpret_cast<const XrEventDataRoomSetupUpdateYVR*>(event);
            if (OnRoomSetupUpdateCallback != nullptr)
            {
                OnRoomSetupUpdateCallback(roomSetupUpdate.progress, roomSetupUpdate.roomSetupState);
            }

            YInfo("roomSetupUpdate progress:%f,state:%d", roomSetupUpdate.progress, roomSetupUpdate.roomSetupState);
            break;
        }
        case XR_TYPE_EVENT_DATA_ROOM_SETUP_COMPLETE_YVR: //空间标定完成
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_ROOM_SETUP_COMPLETE_YVR");
            auto roomSetupUpComplete = *reinterpret_cast<const XrEventDataRoomSetupCompleteYVR*>(event);
            if (OnRoomSetupCompleteCallback != nullptr)
            {
                OnRoomSetupCompleteCallback(roomSetupUpComplete.result, roomSetupUpComplete.roomLayoutSpace);
            }

            break;
        }
        case XR_TYPE_EVENT_DATA_IMAGE_TRACKING_UPDATE_YVR:
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_IMAGE_TRACKING_UPDATE_YVR");
            auto imageTemplateInfo = *reinterpret_cast<const XrEventDataImageTrackingUpdateYVR*>(event);

            plugin.imageTrackingMgr->onImageTracked(imageTemplateInfo);
            break;
        }
        case XR_TYPE_EVENT_DATA_BOUNDARY_UPDATE_YVR: //基于 mesh 的防护边界更新
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_BOUNDARY_UPDATE_YVR");
            auto boundaryUpdate = *reinterpret_cast<const XrEventDataBoundaryUpdateYVR*>(event);
            if (OnBoundaryUpdateCallback != nullptr)
            {
                OnBoundaryUpdateCallback(boundaryUpdate.boundarySpace);
            }

            YInfo("boundaryUpdate %d", boundaryUpdate.boundarySpace);
            break;
        }
        case XR_TYPE_EVENT_DATA_MARKER_TRACKING_UPDATE_YVR:
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_MARKER_TRACKING_UPDATE_YVR");
            auto markerTrackingUpdateYVR = *reinterpret_cast<const XrEventDataMarkerTrackingUpdateYVR*>(event);
            if (onMarkerTrackingUpdateYVRCallback != nullptr)
            {
                YVRMarkerTrackingUpdate yvrMarkerTrackingUpdate{};
                yvrMarkerTrackingUpdate.markerId = markerTrackingUpdateYVR.markerId;
                yvrMarkerTrackingUpdate.markerType = markerTrackingUpdateYVR.markerType;
                yvrMarkerTrackingUpdate.markerPose = ConvertPoseBetweenOpenXRandUnity(markerTrackingUpdateYVR.markerPose);
                yvrMarkerTrackingUpdate.confidence = markerTrackingUpdateYVR.confidence;
                memcpy(yvrMarkerTrackingUpdate.region, markerTrackingUpdateYVR.region, sizeof(markerTrackingUpdateYVR.region));
                onMarkerTrackingUpdateYVRCallback(yvrMarkerTrackingUpdate);
            }

            break;
        }
        case XR_TYPE_EVENT_DATA_LARGE_SPACE_RESULT_YVR:
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_LARGE_SPACE_RESULT_YVR");
            auto largeSpaceResult = *reinterpret_cast<const XrEventDataLargeSpaceResultYVR*>(event);
            if (onLargeSpaceResultYVR != nullptr)
            {
                onLargeSpaceResultYVR(largeSpaceResult.resultCode);
            }

            break;
        }
        case XR_TYPE_EVENT_DATA_LARGE_SPACE_MAP_A_UPDATE_YVR:
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_LARGE_SPACE_MAP_A_UPDATE_YVR");
            auto largeSpaceMapAUpdate = *reinterpret_cast<const XrEventDataLargeSpaceMapAUpdateYVR*>(event);
            if (onLargeSpaceMapAUpdate != nullptr)
            {
                onLargeSpaceMapAUpdate(largeSpaceMapAUpdate.estimated_time_consuming, largeSpaceMapAUpdate.completed_percentage);
            }

            break;
        }
        case XR_TYPE_EVENT_DATA_SLAM_CALIBRATION_YVR:
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SLAM_CALIBRATION_YVR");
            auto slamCalibrationResult = *reinterpret_cast<const XrEventDataSlamCalibrationResultYVR*>(event);
            if (onSlamCalibration != nullptr) onSlamCalibration(slamCalibrationResult.resultCode);
            break;
        }
        case XR_TYPE_EVENT_DATA_PLANE_UPDATE_YVR:
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_PLANE_UPDATE_YVR");
            if (plugin.planeDetectionMgr->planeDetectorRuning)
                plugin.planeDetectionMgr->getPlaneDetections();

            break;
        }
        case XR_TYPE_EVENT_DATA_SLAM_TRAVEL_MODE_STATE_CHANGE_YVR: //travel mode state change
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SLAM_TRAVEL_MODE_STATE_CHANGE_YVR");
            auto travelModeStateChangeEvent = *reinterpret_cast<const XrEventSlamTravelModeStateChangeYVR  *>(event);
            if (onTravelModeStateChange != nullptr) onTravelModeStateChange((XrSlamTravelModeStateYVR)travelModeStateChangeEvent.state);

            break;
        }
        case XR_TYPE_EVENT_DATA_SLAM_TRAVEL_MODE_CHANGE_YVR://travel mode set result
        {
            YInfo("xrPollEvent: received XR_TYPE_EVENT_DATA_SLAM_TRAVEL_MODE_CHANGE_YVR");
            auto travelModeChangeEvent = *reinterpret_cast<const XrEventSlamTravelModeChangeYVR*>(event);
            if (onTravelModeState != nullptr) onTravelModeState((XrSlamTravelModeYVR)travelModeChangeEvent.mode);

            break;
        }
        default:
        {
            YInfo("Ignoring event type %d", event->type);
            break;
        }
    }
}


void YVREventsMgr::setEventCallback(OnYVREvent callback)
{
    AnnounceCallingFunc();
    onEventCallback = callback;
}

void YVREventsMgr::setSpaceDetectRangeChangeCallback(OnSpaceDetectRangeChange callback)
{
    AnnounceCallingFunc();
    onSpaceDetectRangeChangeCallback = callback;
}

void YVREventsMgr::setSpatialAnchorCreateCompleteCallback(OnSpatialAnchorCreateComplete callback)
{
    AnnounceCallingFunc();
    onSpatialAnchorCreateCompleteCallback = callback;
}

void YVREventsMgr::setSpatialAnchorQueryCompleteCallback(OnSpatialAnchorQueryComplete callback)
{
    AnnounceCallingFunc();
    onSpatialAnchorQueryCompleteCallback = callback;
}

void YVREventsMgr::setEraseSpatialAnchorCallback(OnSetEraseSpatialAnchor callback)
{
    AnnounceCallingFunc();
    onSetEraseSpatialAnchorCallback = callback;
}

void YVREventsMgr::setSpaceShareCompleteCallback(OnSpaceShareComplete callback)
{
    AnnounceCallingFunc();
    onSpaceShareCompleteCallback = callback;
}

void YVREventsMgr::setSpaceSaveCompleteCallback(OnSpaceSaveComplete callback)
{
    AnnounceCallingFunc();
    onSpaceSaveCompleteCallback = callback;
}

void YVREventsMgr::setSpaceListSaveCompleteCallback(OnSpaceListSaveComplete callback)
{
    AnnounceCallingFunc();
    OnSpaceListSaveCompleteCallback = callback;
}

void YVREventsMgr::setSpaceSetStatusCompleteCallback(OnSpaceSetStatusComplete callback)
{
    AnnounceCallingFunc();
    onSpaceSetStatusCompleteCallback = callback;
}

void YVREventsMgr::setGlobalAnchorCreateCompleteCallback(OnGlobalAnchorCreateComplete callback)
{
    AnnounceCallingFunc();
    onGlobalAnchorCreateCompleteCallback = callback;
}

void YVREventsMgr::setRequestSceneCaptureCallback(OnSceneCaptureComplete callback)
{
    AnnounceCallingFunc();
    onSceneCaptureCompleteCallback = callback;
}

void YVREventsMgr::setMeshBlockUpdateCallback(OnMeshBlockUpdate callback) { OnMeshBlockUpdateCallback = callback; }
void YVREventsMgr::setRoomSetupUpdateCallback(OnRoomSetupUpdate callback) { OnRoomSetupUpdateCallback = callback; }
void YVREventsMgr::setRoomSetupCompleteCallback(OnRoomSetupComplete callback) { OnRoomSetupCompleteCallback = callback; }
void YVREventsMgr::setBoundaryUpdateCallback(OnBoundaryUpdate callback) { OnBoundaryUpdateCallback = callback; }
void YVREventsMgr::setMarkerTrackingUpdateYVRCallback(OnMarkerTrackingUpdateYVR callback) { onMarkerTrackingUpdateYVRCallback = callback; }
void YVREventsMgr::setLargeSpaceResultYVRCallback(OnLargeSpaceResultYVR callback) { onLargeSpaceResultYVR = callback; }
void YVREventsMgr::setOnLargeSpaceMapAUpdateCallback(OnLargeSpaceMapAUpdate callback) { onLargeSpaceMapAUpdate = callback; }
void YVREventsMgr::setOnSlamCalibrationCallback(OnSlamCalibration callback) { onSlamCalibration = callback; }
void YVREventsMgr::setOnSessionStateChangeCallback(OnSessionStateChange callback) { onSessionStateChange = callback; }
void YVREventsMgr::setOnTravelModeStateChangeCallback(OnTravelModeStateChange callback) { onTravelModeStateChange = callback; }
void YVREventsMgr::setOnTravelModeStateCallback(OnTravelModeState callback) { onTravelModeState = callback; }


const XrEventDataBaseHeader* YVREventsMgr::TryReadNextEvent()
{
    // It is sufficient to clear the just the XrEventDataBuffer header to
    // XR_TYPE_EVENT_DATA_BUFFER
    const auto baseHeader = reinterpret_cast<XrEventDataBaseHeader*>(&eventDataBuffer);
    *baseHeader = {XR_TYPE_EVENT_DATA_BUFFER};
    const XrResult xr = xrPollEvent(plugin.openxrMgr->program->instance, &eventDataBuffer);
    if (xr == XR_SUCCESS)
    {
        if (baseHeader->type == XR_TYPE_EVENT_DATA_EVENTS_LOST)
        {
            const auto* const eventsLost = reinterpret_cast<const XrEventDataEventsLost*>(baseHeader);
            YWarn("%d events lost", eventsLost);
        }

        return baseHeader;
    }

    return nullptr;
}

void YVREventsMgr::triggerBaseEvents(int eventId)
{
    if (onEventCallback != nullptr) onEventCallback(eventId);
}

void YVREventsMgr::handleSessionStateChangedEvent(const XrEventDataSessionStateChanged& stateChangedEvent)
{
    XrSessionState& state = plugin.lifecycleMgr->sessionState;
    const XrSessionState oldState = state;
    state = stateChangedEvent.state;
    YInfo("XrEventDataSessionStateChanged: state %s->%s session=%lld time=%lld", to_string(oldState), to_string(state), stateChangedEvent.session,
          stateChangedEvent.time);

    if ((stateChangedEvent.session != XR_NULL_HANDLE) && (stateChangedEvent.session != plugin.openxrMgr->program->session))
    {
        YError("XrEventDataSessionStateChanged for unknown session");
        return;
    }

    if (onSessionStateChange != nullptr)
    {
        YInfo("onSessionStateChange:%d", state);
        onSessionStateChange(state);
    }

    switch (state)
    {
        case XR_SESSION_STATE_IDLE:
        {
            plugin.lifecycleMgr->setFocus(false);
            plugin.lifecycleMgr->setVisible(false);
            break;
        }
        case XR_SESSION_STATE_READY:
        {
            plugin.lifecycleMgr->enterVrMode();
            break;
        }
        case XR_SESSION_STATE_VISIBLE:
        {
            plugin.lifecycleMgr->setFocus(false);
            plugin.lifecycleMgr->setVisible(true);
            break;
        }
        case XR_SESSION_STATE_FOCUSED:
        {
            plugin.lifecycleMgr->setFocus(true);
            plugin.lifecycleMgr->setVisible(true);
            break;
        }
        case XR_SESSION_STATE_SYNCHRONIZED:
        {
            plugin.lifecycleMgr->setFocus(false);
            plugin.lifecycleMgr->setVisible(false);
            break;
        }
        case XR_SESSION_STATE_STOPPING:
        {
            plugin.lifecycleMgr->leaveVrMode();
            break;
        }
        default:
            break;
    }
}

void YVREventsMgr::handleSpatialAnchorCreateCompleteEvent(const XrEventDataSpatialAnchorCreateCompleteFB& spaceAnchorCreateCompleteEvent)
{
    AnnounceCallingFunc();
    string str = Uint8ToHexString(spaceAnchorCreateCompleteEvent.uuid.data, XR_UUID_SIZE_EXT);
    YVRSpaceAnchorResult result{};
    result.space = spaceAnchorCreateCompleteEvent.space;
    result.requestId = spaceAnchorCreateCompleteEvent.requestId;
    memcpy(result.uuid, str.c_str(), str.length());
    bool createResult = spaceAnchorCreateCompleteEvent.result == XR_SUCCESS;
    if (!createResult)
    {
        YWarn("xrPollEvent: Create spatial anchor failed! result code: %d", spaceAnchorCreateCompleteEvent.result);
    }

    if (onGlobalAnchorCreateCompleteCallback != nullptr)
    {
        onGlobalAnchorCreateCompleteCallback(result, createResult);
    }

    if (onSpatialAnchorCreateCompleteCallback != nullptr)
    {
        onSpatialAnchorCreateCompleteCallback(result, createResult);
    }

    AnnounceCalledFunc();
}

void YVREventsMgr::handleSpatialAnchorQueryCompleteEvent(const XrEventDataSpaceQueryCompleteFB& spaceAnchorQueryCompleteEvent)
{
    if (onSpatialAnchorQueryCompleteCallback == nullptr) return;
    std::vector<YVRSpaceAnchorResult> results{};
    if (plugin.spatialAnchorMgr->spaceMap.count(spaceAnchorQueryCompleteEvent.requestId) == 1)
    {
        results = plugin.spatialAnchorMgr->spaceMap[spaceAnchorQueryCompleteEvent.requestId];
    }

    YVRQuerySpaceResult querySpaceResult;
    querySpaceResult.numResults = results.size();
    memcpy(querySpaceResult.spaces, results.data(), sizeof(YVRSpaceAnchorResult) * results.size());
    onSpatialAnchorQueryCompleteCallback(querySpaceResult, spaceAnchorQueryCompleteEvent.requestId);
    AnnounceCalledFunc();
}

void YVREventsMgr::handleErasedSpatialAnchorEvent(const XrEventDataSpaceEraseCompleteFB& eraseResult)
{
    if (onSetEraseSpatialAnchorCallback == nullptr) return;
    const std::string hexStr = Uint8ToHexString(eraseResult.uuid.data,XR_UUID_SIZE_EXT);
    YVRSpaceAnchorResult result;
    memcpy(result.uuid, hexStr.c_str(), hexStr.length());
    result.space = eraseResult.space;
    result.requestId = eraseResult.requestId;
    bool erasedSuccess = eraseResult.result == XR_SUCCESS;
    if (!erasedSuccess)
    {
        YWarn("xrPollEvent: Erase spatial anchor failed! result code: %d", eraseResult.result);
    }

    onSetEraseSpatialAnchorCallback(result, erasedSuccess);
    AnnounceCalledFunc();
}

void YVREventsMgr::handleSpaceSaveCompleteCallback(const XrEventDataSpaceSaveCompleteFB& saveResult)
{
    if (onSpaceSaveCompleteCallback == nullptr) return;
    YVRSpaceSaveCompleteInfo saveCompleteInfo{};
    saveCompleteInfo.requestId = saveResult.requestId;
    saveCompleteInfo.resultCode = saveResult.result;
    saveCompleteInfo.space = saveResult.space;
    saveCompleteInfo.location = saveResult.location;
    const std::string hexStr = Uint8ToHexString(saveResult.uuid.data,XR_UUID_SIZE_EXT);
    memcpy(saveCompleteInfo.uuid, hexStr.c_str(), hexStr.length());
    bool result = saveResult.result == XR_SUCCESS;
    if (!result)
    {
        YWarn("xrPollEvent: Save spatial anchor failed! result code: %d", saveResult.result);
    }

    onSpaceSaveCompleteCallback(saveCompleteInfo, result);
    AnnounceCalledFunc();
}

void YVREventsMgr::handleSpaceListSaveCompleteCallback(const XrEventDataSpaceListSaveCompleteFB& saveListResult)
{
    if (OnSpaceListSaveCompleteCallback == nullptr) return;
    OnSpaceListSaveCompleteCallback(saveListResult.result == XR_SUCCESS, saveListResult.requestId);
    AnnounceCalledFunc();
}

void YVREventsMgr::handleSpaceSetStatusCompleteCallback(const XrEventDataSpaceSetStatusCompleteFB& enableResult)
{
    if (onSpaceSetStatusCompleteCallback == nullptr) return;
    YVRSpaceSetStatusCompleteInfo statusCompleteInfo{};
    statusCompleteInfo.requestId = enableResult.requestId;
    statusCompleteInfo.resultCode = enableResult.result;
    statusCompleteInfo.space = enableResult.space;
    statusCompleteInfo.componentType = enableResult.componentType;
    statusCompleteInfo.enabled = enableResult.enabled;
    const std::string hexStr = Uint8ToHexString(enableResult.uuid.data,XR_UUID_SIZE_EXT);
    memcpy(statusCompleteInfo.uuid, hexStr.c_str(), hexStr.length());
    bool result = enableResult.result == XR_SUCCESS;
    if (!result)
    {
        YWarn("xrPollEvent: Spatial anchor set status failed! result code: %d", enableResult.result);
    }

    onSpaceSetStatusCompleteCallback(statusCompleteInfo, result);
    AnnounceCalledFunc();
}

YVREventsMgr::~YVREventsMgr() = default;
