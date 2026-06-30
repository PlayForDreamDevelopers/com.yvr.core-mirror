#define YVR_EXPORT

#include <unistd.h>

#include "./YVRUnityXR/YVRDisplayProvider.h"
#include "YVRUnityPlugin.h"
#include "Utilities/YVRCoordinateConverter.h"
#include "YVRUnityXR/YVRInputProvider.h"
#include "YVRModuleMgrs/Render/YVRPassthroughLayer.h"

#include "YVRModuleMgrs/Render/RenderTasks/SetFoveationDataEventTask.h"
#include "YVRModuleMgrs/Render/YVRDPInfo.h"
#include "YVRModuleMgrs/YVRPassthroughMeshMgr.h"

extern "C"
{
#pragma region "Controller status"
YVR_EXPORT bool YVRGetControllerPositionPredicting(xrControllerSide mask) { return false; }

YVR_EXPORT void YVRGetControllerType(XrControllerYVR mask, XrControllerTypeYVR* type)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrQueryCurrentConnectedControllerTypeYVR(plugin.openxrMgr->program->instance, mask, type));
}

#pragma endregion "Controller status"

#pragma region "Controller Binding"
YVR_EXPORT void YVRStartControllerPairing(XrControllerYVR mask)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrStartControllerPairingYVR(plugin.openxrMgr->program->instance, mask));
}

YVR_EXPORT void YVRStopControllerParing(XrControllerYVR mask)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrStopControllerPairingYVR(plugin.openxrMgr->program->instance, mask));
}

YVR_EXPORT void* YVRGetControllerSerialNumber(XrControllerYVR mask)
{
    AnnounceCallingFunc();
    char* serialNumber = new char[20];
    OpenXRAPI(xrGetControllerSerialNumberYVR(plugin.openxrMgr->program->instance, mask, serialNumber));
    YInfo("Get SerialNumber is %s", serialNumber);
    return (void*)serialNumber;
}

YVR_EXPORT void* YVRGetControllerMCUVersion(XrControllerYVR mask)
{
    AnnounceCallingFunc();
    char* mcuVersion = new char[128];
    const xrGetSystemPropertyYVR property =
        mask == XR_CONTROLLER_LEFT_YVR ? YVR_SYS_PROP_GET_LEFT_CONTROLLER_MCU_VERSION : YVR_SYS_PROP_GET_RIGHT_CONTROLLER_MCU_VERSION;

    plugin.openxrMgr->extMgr->getCharArrayProperty(property, mcuVersion, 128);
    YInfo("Get Controller Mcu Version is %s", mcuVersion);
    return (void*)mcuVersion;
}

YVR_EXPORT void* YVRGetHMDMCUVersion()
{
    AnnounceCallingFunc();
    char* mcuVersion = new char[128];
    constexpr xrGetSystemPropertyYVR property = YVR_SYS_PROP_GET_HMD_MCU_VERSION;
    plugin.openxrMgr->extMgr->getCharArrayProperty(property, mcuVersion, 128);
    YInfo("Get HMD Mcu Version is %s", mcuVersion);
    return (void*)mcuVersion;
}

YVR_EXPORT int YVRQueryControllerParingState(XrControllerYVR mask)
{
    AnnounceCallingFunc();
    XrControllerPairingStateYVR state{};
    OpenXRAPI(xrQueryControllerPairingStateYVR(plugin.openxrMgr->program->instance, mask, &state));
    return (int)state;
}

YVR_EXPORT void* YVRGetRuntimeVersion()
{
    AnnounceCallingFunc();

    char* version = new char[128];

    XrInstanceProperties instanceProperties{XR_TYPE_INSTANCE_PROPERTIES};
    OpenXRAPI(xrGetInstanceProperties(plugin.openxrMgr->program->instance, &instanceProperties));
    const string versionStr = GetXrVersionString(instanceProperties.runtimeVersion);
    memcpy(version, versionStr.c_str(), versionStr.length());

    YInfo("Instance RuntimeName=%s RuntimeVersion =%s", instanceProperties.runtimeName, version);
    return (void*)version;
}

YVR_EXPORT void ReleasePointerMemory(const char* memAddr)
{
    if (nullptr != memAddr)
    {
        delete[] memAddr;
        memAddr = nullptr;
    }
}
#pragma endregion "Controller Binding"

#pragma region "Boundary Related"

YVR_EXPORT void YVRGetRecenterOffset(XrPosef* pose)
{
    plugin.hmdMgr->getRecenterOffset(pose);
    *pose = ConvertPoseBetweenOpenXRandUnity(*pose);
}

YVR_EXPORT void YVRGetRecenterPose(XrPosef* pose)
{
    plugin.hmdMgr->getRecenterPose(pose);
    *pose = ConvertPoseBetweenOpenXRandUnity(*pose);
}

YVR_EXPORT void YVRGetStageSpacePose(XrPosef* pose)
{
    plugin.hmdMgr->getStageSpacePose(pose);
    *pose = ConvertPoseBetweenOpenXRandUnity(*pose);
}

YVR_EXPORT void YVRSetBoundaryTypeVisible(XrBoundaryTypeYVR boundaryType, bool quiting)
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData{ACTION_SET_BOUNDARY_CONFIRM};
    const uint8_t type = boundaryType;
    const uint8_t quit = quiting ? 1 : 0;
    const uint8_t data[2]{type, quit};
    controlData.dataLengthBytes = static_cast<int>(2);
    controlData.controlData = const_cast<uint8_t*>(data);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVRSetBoundaryData(int dataLength, float in[512])
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData{ACTION_SET_BOUNDARY_POINTS};
    controlData.dataLengthBytes = static_cast<int>(dataLength * sizeof(float));
    controlData.controlData = reinterpret_cast<uint8_t*>(in);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVRSetGroundDistance(float distance)
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData{ACTION_SET_GROUND_DISTANCE};
    controlData.dataLengthBytes = static_cast<int>(2 * sizeof(float));
    float data[2]{distance, 1};
    controlData.controlData = reinterpret_cast<uint8_t*>(data);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT float YVRGetBoundaryDistance()
{
    AnnounceCallingFunc();
    XrBoundaryStatus status{ACTION_GET_GROUND_DISTANCE};
    OpenXRAPI(xrGetBoundaryStatusYVR(plugin.openxrMgr->program->session, &status));
    float distance = *(float*)(status.statusData);
    return distance;
}

YVR_EXPORT void YVRSetPassthroughMode(bool on)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setPassThroughMode(on);
}

YVR_EXPORT void YVRSetPassthroughVisible(bool visible)
{
    AnnounceCallingFunc();
    if (visible)
    {
        plugin.passThroughProviderMgr->passthroughStartFB();
    }
    else
    {
        plugin.passThroughProviderMgr->passthroughStopFB();
    }

    plugin.renderMgr->configsMgr->setPassThroughMode(visible);
}

YVR_EXPORT void YVRSetPassthroughProviderEnable(bool enable)
{
    AnnounceCallingFunc();
    if (enable)
    {
        plugin.passThroughProviderMgr->startPassthroughProvider();
    }
    else
    {
        plugin.passThroughProviderMgr->stopPassthroughProvider();
    }
}

YVR_EXPORT void YVRForceBoundaryNoneVisible()
{
    AnnounceCallingFunc();
    int xrBoundaryVisible = XrBoundaryVisibleControlYVR::BOUNDARY_FORCE_NON_VISIBLE_YVR;
    XrBoundaryControlData controlData{ACTION_SET_BOUNDARY_VISIBLE};
    controlData.dataLengthBytes = sizeof(int32_t);
    controlData.controlData = reinterpret_cast<uint8_t*>(&xrBoundaryVisible);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVRClearBoundaryData()
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData{ACTION_SET_CLEAR_BOUNDARY_DATA};
    controlData.dataLengthBytes = 0;
    controlData.controlData = nullptr;
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT int YVRGetBoundaryType()
{
    AnnounceCallingFunc();
    XrBoundaryStatus status{ACTION_GET_BOUNDARY_TYPE};
    OpenXRAPI(xrGetBoundaryStatusYVR(plugin.openxrMgr->program->session, &status));
    return static_cast<int32_t>(*status.statusData);
}

YVR_EXPORT void YVRSetBoundaryType(XrBoundaryTypeYVR boundaryType)
{
    AnnounceCallingFunc();
    AnnounceCallingFunc();
    XrBoundaryControlData controlData;
    controlData.action = ACTION_SET_BOUNDARY_TYPE;
    controlData.dataLengthBytes = sizeof(int32_t);
    controlData.controlData = reinterpret_cast<uint8_t*>(&boundaryType);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVRShowBoundaryData(int dataLength, float in[521])
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData;
    controlData.action = ACTION_SET_BOUNDARY_PREVIEW;
    controlData.dataLengthBytes = static_cast<int>(dataLength * sizeof(float));
    controlData.controlData = reinterpret_cast<uint8_t*>(in);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVROnGuardianFinish()
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData;
    controlData.action = ACTION_SET_GUARDIAN_APP_LEAVE;
    controlData.dataLengthBytes = 0;
    controlData.controlData = nullptr;
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVROnGuardianEnter()
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData;
    controlData.action = ACTION_SET_GUARDIAN_APP_ENTER;
    controlData.dataLengthBytes = 0;
    controlData.controlData = nullptr;
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVRSetBoundaryShowStyle(XrBoundaryShowStyleYVR showStyle)
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData{ACTION_SET_BOUNDARY_SHOW_STYLE};
    controlData.dataLengthBytes = sizeof(int32_t);
    controlData.controlData = reinterpret_cast<uint8_t*>(&showStyle);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT void YVRSetBoundaryRadiusType(XrLocalBoundaryRadiusTypeYVR radiusType)
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData{ACTION_SET_LOCAL_BOUNDARY_RADIUS};
    controlData.dataLengthBytes = sizeof(int32_t);
    controlData.controlData = reinterpret_cast<uint8_t*>(&radiusType);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT int YVRGetBoundaryRadiusType()
{
    AnnounceCallingFunc();
    XrBoundaryStatus status{ACTION_GET_LOCAL_BOUNDARY_RADIUS};
    OpenXRAPI(xrGetBoundaryStatusYVR(plugin.openxrMgr->program->session, &status));
    return static_cast<int32_t>(*status.statusData);
}


YVR_EXPORT void YVRSwitchGroundDistanceAutoDetecting(bool on)
{
    AnnounceCalledFunc();
    const xrAutoGroundDistance type = on ? YVR_AUTO_GROUND_DISTANCE_ENABLE : YVR_AUTO_GROUND_DISTANCE_DISABLE;
    xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_AUTO_GROUND_DISTANCE, type);
}

YVR_EXPORT void YVRSetBoundaryEffectDistance(float distance)
{
    AnnounceCalledFunc();
    XrBoundaryControlData controlData{ACTION_SET_EFFECT_DISTANCE};
    controlData.dataLengthBytes = sizeof(float);
    controlData.controlData = reinterpret_cast<uint8_t*>(&distance);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT float YVRGetBoundaryEffectDistance()
{
    AnnounceCallingFunc();
    XrBoundaryStatus state{ACTION_GET_EFFECT_DISTANCE};
    OpenXRAPI(xrGetBoundaryStatusYVR(plugin.openxrMgr->program->session, &state));
    float value = *(float*)(state.statusData);
    return value;
}

YVR_EXPORT void YVRSetBoundaryOutEffectType(XrBoundaryOutEffectTypeYVR effectType)
{
    AnnounceCalledFunc();
    XrBoundaryControlData controlData{ACTION_SET_OUT_BOUNDARY_EFFECT};
    controlData.dataLengthBytes = sizeof(int32_t);
    controlData.controlData = reinterpret_cast<uint8_t*>(&effectType);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT int32_t YVRGetBoundaryOutEffectType()
{
    AnnounceCallingFunc();
    XrBoundaryStatus status{ACTION_GET_OUT_BOUNDARY_EFFECT};
    OpenXRAPI(xrGetBoundaryStatusYVR(plugin.openxrMgr->program->session, &status));
    return static_cast<int32_t>(*status.statusData);
}

#pragma endregion "Boundary Related"

#pragma region "Misc"
YVR_EXPORT int YVRGetCurrentThreadID() { return static_cast<int>(gettid()); }

YVR_EXPORT void YVRSetExtraLatencyMode(bool enable)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setExtraLatency(enable);
}

#pragma endregion "Misc"

#pragma region "Log"
void setDebugOutputHandler(OutputLogViaUnityHandler debugHandler) { YVRLog::GetInstance()->SetDebugOutputHandler(debugHandler); }
void setInfoOutputHandler(OutputLogViaUnityHandler infoHandler) { YVRLog::GetInstance()->SetInfoOutputHandler(infoHandler); }
void setWarnOutputHandler(OutputLogViaUnityHandler warnHandler) { YVRLog::GetInstance()->SetWarnOutputHandler(warnHandler); }
void setErrorOutputHandler(OutputLogViaUnityHandler errorHandler) { YVRLog::GetInstance()->SetErrorOutputHandler(errorHandler); }

#pragma endregion "Log"

#pragma region "Rendering Frame && Layers"

YVR_EXPORT int YVRGetEyeBufferLayerID() { return plugin.renderMgr->renderLayersMgr->getEyeBufferLayer()->id; }

YVR_EXPORT void YVRGetEyeBias(int side, XrVector3f* position, XrQuaternionf* rotation)
{
    YInfo("YVRGetEyeBias: {%d}", side);
    *position = ConvertPoseBetweenOpenXRandUnity(plugin.hmdMgr->renderViews[side].pose.position);
    *rotation = ConvertPoseBetweenOpenXRandUnity(plugin.hmdMgr->renderViews[side].pose.orientation);
}

YVR_EXPORT void YVRLockLayerFlags(int layerID, int layerFlags)
{
    AnnounceCallingFunc();
    plugin.renderMgr->renderLayersMgr->lockLayerFlags(layerID, layerFlags);
}

YVR_EXPORT void YVRUnLockLayerFlags(int layerID)
{
    AnnounceCallingFunc();
    plugin.renderMgr->renderLayersMgr->unLockLayerFlags(layerID);
}

YVR_EXPORT void YVRSetLayerFlags(int layerID, int layerFlags)
{
    AnnounceCallingFunc();
    plugin.renderMgr->renderLayersMgr->setLayerFlags(layerID, layerFlags);
}

YVR_EXPORT void YVRUnsetLayerFlags(int layerID, int layerFlags)
{
    AnnounceCallingFunc();
    plugin.renderMgr->renderLayersMgr->unsetLayerFlags(layerID, layerFlags);
}

YVR_EXPORT void YVRSetSystemMenuMode(bool enable)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setSystemMenuMode(enable);
}

YVR_EXPORT void YVRSetSkipFrameCount(int frameCount)
{
    AnnounceCallingFunc();
    YInfo("YVRSetSkipFrameCount: %d", frameCount);
    plugin.renderMgr->setSkipFrameCount(frameCount);
}

YVR_EXPORT void YVRSwitchHevFilter(bool on)
{
    AnnounceCallingFunc()
    const int value = on ? 1 : 0;
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_HEVFILTER_ENABLE, value));
}

YVR_EXPORT int YVRGetHevFilter()
{
    AnnounceCallingFunc();
    return plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_HEVFILTER_ENABLE);
}

YVR_EXPORT bool YVRSetCameraFrequency(int freq)
{
    AnnounceCallingFunc();
    const XrResult result = xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_CAM_FREQ, freq);
    return result == XrResult::XR_SUCCESS;
}

YVR_EXPORT int YVRGetCameraFrequency()
{
    AnnounceCallingFunc()
    int ret;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_CAM_TRACKING_FREQ, &ret));
    return ret;
}

YVR_EXPORT bool YVRGetRecommendedResolution(XrExtent2Di* outRecommendedResolution)
{
    AnnounceCalledFunc();
    const XrResult result = xrGetRecommendedResolution(plugin.openxrMgr->program->session,
                                                       plugin.renderMgr->configsMgr->GetAdapterResolutionInfo(), outRecommendedResolution);
    return result == XrResult::XR_SUCCESS;
}

YVR_EXPORT void YVRSetAdapterResolutionPolicy(XrAdapterResolutionPolicyYVR adapterResolutionPolicy)
{
    AnnounceCalledFunc();
    plugin.renderMgr->configsMgr->setAdapterResolutionPolicy(adapterResolutionPolicy);
}

YVR_EXPORT int YVRCreatePassthroughLayer()
{
    AnnounceCalledFunc();
    XrPassthroughLayerCreateInfoFB plci = {XR_TYPE_PASSTHROUGH_LAYER_CREATE_INFO_FB};
    plci.passthrough = plugin.passThroughProviderMgr->passthrough;
    plci.purpose = XR_PASSTHROUGH_LAYER_PURPOSE_RECONSTRUCTION_FB;
    XrPassthroughLayerFB passthroughLayer;
    OpenXRAPI(xrCreatePassthroughLayerFB(plugin.openxrMgr->program->session, &plci, &passthroughLayer));
    int layerID = plugin.renderMgr->renderLayersMgr->addPassthroughLayer(passthroughLayer);
    OpenXRAPI(xrPassthroughLayerResumeFB(passthroughLayer));
    return layerID;
}

YVR_EXPORT void YVRDestroyPassthroughLayer(int passthroughLayerId)
{
    AnnounceCalledFunc();
    YVRRenderLayer* layer = plugin.renderMgr->renderLayersMgr->getRenderLayer(passthroughLayerId);
    if (layer == nullptr) return;
    if (layer->getXrLayerHeader() == nullptr || layer->getXrLayerHeader()->type != XR_TYPE_COMPOSITION_LAYER_PASSTHROUGH_FB)
    {
        YError("Layer %d is not a YVRPassthroughLayer.", passthroughLayerId);
        return;
    }

    YVRPassthroughLayer* passthroughLayer = static_cast<YVRPassthroughLayer*>(layer);
    const XrPassthroughLayerFB layerHandle = passthroughLayer->passthroughLayer;
    if (layerHandle != XR_NULL_HANDLE)
    {
        OpenXRAPI(xrPassthroughLayerPauseFB(layerHandle));
        OpenXRAPI(xrDestroyPassthroughLayerFB(layerHandle));
        passthroughLayer->passthroughLayer = XR_NULL_HANDLE;
    }

    plugin.renderMgr->renderLayersMgr->deletePassthroughLayer(passthroughLayerId);
}

#pragma endregion "Rendering Frame && Layers"

#pragma region "Rendering task"
YVR_EXPORT void YVRSetFoveation(XrFoveationLevelFB ffrLevel, XrFoveationDynamicFB dynamic)
{
    plugin.gfxTasksMgr->PushOnPreSubmitTask(new SetFoveationDataEventTask(ffrLevel, dynamic));
}
#pragma endregion "Rendering task"

#pragma region "HandTracking"
YVR_EXPORT void YVRSetHandEnable(bool enable)
{
    AnnounceCallingFunc();
    const int value = enable ? 1 : 0;
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_HAND_ENABLED, value));
}

YVR_EXPORT void YVRSetHandAutoChangeTime(int ms)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_HAND_AUTO_ACTIVATE_TIME, ms));
}

#pragma endregion

#pragma region "EyeTracking"
YVR_EXPORT void YVRSetEyeTrackingEnable(bool enable)
{
    AnnounceCallingFunc();
    const int mode = enable ? 1 : 0;
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_EYE_TRACKING_ENABLED, &propData));
    propData = (propData & ~0x0F) | (mode & 0x0F);
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_EYE_TRACKING_ENABLED, propData));
}

#pragma endregion


YVR_EXPORT void YVRSetForceHeadMode(bool enable)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetForceHeadModeYVR(plugin.openxrMgr->program->session, enable));
}

#pragma region "SpaceDetect"

YVR_EXPORT void YVRSetSpaceDetectEnable(bool enable)
{
    AnnounceCallingFunc();
    const int value = enable ? 1 : 0;
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_SPACE_DETECT_ENABLED, value));
}

YVR_EXPORT bool YVRGetSpaceDetectEnable()
{
    AnnounceCallingFunc();
    int result = plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_SPACE_DETECT_ENABLE_STATE);
    return result == 1;
}

YVR_EXPORT void YVRSetSpaceDetectRange(float value)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_SPACE_DETECT_RANGE, value));
}

YVR_EXPORT float YVRGetSpaceDetectRange()
{
    AnnounceCallingFunc();
    return plugin.openxrMgr->extMgr->getFloatProperty(YVR_SYS_PROP_GET_SPACE_DETECT_RANGE);
}

YVR_EXPORT void YVRRSetSpaceDetectRangeChangeCallback(OnSpaceDetectRangeChange callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setSpaceDetectRangeChangeCallback(callback);
}

#pragma endregion "SpaceDetect"

#pragma region "IPD"

YVR_EXPORT float YVRGetIPD()
{
    AnnounceCallingFunc();
    return plugin.openxrMgr->extMgr->getFloatProperty(YVR_SYS_PROP_GET_IPD);
}

YVR_EXPORT void YVRSetIPD(float value)
{
    AnnounceCallingFunc();
    plugin.openxrMgr->extMgr->setFloatProperty(YVR_SYS_PROP_SET_IPD, value);
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRRefreshSystemProperty()
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->refreshSystemProperty();
}

#pragma endregion "IPD"

#pragma region "SceneAnchor"

YVR_EXPORT void YVRCreateGlobalAnchor(XrVector3f position, XrQuaternionf rotation, uint64_t* createRequest)
{
    plugin.sceneMgr->createGlobalAnchor(ConvertPoseBetweenOpenXRandUnity(position), ConvertPoseBetweenOpenXRandUnity(rotation), createRequest);
}

YVR_EXPORT void YVRSetSceneAnchorCallback(OnGlobalAnchorCreateComplete callback)
{
    return plugin.eventsMgr->setGlobalAnchorCreateCompleteCallback(callback);
}

YVR_EXPORT XrResult YVRSetSpaceBoundingBox2D(XrSpace space, XrRect2Df boundingBox2D)
{
    return plugin.sceneMgr->setSpaceBoundingBox2D(space, &boundingBox2D);
}

YVR_EXPORT XrResult YVRSetSpaceBoundingBox3D(XrSpace space, XrRect3DfFB boundingBox3D)
{
    return plugin.sceneMgr->setSpaceBoundingBox3D(space, &boundingBox3D);
}

YVR_EXPORT XrResult YVRSetSpaceSemanticLabels(XrSpace space, YVRAnchorSemanticLabel semanticLabels)
{
    return plugin.sceneMgr->setSpaceSemanticLabels(space, &semanticLabels);
}

YVR_EXPORT XrResult YVRSetSpaceBoundary2D(XrSpace space, YVRBoundary2D boundary2D)
{
    return plugin.sceneMgr->setSpaceBoundary2D(space, &boundary2D);
}

YVR_EXPORT XrResult YVRSetSpaceRoomLayout(XrSpace space, YVRRoomLayout roomLayout)
{
    return plugin.sceneMgr->setSpaceRoomLayout(space, &roomLayout);
}

YVR_EXPORT XrResult YVRSetSpaceContainer(XrSpace space, YVRSceneAnchorContainer spaceContainer)
{
    return plugin.sceneMgr->setSpaceContainer(space, &spaceContainer);
}

YVR_EXPORT XrResult YVRGetSceneStatus(uint32_t* status) { return plugin.sceneMgr->getSceneStatus(status); }

YVR_EXPORT XrResult YVRSetLocateRoom(XrLocateRoomInfoYVR* roomInfo) { return plugin.sceneMgr->setLocateRoom(roomInfo); }

YVR_EXPORT XrResult YVRGetLocateRoom(XrLocateRoomInfoYVR* roomInfo) { return plugin.sceneMgr->getLocateRoom(roomInfo); }

YVR_EXPORT XrResult YVRResetScene() { return plugin.sceneMgr->resetScene(); }

YVR_EXPORT bool YVRSetSpaceTriangleMesh(XrSpace space, SpaceTriangleMesh* spaceTriangleMesh)
{
    return plugin.sceneMgr->setSpaceTriangleMeshYVR(space, spaceTriangleMesh);
}

YVR_EXPORT void YVRSetRoomSetupUpdateCallback(OnRoomSetupUpdate callback) { return plugin.eventsMgr->setRoomSetupUpdateCallback(callback); }

YVR_EXPORT void YVRSetRoomSetupCompleteCallback(OnRoomSetupComplete callback)
{
    AnnounceCallingFunc()
    return plugin.eventsMgr->setRoomSetupCompleteCallback(callback);
}

YVR_EXPORT void YVRBeginRoomSetup(RoomSetupBeginInfo roomSetupBeginInfo) { plugin.meshMgr->beginRoomSetupYVR(roomSetupBeginInfo); }

YVR_EXPORT void YVREndRoomSetup() { plugin.meshMgr->endRoomSetupYVR(); }

#pragma endregion "SceneAnchor"

#pragma region "Passthrough Provider"

YVR_EXPORT int YVRGetPassthroughSwapchainImageIndex() { return plugin.passThroughProviderMgr->getSwapchainImageIndex(); }

YVR_EXPORT XrQuaternionf YVRGetPassthroughImageDiffRotation(int eyeIndex)
{
    // As the quaternion is used for opengl library, thus here we keep it in openxr coordinate
    return plugin.passThroughProviderMgr->getPassthroughImageDiffRotation(eyeIndex);
}

YVR_EXPORT bool YVRGetPassthroughSwapchainImageValid() { return plugin.passThroughProviderMgr->isImageValid(); }

YVR_EXPORT void YVRCreateLargePassthroughStream()
{
    plugin.passThroughProviderMgr->createLargePassthroughStream();
}

YVR_EXPORT void YVRDestroyLargePassthroughStream()
{
    plugin.passThroughProviderMgr->destroyLargePassthroughStream();
}

YVR_EXPORT int YVRGetLargePassthroughSwapchainImageIndex()
{
    return plugin.passThroughProviderMgr->getLargeSwapchainImageIndex();
}

YVR_EXPORT bool YVRGetLargePassthroughSwapchainImageValid()
{
    return plugin.passThroughProviderMgr->isLargeImageValid();
}

#pragma endregion

#pragma region "Mesh"

YVR_EXPORT void YVRSetBoundaryUpdateCallback(OnBoundaryUpdate callback) { plugin.eventsMgr->setBoundaryUpdateCallback(callback); }

YVR_EXPORT void YVRBeginBoundaryDetect(float radius) { plugin.meshMgr->beginBoundaryDetect(radius); }

YVR_EXPORT void YVREndBoundaryDetect(float radius) { plugin.meshMgr->endBoundaryDetect(); }

#pragma endregion

#pragma region "LargeSpatial"

YVR_EXPORT void YVRSetMarkerTrackingUpdateCallback(OnMarkerTrackingUpdateYVR callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setMarkerTrackingUpdateYVRCallback(callback);
}

YVR_EXPORT void YVRSetLargeSpaceResultCallback(OnLargeSpaceResultYVR callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setLargeSpaceResultYVRCallback(callback);
}

YVR_EXPORT void YVRSetLargeSpaceMapRecordEnable(bool enable)
{
    AnnounceCallingFunc();
    plugin.largeSpatialMgr->setLargeSpaceMapRecordEnableYVR(enable);
}

YVR_EXPORT void YVRSetMarkerDetectionEnable(bool enable)
{
    AnnounceCallingFunc();
    plugin.largeSpatialMgr->setMarkerDetectionEnableYVR(enable);
}

YVR_EXPORT void YVRClearAllMarkers()
{
    AnnounceCallingFunc();
    plugin.largeSpatialMgr->clearAllMarkersYVR();
}

YVR_EXPORT void YVRSetLargeSpaceOriginPose(const XrPosef& inPose)
{
    AnnounceCallingFunc();
    plugin.largeSpatialMgr->setLargeSpaceOriginPoseYVR(ConvertPoseBetweenOpenXRandUnity(inPose));
}

YVR_EXPORT void YVRExportLargeSpaceMap()
{
    AnnounceCallingFunc();
    plugin.largeSpatialMgr->exportLargeSpaceMapYVR();
}

YVR_EXPORT void YVRSetRecenterEnable(XrBool32 enable)
{
    AnnounceCallingFunc();
    plugin.largeSpatialMgr->setRecenterEnableYVR(enable);
}

YVR_EXPORT bool YVRGetRecenterEnable()
{
    AnnounceCallingFunc();
    return plugin.largeSpatialMgr->getRecenterEnableYVR();
}

YVR_EXPORT void YVRSetOnLargeSpaceMapAUpdateCallback(OnLargeSpaceMapAUpdate callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setOnLargeSpaceMapAUpdateCallback(callback);
}

YVR_EXPORT void YVRSetLargeSpaceMapAssemblingEnable(XrBool32 enable)
{
    AnnounceCallingFunc();
    plugin.largeSpatialMgr->setLargeSpaceMapAssemblingEnable(enable);
}

YVR_EXPORT void YVRGetLargeSpacePointsCalibParams(XrPosef* outTransform, float* outGroundDistance)
{
    AnnounceCallingFunc();
    plugin.largeSpatialMgr->getLargeSpacePointsCalibParams(outTransform, outGroundDistance);
    *outTransform = ConvertPoseBetweenOpenXRandUnity(*outTransform);
}

#pragma endregion

YVR_EXPORT void YVRSetBlockInteractionData(XrBool32 isBlock)
{
    AnnounceCallingFunc();
    plugin.inputMgr->isBlockingInteractionData = isBlock;
}

YVR_EXPORT bool YVRGetBlockInteractionData()
{
    return plugin.inputMgr->isBlockingInteractionData;
}

YVR_EXPORT void YVRSetEyeTrackingFilterMode(bool enable)
{
    AnnounceCallingFunc();
    const int mode = enable ? 1 : 0;
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_EYE_TRACKING_ENABLED, &propData));
    propData = ((propData & ~0x30) | ((mode << 4) & 0x30));
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_EYE_TRACKING_ENABLED, propData));
}

YVR_EXPORT bool YVRGetEyeTrackingFilterMode()
{
    AnnounceCallingFunc();
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_EYE_TRACKING_ENABLED, &propData));
    propData = ((propData >> 4) & 0x03);
    return propData == 1;
}

YVR_EXPORT void YVRSetEyeTrackingInputMode(bool enable)
{
    AnnounceCallingFunc();
    const int mode = enable ? 1 : 0;
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_EYE_TRACKING_ENABLED, &propData));
    propData = ((propData & ~0x40) | ((mode << 6) & 0x40));
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_EYE_TRACKING_ENABLED, propData));
}

YVR_EXPORT bool YVRGetEyeTrackingInputMode()
{
    AnnounceCallingFunc();
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_EYE_TRACKING_ENABLED, &propData));
    propData = ((propData >> 6) & 0x01);
    return propData == 1;
}

YVR_EXPORT void YVRSetWearingDetecionEnable(bool enable)
{
    AnnounceCallingFunc();
    const int mode = enable ? 1 : 0;
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_WEARING_DETECTION_SETTING, &propData));
    propData = ((propData & ~0x01) | (mode & 0x01));
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_WEARING_DETECTION_SETTING, propData));
}

YVR_EXPORT bool YVRGetWearingDetecionEnable()
{
    AnnounceCallingFunc();
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_WEARING_DETECTION_SETTING, &propData));
    propData = (propData & 0x01);
    return propData == 1;
}

YVR_EXPORT void YVRSetIPDEnable(bool enable)
{
    AnnounceCallingFunc();
    const int mode = enable ? 1 : 0;
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_WEARING_DETECTION_SETTING, &propData));
    propData = ((propData & ~0x02) | ((mode << 1) & 0x02));
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_WEARING_DETECTION_SETTING, propData));
}

YVR_EXPORT bool YVRGetIPDEnable()
{
    AnnounceCallingFunc();
    int propData;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_WEARING_DETECTION_SETTING, &propData));
    propData = ((propData >> 1) & 0x01);
    return propData == 1;
}

YVR_EXPORT float YVRGetIPDValue()
{
    AnnounceCallingFunc();
    float value;
    OpenXRAPI(xrGetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_IPD_CONFIGURATION, &value));
    return value;
}

YVR_EXPORT void YVRSetIPDValue(float value)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_IPD_CONFIGURATION, value));
    OpenXRAPI(xrSetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_IPD, value));
}

YVR_EXPORT void YVRSetOriginalBoundaryRadius(float value)
{
    AnnounceCallingFunc();
    XrBoundaryControlData controlData{ACTION_SET_ORIGINAL_BOUNDARY_RADIUS};
    controlData.dataLengthBytes = sizeof(float);
    controlData.controlData = reinterpret_cast<uint8_t*>(&value);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT float YVRGetOriginalBoundaryRadius()
{
    AnnounceCallingFunc();
    XrBoundaryStatus state{ACTION_GET_ORIGINAL_BOUNDARY_RADIUS};
    OpenXRAPI(xrGetBoundaryStatusYVR(plugin.openxrMgr->program->session, &state));
    float value = *(float*)(state.statusData);
    return value;
}

YVR_EXPORT void YVRSetAutoGroundDistanceEnable(bool value)
{
    AnnounceCallingFunc();
    int enable = value ? 1 : 0;
    XrBoundaryControlData controlData{ACTION_SET_AUTO_GROUND_DISTANCE_ENABLE};
    controlData.dataLengthBytes = sizeof(int);
    controlData.controlData = reinterpret_cast<uint8_t*>(&enable);
    OpenXRAPI(xrSetBoundaryControlYVR(plugin.openxrMgr->program->session, &controlData));
}

YVR_EXPORT bool YVRGetAutoGroundDistanceEnable()
{
    AnnounceCallingFunc();
    XrBoundaryStatus state{ACTION_GET_AUTO_GROUND_DISTANCE_ENABLE};
    OpenXRAPI(xrGetBoundaryStatusYVR(plugin.openxrMgr->program->session, &state));
    float value = *(int*)(state.statusData);
    return value == 1;
}

YVR_EXPORT void YVRSetColorChromaState(bool enable)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_COLOR_CHROMA_STATE,enable?1:0));
}

YVR_EXPORT bool YVRGetColorChromaState()
{
    AnnounceCallingFunc();
    int ret;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_COLOR_CHROMA_STATE, &ret));
    return ret == 1;
}

# pragma region "Hand Calibration"

YVR_EXPORT void YVRStartHandCalibration()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrStartHandCalibrationYVR(plugin.openxrMgr->program->session));
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRRegisterHandCalibrationEvent(OnHandCalibrationResultYVR callback)
{
    AnnounceCallingFunc();
    plugin.handCalibrationMgr->registerHandCalibrationEvent(callback);
}

YVR_EXPORT void YVRUnregisterHandCalibrationEvent()
{
    AnnounceCalledFunc();
    plugin.handCalibrationMgr->unregisterHandCalibrationEvent();
}

YVR_EXPORT void YVRStopHandCalibration()
{
    OpenXRAPI(xrStopHandCalibrationYVR(plugin.openxrMgr->program->session));
}

# pragma endregion

#pragma region "SlamTrackingCalibration"

YVR_EXPORT void YVRSetSlamCalibrationCallback(OnSlamCalibration action)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setOnSlamCalibrationCallback(action);
}

YVR_EXPORT void YVRSetSlamCalibrationEnableYVR(bool enable)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSlamCalibrationEnableYVR(plugin.openxrMgr->program->session,enable));
}

#pragma endregion

#pragma region "Rendering"

YVR_EXPORT void YVRDestroyUnityTexture()
{
    AnnounceCallingFunc();
    plugin.unityXRProviderMgr->displayProvider->DestroyUnityTexture();
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRSetFullRenderScale(float scale)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setResolutionScale(scale);
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRSetInnerRenderScale(float scale)
{

    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setInnerPassResolutionScale(scale);
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRSetOuterRenderScale(float scale)
{

    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setOuterPassResolutionScale(scale);
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRSetInnerExtraRenderScale(float scale)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setInnerExtraPassResolutionScale(scale);
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRSetOuterExtraRenderScale(float scale)
{

    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setOuterExtraPassResolutionScale(scale);
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRNotifyVirtualEnvironmentEnable(bool enable)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_VIRTUAL_SCENE, enable ?1.0f :0.0f));
    AnnounceCalledFunc();
}

YVR_EXPORT void YVRGetRenderScale(const char* packageName, YVRRenderScaleBuffers* renderScaleBuffers)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->getRenderScale(packageName, renderScaleBuffers);
}

YVR_EXPORT void YVRSetRenderScale(const char* packageName, XrRenderScaleYVR renderScale)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setRenderScale(packageName, renderScale);
}

YVR_EXPORT void YVRSetRenderSharpen(const char* packageName, XrSharpenInfoYVR sharpenInfo)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setRenderSharpen(packageName, &sharpenInfo);
}

YVR_EXPORT void YVRGetRenderSharpen(const char* packageName, XrSharpenInfoYVR* sharpenInfo)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->getRenderSharpen(packageName, sharpenInfo);
}

YVR_EXPORT void YVRSetRenderFov(const char* packageName, XrRenderFovYVR fovInfo)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setRenderFov(packageName, &fovInfo);
}

YVR_EXPORT void YVRGetRenderFov(const char* packageName, XrRenderFovYVR* fovInfo)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->getRenderFov(packageName, fovInfo);
}

YVR_EXPORT void YVRResetRenderHorizontalFov(const char* packageName)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->resetRenderFov(packageName, XrFovResetTypeYVR::XR_FOV_RESET_HORIZON_YVR);
}

YVR_EXPORT void YVRResetRenderVerticalFov(const char* packageName)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->resetRenderFov(packageName, XrFovResetTypeYVR::XR_FOV_RESET_VERTICAL_YVR);
}

YVR_EXPORT void YVRGetAppColorSpace(const char* packageName, XrColorSpaceConfigYVR* colorSpace)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->getAppColorSpace(packageName, colorSpace);
}

YVR_EXPORT void YVRSetAppColorSpace(const char* packageName, XrColorSpaceConfigYVR colorSpace)
{
    AnnounceCallingFunc();
    plugin.renderMgr->configsMgr->setAppColorSpace(packageName, &colorSpace);
}

YVR_EXPORT void YVRPollEvent()
{
    const XrEventDataBaseHeader* event = plugin.eventsMgr->TryReadNextEvent();
    if (event != nullptr) plugin.eventsMgr->handleEvent(event);
}

#pragma endregion

YVR_EXPORT void YVRSetTravelMode(int mode)
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_SET_TRAVEL_MODE, mode));
}

YVR_EXPORT int YVRGetTravelMode()
{
    AnnounceCallingFunc();
    int result;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_TRAVEL_MODE, &result));
    return result;
}

YVR_EXPORT int YVRGetTravelModeState()
{
    AnnounceCallingFunc();
    int mode;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, YVR_SYS_PROP_GET_TRAVEL_MODE_STATE, &mode));
    return mode;
}


YVR_EXPORT void YVRSetTravelModeStateChangeCallback(OnTravelModeStateChange callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setOnTravelModeStateChangeCallback(callback);
}

YVR_EXPORT void YVRSetTravelModeCallback(OnTravelModeState callback)
{
    AnnounceCallingFunc();
    plugin.eventsMgr->setOnTravelModeStateCallback(callback);
}

YVR_EXPORT int64_t YVRGetPredictedTime()
{
    return plugin.inputMgr->getTime(true);
}

YVR_EXPORT XrPosef YVRGetPredictedTimeHeadPose(int64_t time) { return plugin.hmdMgr->getPredictedTimePose(time); }

YVR_EXPORT void YVRGetDPCompositionStateYVR(YVRDPInfo* dpInfo)
{
    AnnounceCallingFunc();
    XrDPCompostionStateYVR state;
    state.type = XR_TYPE_DP_COMPOSITION_STATE_YVR;
    OpenXRAPI(xrGetDPCompositionStateYVR(plugin.openxrMgr->program->session, &state));

    dpInfo->width = state.width;
    dpInfo->height = state.height;
    dpInfo->isConnected = state.connected;
}

YVR_EXPORT void YVRStartDPComposition()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrStartDPCompositionYVR(plugin.openxrMgr->program->session));
}

YVR_EXPORT void YVRStopDPComposition()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrStopDPCompositionYVR(plugin.openxrMgr->program->session));
}

#pragma region "Passthrough Mesh"

YVR_EXPORT void YVRSetPersistentPassthroughMeshInfoYVR(PersistentPassthroughMeshModeInfo setInfo)
{
    AnnounceCallingFunc();
    plugin.passthroughMeshMgr->SetPersistentPassthroughMeshInfo(setInfo);
}

YVR_EXPORT void YVRGetPersistentPassthroughMeshInfoYVR(PersistentPassthroughMeshModeInfo* getInfo)
{
    AnnounceCallingFunc();
    plugin.passthroughMeshMgr->GetPersistentPassthroughMeshInfo(getInfo);
}

YVR_EXPORT void YVRSetPersistentPassthroughMeshYVR(SpaceTriangleMesh* mesh)
{
    AnnounceCallingFunc();
    plugin.passthroughMeshMgr->SetPersistentPassthroughMesh(mesh);
}

YVR_EXPORT void YVRSetPersistentPassthroughTransformYVR(PersistentPassthroughMeshTransform transform)
{
    AnnounceCallingFunc();
    plugin.passthroughMeshMgr->SetPersistentPassthroughTransform(transform);
}
#pragma endregion
}
