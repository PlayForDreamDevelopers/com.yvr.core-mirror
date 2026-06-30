#pragma once

#include "../../OpenXR/Common.h"
#include "pfdm_openxr_preview/openxr_yvr_private.h"
#include "../YVRSpatialAnchorMgr.h"
#include "../YVRLargeSpatialMgr.h"

struct TrackedImageInfo;
typedef void (*OnYVREvent)(int eventId);
typedef void (*OnGroundDistanceDetected)(float distance);
typedef void (*OnSpaceDetectRangeChange)(float range);
typedef void (*OnSpatialAnchorCreateComplete)(YVRSpaceAnchorResult result, bool success);
typedef void (*OnSpatialAnchorQueryComplete)(YVRQuerySpaceResult result, uint64_t request);
typedef void (*OnSetEraseSpatialAnchor)(YVRSpaceAnchorResult result, bool success);
typedef void (*OnSpaceShareComplete)(bool result, uint64_t requestId);
typedef void (*OnSpaceSaveComplete)(YVRSpaceSaveCompleteInfo result, bool success);
typedef void (*OnSpaceSetStatusComplete)(YVRSpaceSetStatusCompleteInfo result, bool success);
typedef void (*OnSceneCaptureComplete)(uint64_t reuqestId, bool success);
typedef void (*OnGlobalAnchorCreateComplete)(YVRSpaceAnchorResult result, bool success);
typedef void (*OnSpaceListSaveComplete)(bool success, uint64_t requestId);
typedef void (*OnMeshBlockUpdate)(XrSpace meshBlockSpace, XrMeshBlockChangeStateYVR changeState);
typedef void (*OnRoomSetupUpdate)(float progress, XrRoomSetupStateYVR roomSetupState);
typedef void (*OnRoomSetupComplete)(bool result, XrSpace roomLayoutSpace);
typedef void (*OnBoundaryUpdate)(XrSpace boundarySpace);
typedef void (*OnMarkerTrackingUpdateYVR)(YVRMarkerTrackingUpdate markerTrackingUpdateYVR);
typedef void (*OnLargeSpaceResultYVR)(int largeSpaceResultYVR);
typedef void (*OnLargeSpaceMapAUpdate)(float timeConsuming, float completedPercentage);
typedef void (*OnSlamCalibration)(int resultCode);
typedef void (*OnSessionStateChange)(XrSessionState state);
typedef void (*OnTravelModeStateChange)(XrSlamTravelModeStateYVR state);
typedef void (*OnTravelModeState)(XrSlamTravelModeYVR state);

class YVREventsMgr
{
public:
    YVREventsMgr();
    ~YVREventsMgr();

    void pollEvents();
    void triggerBaseEvents(int eventId);

    void setEventCallback(OnYVREvent callback);
    void setGroundDistanceDetectedCallback(OnGroundDistanceDetected callback);
    void setSpaceDetectRangeChangeCallback(OnSpaceDetectRangeChange callback);
    void setSpatialAnchorCreateCompleteCallback(OnSpatialAnchorCreateComplete callback);
    void setSpatialAnchorQueryCompleteCallback(OnSpatialAnchorQueryComplete callback);
    void setEraseSpatialAnchorCallback(OnSetEraseSpatialAnchor callback);
    void setSpaceShareCompleteCallback(OnSpaceShareComplete callback);
    void setSpaceSaveCompleteCallback(OnSpaceSaveComplete callback);
    void setSpaceSetStatusCompleteCallback(OnSpaceSetStatusComplete callback);
    void setGlobalAnchorCreateCompleteCallback(OnGlobalAnchorCreateComplete callback);
    void setSpaceListSaveCompleteCallback(OnSpaceListSaveComplete callback);
    void setRequestSceneCaptureCallback(OnSceneCaptureComplete callback);
    void setMeshBlockUpdateCallback(OnMeshBlockUpdate callback);
    void setRoomSetupUpdateCallback(OnRoomSetupUpdate callback);
    void setRoomSetupCompleteCallback(OnRoomSetupComplete callback);
    void setBoundaryUpdateCallback(OnBoundaryUpdate callback);
    void setMarkerTrackingUpdateYVRCallback(OnMarkerTrackingUpdateYVR callback);
    void setLargeSpaceResultYVRCallback(OnLargeSpaceResultYVR callback);
    void setOnLargeSpaceMapAUpdateCallback(OnLargeSpaceMapAUpdate callback);
    void setOnSlamCalibrationCallback(OnSlamCalibration callback);
    void setOnSessionStateChangeCallback(OnSessionStateChange callback);
    void setOnTravelModeStateChangeCallback(OnTravelModeStateChange callback);
    void setOnTravelModeStateCallback(OnTravelModeState callback);

    void handleEvent(const XrEventDataBaseHeader* event);
    const XrEventDataBaseHeader* TryReadNextEvent();

private:
    void handleSessionStateChangedEvent(const XrEventDataSessionStateChanged& stateChangedEvent);
    void handleGroundDistanceEvent(const XrEventDataGroundDistanceYVR& groundDistanceEvent);
    void handleSpatialAnchorCreateCompleteEvent(const XrEventDataSpatialAnchorCreateCompleteFB& spaceAnchorCreateCompleteEvent);
    void handleSpatialAnchorQueryCompleteEvent(const XrEventDataSpaceQueryCompleteFB& spaceAnchorQueryCompleteEvent);
    void handleErasedSpatialAnchorEvent(const XrEventDataSpaceEraseCompleteFB& eraseSpatialAnchorEvent);
    void handleSpaceSaveCompleteCallback(const XrEventDataSpaceSaveCompleteFB& saveResult);
    void handleSpaceSetStatusCompleteCallback(const XrEventDataSpaceSetStatusCompleteFB& enableResult);
    void handleSpaceListSaveCompleteCallback(const XrEventDataSpaceListSaveCompleteFB& saveListResult);
    void handleRequestSceneCaptureCallback(const XrEventDataSceneCaptureCompleteFB& sceneCaptureComplete);

    XrEventDataBuffer eventDataBuffer;

    OnYVREvent onEventCallback = nullptr;
    OnSpaceDetectRangeChange onSpaceDetectRangeChangeCallback = nullptr;
    OnSpatialAnchorCreateComplete onSpatialAnchorCreateCompleteCallback = nullptr;
    OnSpatialAnchorQueryComplete onSpatialAnchorQueryCompleteCallback = nullptr;
    OnSetEraseSpatialAnchor onSetEraseSpatialAnchorCallback = nullptr;
    OnSpaceShareComplete onSpaceShareCompleteCallback = nullptr;
    OnSpaceSaveComplete onSpaceSaveCompleteCallback = nullptr;
    OnSpaceSetStatusComplete onSpaceSetStatusCompleteCallback = nullptr;
    OnSceneCaptureComplete onSceneCaptureCompleteCallback = nullptr;
    OnGlobalAnchorCreateComplete onGlobalAnchorCreateCompleteCallback = nullptr;
    OnSpaceListSaveComplete OnSpaceListSaveCompleteCallback = nullptr;
    OnMeshBlockUpdate OnMeshBlockUpdateCallback = nullptr;
    OnRoomSetupUpdate OnRoomSetupUpdateCallback = nullptr;
    OnRoomSetupComplete OnRoomSetupCompleteCallback = nullptr;
    OnBoundaryUpdate OnBoundaryUpdateCallback = nullptr;
    OnMarkerTrackingUpdateYVR onMarkerTrackingUpdateYVRCallback = nullptr;
    OnLargeSpaceResultYVR onLargeSpaceResultYVR = nullptr;
    OnLargeSpaceMapAUpdate onLargeSpaceMapAUpdate = nullptr;
    OnSlamCalibration onSlamCalibration = nullptr;
    OnSessionStateChange onSessionStateChange = nullptr;
    OnTravelModeStateChange onTravelModeStateChange = nullptr;
    OnTravelModeState onTravelModeState = nullptr;
};
