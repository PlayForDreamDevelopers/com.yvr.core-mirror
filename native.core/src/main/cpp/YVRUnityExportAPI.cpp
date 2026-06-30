#include "YVRUnityPlugin.h"
#include "YVRUnityXR/YVRInputProvider.h"
#include "UnityHeader/IUnityInterface.h"
#include "Utilities/YVRCoordinateConverter.h"
#include "YVRModuleMgrs/Inputs/Hands/Handness.h"
#include "YVRModuleMgrs/Inputs/Hands/XRHandJointLocation.h"

#define YVR_EXPORT

YVRUnityPlugin plugin;

extern "C"
{
YVR_EXPORT void YVRSetVSyncCount(int vSyncCount)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setFrameVsyncCount(vSyncCount);
}

YVR_EXPORT YVRStereoRenderMode YVRGetActualStereoRenderingMode()
{
    AnnounceCallingFunc();
    return plugin.renderMgr->configsMgr->getActualRenderingMode();
}

YVR_EXPORT void YVRSetControllerVibration(xrControllerSide mask, float frequency, float amplitude)
{
    AnnounceCallingFunc();
    plugin.controllersMgr->setControllerVibration(mask == kControllerLeft, amplitude, frequency, 1.5f * 1000 * 1000 * 1000);
}

YVR_EXPORT void YVRSetControllerVibrationWithDuration(xrControllerSide mask, float frequency, float amplitude, float duration)
{
    AnnounceCallingFunc();
    plugin.controllersMgr->setControllerVibration(mask == kControllerLeft, amplitude, frequency, duration * 1000 * 1000 * 1000);
}

YVR_EXPORT float YVRGetBatteryLevel()
{
    float ret = plugin.openxrMgr->extMgr->getFloatProperty(YVR_SYS_PROP_GET_BATTERY_LEVEL);
    return ret;
}

YVR_EXPORT int YVRGetBatteryStatus() { return plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_BATTERY_CHARGE_STATUS); }

YVR_EXPORT float YVRGetBatteryTemperature() { return plugin.openxrMgr->extMgr->getFloatProperty(YVR_SYS_PROP_GET_BATTERY_TEMPERATURE); }

YVR_EXPORT float YVRGetVolumeLevel() { return plugin.openxrMgr->extMgr->getFloatProperty(YVR_SYS_PROP_GET_VOLUME_LEVEL); }

YVR_EXPORT int YVRGetCpuLevel()
{
    return plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_CPU_LEVEL);;
}

YVR_EXPORT int YVRGetGpuLevel()
{
    return plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_GPU_LEVEL);;
}

YVR_EXPORT float YVRGetCpuUtilization() { return plugin.openxrMgr->extMgr->getFloatProperty(YVR_SYS_PROP_GET_CPU_UTILIZATION); }

YVR_EXPORT float YVRGetGpuUtilization() { return plugin.openxrMgr->extMgr->getFloatProperty(YVR_SYS_PROP_GET_GPU_UTILIZATION); }

YVR_EXPORT void YVRGetEyeResolution(XrVector2f* resolution)
{
    AnnounceCallingFunc();
    resolution->x = plugin.renderMgr->configsMgr->getEyeBufferResolution().x;
    resolution->y = plugin.renderMgr->configsMgr->getEyeBufferResolution().y;
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRGetEyeFov(int eyeSide, XrFovf* eyeFov)
{
    AnnounceCallingFunc();
    plugin.hmdMgr->refreshViews(false);
    memcpy(eyeFov, &plugin.hmdMgr->physicalViews[eyeSide].fov, sizeof(XrFovf));
    AnnounceCalledFunc();
}

YVR_EXPORT bool YVRIsUserPresent() { return plugin.hmdMgr->isUserPresent(); }

YVR_EXPORT float YVRGetDisplayFrequency() { return static_cast<float>(plugin.renderMgr->configsMgr->getDisplayRate()); }

YVR_EXPORT void YVRSetDisplayFrequency(float refreshRate) { plugin.renderMgr->configsMgr->setDisplayRate(static_cast<int>(refreshRate)); }

YVR_EXPORT int YVRGetDisplayAvailableFrequenciesNum()
{
    uint32_t ret{};
    OpenXRAPI(xrEnumerateDisplayRefreshRatesFB(plugin.openxrMgr->program->session, 0, &ret, nullptr));
    return static_cast<int>(ret);
}

YVR_EXPORT void YVRGetDisplayAvailableFrequencies(float* frequenciesArray)
{
    uint32_t number = YVRGetDisplayAvailableFrequenciesNum();
    OpenXRAPI(xrEnumerateDisplayRefreshRatesFB(plugin.openxrMgr->program->session, number, &number, frequenciesArray));
}

YVR_EXPORT void YVRSetPerformanceLevel(int cpuLevel, int gpuLevel)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_CPU_LEVEL, cpuLevel));
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_GPU_LEVEL, gpuLevel));
}

YVR_EXPORT void YVRSetXRUserDefinedSettings(YVRXRUserDefinedSettings* userDefinedSettings)
{
    AnnounceCallingFunc();
    plugin.unityXRProviderMgr->setXrUserDefinedSettings(userDefinedSettings);
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) { return plugin.lifecycleMgr->onJniLoaded(vm, reserved); }

YVR_EXPORT void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
    plugin.lifecycleMgr->onUnityPluginLoaded(unityInterfaces);
}

YVR_EXPORT void UNITY_INTERFACE_API UnityPluginUnload() { plugin.lifecycleMgr->onUnityPluginUnLoaded(); }

YVR_EXPORT int YVRGetLayerColorHandle(int layerId, int bufferIndex)
{
    return plugin.renderMgr->renderLayersMgr->getLayerColorHandle(layerId, xrEyeSide::kEyeMaskLeft, bufferIndex);
}

YVR_EXPORT void YVRSetAppSWEnable(bool enable) { plugin.renderMgr->configsMgr->setAppSWEnable(enable); }
YVR_EXPORT bool YVRGetAppSWEnable() { return plugin.renderMgr->configsMgr->isAppSWEnable(); }

YVR_EXPORT void YVRSetAppSWSwitch(bool isOn) { plugin.renderMgr->configsMgr->setAppSWSwitch(isOn); }
YVR_EXPORT bool YVRGetAppSWSwitch() { return plugin.renderMgr->configsMgr->isAppSWSwitchOn(); }

YVR_EXPORT void YVRSetAppSpacePosition(float x, float y, float z)
{
    plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->setCurrentPosePosition(x, y, z);
}

YVR_EXPORT void YVRSetAppSpaceRotation(float x, float y, float z, float w)
{
    plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->setCurrentPoseRotation(x, y, z, w);
}

YVR_EXPORT int YVRGetPrimaryController(XrControllerYVR* mask)
{
    *mask = plugin.controllersMgr->getPrimaryController();
    return 1;
}

YVR_EXPORT int YVRSetPrimaryController(XrControllerYVR mask)
{
    plugin.controllersMgr->setPrimaryController(mask);
    return 1;
}

YVR_EXPORT void YVRSetControllerModelType(int type)
{
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_CONTROLLER_MODEL_TYPE, type));
}

YVR_EXPORT int YVRGetControllerModelType()
{
    return plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_CONTROLLER_MODEL_TYPE);
}

YVR_EXPORT void YVRSetAntiGlareLevel(int level)
{
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_ANTI_GLARE_LEVEL, level));
}

YVR_EXPORT int YVRGetAntiGlareLevel()
{
    return plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_ANTI_GLARE_LEVEL);
}

YVR_EXPORT void YVRRecenterPose() { plugin.hmdMgr->recenterPose(); }

YVR_EXPORT void YVRSetTrackingSpace(int trackingSpace) { plugin.inputMgr->setTrackingSpace((XrReferenceSpaceType)trackingSpace); }

YVR_EXPORT int YVRGetTrackingSpace() { return (int)plugin.inputMgr->getTrackingSpace(); }

YVR_EXPORT float YVRGetGroundDistance()
{
    AnnounceCallingFunc();
    float ret;
    OpenXRAPI(xrGetGroundDistanceYVR(plugin.openxrMgr->program->session, &ret));
    return ret;
}

YVR_EXPORT bool YVRGetBoundaryConfigured()
{
    AnnounceCallingFunc();
    XrBool32 ret{};
    OpenXRAPI(xrGetBoundaryConfiguredYVR(plugin.openxrMgr->program->session, &ret));
    return static_cast<bool>(ret);
}

YVR_EXPORT void YVRTestBoundaryNode(const XrBoundaryTestNodeYVR deviceNode, XrBoundaryTestResultYVR* result)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrTestBoundaryNodeYVR(plugin.openxrMgr->program->session, deviceNode, result));
}

YVR_EXPORT void YVRTestBoundaryPoint(const XrVector3f point, XrBoundaryTestResultYVR* result)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrTestBoundaryPointYVR(plugin.openxrMgr->program->session, ConvertPoseBetweenOpenXRandUnity(point), result));
}

XrVector3f dimensions;
YVR_EXPORT XrVector3f YVRGetBoundaryDimensions()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrGetBoundaryDimensionsYVR(plugin.openxrMgr->program->session, &dimensions));
    dimensions = ConvertPoseBetweenOpenXRandUnity(dimensions);
    return dimensions;
}

YVR_EXPORT bool YVRGetBoundaryVisible()
{
    AnnounceCallingFunc();
    bool ret;
    OpenXRAPI(xrGetBoundaryVisibleYVR(plugin.openxrMgr->program->session, reinterpret_cast<XrBool32*>(&ret)));
    return ret;
}

YVR_EXPORT void YVRSetBoundaryVisible(bool visible)
{
    AnnounceCallingFunc();
    int xrBoundaryVisible = visible ? BOUNDARY_FORCE_VISIBLE_YVR : BOUNDARY_NON_VISIBLE_YVR;
    XrBoundaryControlData controlData;
    controlData.action = ACTION_SET_BOUNDARY_VISIBLE;
    controlData.dataLengthBytes = sizeof(int32_t);
    controlData.controlData = reinterpret_cast<uint8_t*>(&xrBoundaryVisible);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVRSetBoundaryDetectionEnable(bool enable)
{
    AnnounceCallingFunc();
    int xrBoundaryVisible = enable ? BOUNDARY_ENABLE_DETECTION : BOUNDARY_DISABLE_DETECTION;
    XrBoundaryControlData controlData;
    controlData.action = ACTION_SET_BOUNDARY_DETECTION_ENABLE;
    controlData.dataLengthBytes = sizeof(int32_t);
    controlData.controlData = reinterpret_cast<uint8_t*>(&xrBoundaryVisible);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT int YVRGetBoundaryGeometryPointsCount()
{
    AnnounceCallingFunc();
    uint32_t ret;
    OpenXRAPI(xrGetBoundaryGeometryPointsCountYVR(plugin.openxrMgr->program->session, &ret));
    return static_cast<int>(ret);
}

YVR_EXPORT void YVRGetBoundaryGeometry(XrVector3f* boundaryData)
{
    AnnounceCallingFunc();

    const uint32_t length = YVRGetBoundaryGeometryPointsCount();
    OpenXRAPI(xrGetBoundaryGeometryYVR(plugin.openxrMgr->program->session, length, boundaryData));
    for (int i = 0; i < length; i++)
    {
        boundaryData[i] = ConvertPoseBetweenOpenXRandUnity(boundaryData[i]);
    }
}

YVR_EXPORT bool YVRIsFocusing() { return plugin.lifecycleMgr->isFocusing(); }
YVR_EXPORT bool YVRIsVisible() { return plugin.lifecycleMgr->isVisible(); }
YVR_EXPORT void YVRSetEventCallback(OnYVREvent callback) { plugin.eventsMgr->setEventCallback(callback); }

YVR_EXPORT void YVRGetHandHandJointLocations(const Handedness handedness, XRHandJointLocations* handJointLocations, void* locations,
                                             void* velocities)
{
    // TODO: Convert coordinate here
    const XRHandData& handData = handedness == HAND_LEFT ? plugin.handMgr->handDataL : plugin.handMgr->handDataR;

    const auto locationsData = static_cast<XrHandJointLocationEXT*>(locations);
    const auto velocitiesData = static_cast<XrHandJointVelocityEXT*>(velocities);
    for (size_t i = 0; i < handData.locations.jointCount; i++)
    {
        locationsData[i] = handData.locations.jointLocations[i];
        locationsData[i].pose = ConvertPoseBetweenOpenXRandUnity(locationsData[i].pose);
        velocitiesData[i] = handData.velocities.jointVelocities[i];
        velocitiesData[i].linearVelocity = ConvertPoseBetweenOpenXRandUnity(velocitiesData[i].linearVelocity);
        velocitiesData[i].angularVelocity = ConvertPoseBetweenOpenXRandUnity(velocitiesData[i].angularVelocity);
    }

    handJointLocations->isActive = handData.locations.isActive;
    handJointLocations->jointCount = handData.locations.jointCount;
    handJointLocations->handScale = handData.scale.currentOutput;
    handJointLocations->aimState.aimPose = ConvertPoseBetweenOpenXRandUnity(handData.aimState.aimPose);
    handJointLocations->aimState.status = handData.aimState.status;
    handJointLocations->aimState.pinchStrengthIndex = handData.aimState.pinchStrengthIndex;
    handJointLocations->aimState.pinchStrengthLittle = handData.aimState.pinchStrengthLittle;
    handJointLocations->aimState.pinchStrengthMiddle = handData.aimState.pinchStrengthMiddle;
    handJointLocations->aimState.pinchStrengthRing = handData.aimState.pinchStrengthRing;
}

YVR_EXPORT void YVRGetCurrentInputDevice(XrInputDeviceFlagsYVR* inputDeviceFlagsYVR)
{
    plugin.inputMgr->getCurrentInputDevice(inputDeviceFlagsYVR);
}

YVR_EXPORT bool YVRGetHandEnable()
{
    int result = plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_HAND_ENABLED);
    return result == 1;
}

YVR_EXPORT int YVRGetHandAutoActivateTime() { return plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_HAND_AUTO_ACTIVATE_TIME); }

YVR_EXPORT void YVRSetAPPHandEnable(bool enable) { plugin.handMgr->setHandTrackingEnable(enable); }

YVR_EXPORT void YVRSetAPPControllerEnable(bool enable) { plugin.controllersMgr->setControllerEnable(enable); }

#pragma region "SpatialAnchor"

YVR_EXPORT void YVRCreateSpatialAnchor(XrVector3f position, XrQuaternionf rotation, uint64_t* requestId)
{
    plugin.spatialAnchorMgr->createSpatialAnchorFB(ConvertPoseBetweenOpenXRandUnity(position), ConvertPoseBetweenOpenXRandUnity(rotation), requestId);
}

YVR_EXPORT void YVRSetCreateSpatialAnchorCallback(OnSpatialAnchorCreateComplete callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setSpatialAnchorCreateCompleteCallback(callback);
}

YVR_EXPORT bool YVRGetSpatialAnchorPose(uint64_t space, XrVector3f* position, XrQuaternionf* rotation, XrSpaceLocationFlags* locationFlags)
{
    const bool ret = plugin.spatialAnchorMgr->getSpatialAnchorPose(space, position, rotation, locationFlags);
    *position = ConvertPoseBetweenOpenXRandUnity(*position);
    *rotation = ConvertPoseBetweenOpenXRandUnity(*rotation);
    return ret;
}

YVR_EXPORT void YVRSaveSpatialAnchor(YVRSpaceAnchorSaveInfo spatialAnchorSaveInfo, uint64_t* requestId)
{
    AnnounceCallingFunc();
    plugin.spatialAnchorMgr->saveSpatialAnchor(spatialAnchorSaveInfo, requestId);
}

YVR_EXPORT void YVRQuerySpaces(YVRSpaceQueryInfo queryinfo, uint64_t* requestId)
{
    AnnounceCallingFunc();
    plugin.spatialAnchorMgr->getSpatialAnchor(queryinfo, requestId);
}

YVR_EXPORT void YVRSetQuerySpatialAnchorCallback(OnSpatialAnchorQueryComplete callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setSpatialAnchorQueryCompleteCallback(callback);
}

YVR_EXPORT void YVRDestroySpatialAnchor(uint64_t space, XrSpaceStorageLocationFB storageLocation, uint64_t* requestId)
{
    AnnounceCallingFunc();
    plugin.spatialAnchorMgr->destroyAnchor(space, storageLocation, requestId);
}

YVR_EXPORT void YVRSetEraseSpatialAnchorCallback(OnSetEraseSpatialAnchor callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setEraseSpatialAnchorCallback(callback);
}

YVR_EXPORT void YVRGetEnumerateSpaceSupported(uint64_t space, YVRSpaceSupportedComponent* supportedComponent)
{
    AnnounceCallingFunc();
    plugin.spatialAnchorMgr->enumerateSpaceSupported(space, supportedComponent);
}

YVR_EXPORT bool YVRSetSpaceComponentStatus(uint64_t space, YVRSpaceComponentStatusSetInfo setInfo, uint64_t* requestId)
{
    AnnounceCallingFunc();
    return plugin.spatialAnchorMgr->setSpaceComponentStatus(space, setInfo, requestId);
}

YVR_EXPORT void YVRGetSpaceComponentStatus(uint64_t space, XrSpaceComponentTypeFB componentType, YVRSpaceComponentStatus* status)
{
    plugin.spatialAnchorMgr->getSpaceComponentStatus(space, componentType, status);
}

YVR_EXPORT void YVRCreateSpaceUser(uint64_t userId, XrSpaceUserFB* spaceUser)
{
    AnnounceCallingFunc();
    plugin.spatialAnchorMgr->createSpaceUser(userId, spaceUser);
}

YVR_EXPORT void YVRShareSpace(YVRSpaceShareInfo shareInfo, uint64_t* requestId)
{
    AnnounceCallingFunc();
    plugin.spatialAnchorMgr->shareSpace(shareInfo, requestId);
}

YVR_EXPORT void YVRGetSpaceUserId(XrSpaceUserFB spaceUser, uint64_t* userId)
{
    AnnounceCallingFunc();
    plugin.spatialAnchorMgr->getSpaceUserId(spaceUser, userId);
}

YVR_EXPORT void YVRSetSpaceShareCompleteCallback(OnSpaceShareComplete callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setSpaceShareCompleteCallback(callback);
}

YVR_EXPORT void YVRSetSpaceSaveCompleteCallback(OnSpaceSaveComplete callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setSpaceSaveCompleteCallback(callback);
}

YVR_EXPORT void YVRSetSpaceSetStatusCompleteCallback(OnSpaceSetStatusComplete callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setSpaceSetStatusCompleteCallback(callback);
}

YVR_EXPORT void YVRSaveSpaceList(YVRSpaceListSaveInfo saveInfo, uint64_t* requestId)
{
    AnnounceCallingFunc();
    plugin.spatialAnchorMgr->saveSpatialAnchorList(saveInfo, requestId);
}

YVR_EXPORT void YVRSetSpaceListSaveCompleteCallback(OnSpaceListSaveComplete callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setSpaceListSaveCompleteCallback(callback);
}

YVR_EXPORT void YVRGetSpaceHandleUuid(uint64_t spaceHandle, YVRSpatialAnchorUUID* yvrUUID)
{
    plugin.spatialAnchorMgr->getSpaceUuid(spaceHandle, yvrUUID);
}

#pragma endregion

#pragma region "PlaneDetection"


YVR_EXPORT void YVRCreatePlaneDetection()
{
    AnnounceCallingFunc();
    plugin.planeDetectionMgr->createPlaneDetection();
}

YVR_EXPORT void YVRSetPlaneDetectionsCallback(OnGetPlaneDetections callback)
{
    plugin.planeDetectionMgr->setPlaneDetectionsCallback(callback);
}

YVR_EXPORT YVRPlaneDetectorPolygonBuffers* YVRGetPolygonBuffer(uint64_t planeId, int32_t count)
{
    return plugin.planeDetectionMgr->getPolygonBuffer(planeId, count);
}

YVR_EXPORT void YVREndPlaneDetection()
{
    AnnounceCallingFunc();
    plugin.planeDetectionMgr->destoryPlaneDetection();
}

#pragma endregion

#pragma region "EyeTracking"

YVR_EXPORT bool YVRGetEyeTrackingSupports()
{
    AnnounceCalledFunc();
    return plugin.eyeTrackingMgr->getEyeTrackingSupports();
}

YVR_EXPORT bool YVRGetEyeTrackingEnable()
{
    AnnounceCalledFunc();
    return plugin.eyeTrackingMgr->getEyeTrackingEnable();
}

#pragma endregion

#pragma region "SceneAnchor"

YVR_EXPORT XrResult YVRGetSpaceBoundingBox2D(XrSpace space, XrRect2Df* boundingBox2DOutput)
{
    return plugin.sceneMgr->getSpaceBoundingBox2D(space, boundingBox2DOutput);
}

YVR_EXPORT XrResult YVRGetSpaceBoundingBox3D(XrSpace space, XrRect3DfFB* boundingBox3DOutput)
{
    return plugin.sceneMgr->getSpaceBoundingBox3D(space, boundingBox3DOutput);
}

YVR_EXPORT XrResult YVRGetSpaceBoundary2D(XrSpace space, YVRBoundary2D* boundary2DOutput)
{
    return plugin.sceneMgr->getSpaceBoundary2D(space, boundary2DOutput);
}

YVR_EXPORT XrResult YVRGetSpaceSemanticLabels(XrSpace space, YVRAnchorSemanticLabel* semanticLabelsOutput)
{
    return plugin.sceneMgr->getSpaceSemanticLabels(space, semanticLabelsOutput);
}

YVR_EXPORT XrResult YVRGetSpaceRoomLayout(XrSpace space, YVRRoomLayout* roomLayoutOutput)
{
    return plugin.sceneMgr->getSpaceRoomLayout(space, roomLayoutOutput);
}

YVR_EXPORT XrResult YVRGetSpaceContainer(XrSpace space, YVRSceneAnchorContainer* spaceContainers)
{
    return plugin.sceneMgr->getSpaceContainer(space, spaceContainers);
}

YVR_EXPORT XrResult YVRRequestSceneCapture(YVRSceneCaptureRequest* request, uint64_t* requestId)
{
    return plugin.sceneMgr->requestSceneCapture(request, requestId);
}

YVR_EXPORT void YVRSetRequestSceneCaptureCallback(OnSceneCaptureComplete callback)
{
    plugin.eventsMgr->setRequestSceneCaptureCallback(callback);
}

YVR_EXPORT XrResult YVRGetSpaceTriangleMesh(XrSpace space, SpaceTriangleMesh* spaceTriangleMesh)
{
    return plugin.sceneMgr->getSpaceTriangleMeshMETA(space, spaceTriangleMesh);
}

#pragma endregion

#pragma region "Mesh"
YVR_EXPORT void YVRCreateMeshDetector()
{
    plugin.meshMgr->createMeshDetectorYVR();
}

YVR_EXPORT void YVRDestroyMeshDetector()
{
    plugin.meshMgr->destroyMeshDetectorYVR();
}

YVR_EXPORT void YVRSetMeshBlockUpdateCallback(OnMeshBlockUpdate callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setMeshBlockUpdateCallback(callback);
}

#pragma endregion

#pragma region "ARFoundation"

YVR_EXPORT void YVRSetSessionStateChangeCallback(OnSessionStateChange callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setOnSessionStateChangeCallback(callback);
}

#pragma endregion

#pragma region "Image Tracking"

YVR_EXPORT void YVRSwitchImageTracking(bool enable)
{
    AnnounceCalledFunc();
    plugin.imageTrackingMgr->switchImageTracking(enable);
}

YVR_EXPORT void YVRSetImageTrackingUpdateCallback(OnImageTrackingUpdate callback)
{
    AnnounceCalledFunc();
    plugin.imageTrackingMgr->setImageTrackingUpdateCallback(callback);
}

YVR_EXPORT void YVRRegisterImageTemplate(const ToTrackImageInfo& templateInfo)
{
    AnnounceCallingFunc();
    plugin.imageTrackingMgr->registerImageTemplate(templateInfo);
}

YVR_EXPORT void YVRUnRegisterImageTemplate(const char* imageId)
{
    AnnounceCallingFunc();
    plugin.imageTrackingMgr->unRegisterImageTemplate(imageId);
}
#pragma endregion

#pragma region "PassthroughLUT"

YVR_EXPORT XrResult YVRSetPassthroughStyle(PassthroughStyle passthroughStyle)
{
   return plugin.passThroughProviderMgr->passthroughLayerSetStyle(passthroughStyle);
}

YVR_EXPORT XrResult YVRCreatePassthroughColorLut(XrPassthroughColorLutChannelsMETA channels, uint32_t resolution, XrPassthroughColorLutDataMETA data,
                                             XrPassthroughColorLutMETA* colorLut)
{
   return plugin.passThroughProviderMgr->createPassthroughColorLut(channels,resolution,data,colorLut);
}

YVR_EXPORT XrResult  YVRDestroyPassthroughColorLut(XrPassthroughColorLutMETA colorLut)
{
    return plugin.passThroughProviderMgr->destroyPassthroughColorLut(colorLut);
}

YVR_EXPORT XrResult  YVRUpdatePassthroughColorLut(XrPassthroughColorLutMETA lutHandle, XrPassthroughColorLutDataMETA data)
{
    return plugin.passThroughProviderMgr->updatePassthroughColorLut(lutHandle, data);
}
#pragma endregion

YVR_EXPORT void YVRSetColorSpace(XrColorSpaceFB colorSpace)
{
    YInfo("YVRSetColorSpace %d:", colorSpace);
    OpenXRAPI(xrSetColorSpaceFB(plugin.openxrMgr->program->session,colorSpace));
}
}
