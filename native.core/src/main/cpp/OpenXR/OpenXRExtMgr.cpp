#include "OpenXRExtMgr.h"

#include "../YVRUnityPlugin.h"

namespace
{
template <typename XrFunction>
struct UnsupportedXrFunction;

template <typename Ret, typename... Args>
struct UnsupportedXrFunction<Ret (XRAPI_PTR *)(Args...)>
{
    static XRAPI_ATTR Ret XRAPI_CALL Call(Args...)
    {
        return static_cast<Ret>(XR_ERROR_FUNCTION_UNSUPPORTED);
    }
};

template <typename... Args>
struct UnsupportedXrFunction<void (XRAPI_PTR *)(Args...)>
{
    static XRAPI_ATTR void XRAPI_CALL Call(Args...)
    {
    }
};

template <typename XrFunction>
XrFunction GetUnsupportedXrFunction()
{
    return &UnsupportedXrFunction<XrFunction>::Call;
}

template <typename XrFunction>
void LoadOptionalXrFunction(XrInstance instance, const char* functionName, XrFunction& function)
{
    function = nullptr;
    const XrResult result =
        xrGetInstanceProcAddr(instance, functionName, reinterpret_cast<PFN_xrVoidFunction*>(&function));

    if (XR_FAILED(result) || function == nullptr)
    {
        YWarn("OpenXR optional function unavailable: %s, result=%d", functionName, result);
        function = GetUnsupportedXrFunction<XrFunction>();
    }
}
}

PFN_xrGetSystemPropertyIntYVR xrGetSystemPropertyIntYVR = nullptr;
PFN_xrGetSystemPropertyFloatYVR xrGetSystemPropertyFloatYVR = nullptr;
PFN_xrGetSystemPropertyFloatArrayYVR xrGetSystemPropertyFloatArrayYVR = nullptr;
PFN_xrGetSystemPropertyCharArrayYVR xrGetSystemPropertyCharArrayYVR = nullptr;
PFN_xrSetSystemPropertyIntYVR xrSetSystemPropertyIntYVR = nullptr;
PFN_xrSetSystemPropertyFloatYVR xrSetSystemPropertyFloatYVR = nullptr;
PFN_xrSetForceHeadModeYVR xrSetForceHeadModeYVR = nullptr;

PFN_xrStartControllerPairingYVR xrStartControllerPairingYVR = nullptr;
PFN_xrStopControllerPairingYVR xrStopControllerPairingYVR = nullptr;
PFN_xrRemoveControllerPairingYVR xrRemoveControllerPairingYVR = nullptr;
PFN_xrQueryControllerPairingStateYVR xrQueryControllerPairingStateYVR = nullptr;
PFN_xrGetControllerSerialNumberYVR xrGetControllerSerialNumberYVR = nullptr;
PFN_xrSetPrimaryControllerHandleYVR xrSetPrimaryControllerHandleYVR = nullptr;
PFN_xrGetPrimaryControllerHandleYVR xrGetPrimaryControllerHandleYVR = nullptr;
PFN_xrGetControllerChargeStatusYVR xrGetControllerChargeStatusYVR = nullptr;
PFN_xrQueryCurrentConnectedControllerTypeYVR xrQueryCurrentConnectedControllerTypeYVR = nullptr;

PFN_xrPassthroughStartYVR xrPassthroughStartYVR = nullptr;
PFN_xrPassthroughStopYVR xrPassthroughStopYVR = nullptr;

PFN_xrTestBoundaryNodeYVR xrTestBoundaryNodeYVR = nullptr;
PFN_xrTestBoundaryPointYVR xrTestBoundaryPointYVR = nullptr;
PFN_xrGetBoundaryDimensionsYVR xrGetBoundaryDimensionsYVR = nullptr;
PFN_xrGetBoundaryVisibleYVR xrGetBoundaryVisibleYVR = nullptr;
PFN_xrGetBoundaryGeometryPointsCountYVR xrGetBoundaryGeometryPointsCountYVR = nullptr;
PFN_xrGetBoundaryGeometryYVR xrGetBoundaryGeometryYVR = nullptr;
PFN_xrGetGroundDistanceYVR xrGetGroundDistanceYVR = nullptr;
PFN_xrGetBoundaryConfiguredYVR xrGetBoundaryConfiguredYVR = nullptr;
PFN_xrSetBoundaryControlYVR xrSetBoundaryControlYVR = nullptr;
PFN_xrGetBoundaryStatusYVR xrGetBoundaryStatusYVR = nullptr;

PFN_xrGetOpenGLESGraphicsRequirementsKHR xrGetOpenGLESGraphicsRequirementsKHR = nullptr;
PFN_xrGetVulkanGraphicsRequirements2KHR xrGetVulkanGraphicsRequirements2KHR = nullptr;
PFN_xrSetAndroidApplicationThreadKHR xrSetAndroidApplicationThreadKHR = nullptr;

PFN_xrEnumerateDisplayRefreshRatesFB xrEnumerateDisplayRefreshRatesFB = nullptr;
PFN_xrGetDisplayRefreshRateFB xrGetDisplayRefreshRateFB = nullptr;
PFN_xrRequestDisplayRefreshRateFB xrRequestDisplayRefreshRateFB = nullptr;

PFN_xrCreateFoveationProfileFB xrCreateFoveationProfileFB = nullptr;
PFN_xrDestroyFoveationProfileFB xrDestroyFoveationProfileFB = nullptr;
PFN_xrUpdateSwapchainFB xrUpdateSwapchainFB = nullptr;
PFN_xrGetSwapchainStateFB xrGetSwapchainStateFB = nullptr;

PFN_xrGetVulkanGraphicsDevice2KHR xrGetVulkanGraphicsDevice2KHR = nullptr;
PFN_xrGetVulkanGraphicsDeviceKHR xrGetVulkanGraphicsDeviceKHR = nullptr;
PFN_xrCreateVulkanInstanceKHR xrCreateVulkanInstanceKHR = nullptr;
PFN_xrCreateVulkanDeviceKHR xrCreateVulkanDeviceKHR = nullptr;

PFN_xrCreateHandTrackerEXT xrCreateHandTrackerEXT = nullptr;
PFN_xrDestroyHandTrackerEXT xrDestroyHandTrackerEXT = nullptr;
PFN_xrGetHandMeshFB xrGetHandMeshFB = nullptr;
PFN_xrLocateHandJointsEXT xrLocateHandJointsEXT = nullptr;
PFN_xrSetCurrentInputDeviceYVR xrSetCurrentInputDeviceYVR = nullptr;
PFN_xrGetCurrentInputDeviceYVR xrGetCurrentInputDeviceYVR = nullptr;

PFN_xrCreateSpatialAnchorFB xrCreateSpatialAnchorFB = nullptr;
PFN_xrGetSpaceUuidFB xrGetSpaceUuidFB = nullptr;
PFN_xrEnumerateSpaceSupportedComponentsFB xrEnumerateSpaceSupportedComponentsFB = nullptr;
PFN_xrSetSpaceComponentStatusFB xrSetSpaceComponentStatusFB = nullptr;
PFN_xrGetSpaceComponentStatusFB xrGetSpaceComponentStatusFB = nullptr;

PFN_xrQuerySpacesFB xrQuerySpacesFB = nullptr;
PFN_xrRetrieveSpaceQueryResultsFB xrRetrieveSpaceQueryResultsFB = nullptr;

PFN_xrSaveSpaceFB xrSaveSpaceFB = nullptr;
PFN_xrEraseSpaceFB xrEraseSpaceFB = nullptr;

PFN_xrCreateSpaceUserFB xrCreateSpaceUserFB = nullptr;
PFN_xrGetSpaceUserIdFB xrGetSpaceUserIdFB = nullptr;
PFN_xrDestroySpaceUserFB xrDestroySpaceUserFB = nullptr;

PFN_xrShareSpacesFB xrShareSpacesFB = nullptr;

PFN_xrSaveSpaceListFB xrSaveSpaceListFB = nullptr;

PFN_xrCreatePlaneDetectorEXT xrCreatePlaneDetectorEXT = nullptr;
PFN_xrDestroyPlaneDetectorEXT xrDestroyPlaneDetectorEXT = nullptr;
PFN_xrBeginPlaneDetectionEXT xrBeginPlaneDetectionEXT = nullptr;
PFN_xrGetPlaneDetectionStateEXT xrGetPlaneDetectionStateEXT = nullptr;
PFN_xrGetPlaneDetectionsEXT xrGetPlaneDetectionsEXT = nullptr;
PFN_xrGetPlanePolygonBufferEXT xrGetPlanePolygonBufferEXT = nullptr;

PFN_xrCreateEyeTrackerFB xrCreateEyeTrackerFB = nullptr;
PFN_xrDestroyEyeTrackerFB xrDestroyEyeTrackerFB = nullptr;
PFN_xrGetEyeGazesFB xrGetEyeGazesFB = nullptr;

PFN_xrGetSpaceBoundingBox2DFB xrGetSpaceBoundingBox2DFB = nullptr;
PFN_xrGetSpaceBoundingBox3DFB xrGetSpaceBoundingBox3DFB = nullptr;
PFN_xrGetSpaceSemanticLabelsFB xrGetSpaceSemanticLabelsFB = nullptr;
PFN_xrGetSpaceBoundary2DFB xrGetSpaceBoundary2DFB = nullptr;
PFN_xrGetSpaceRoomLayoutFB xrGetSpaceRoomLayoutFB = nullptr;
PFN_xrGetSpaceContainerFB xrGetSpaceContainerFB = nullptr;
PFN_xrRequestSceneCaptureFB xrRequestSceneCaptureFB = nullptr;

PFN_xrSetSpaceBoundingBox2DYVR xrSetSpaceBoundingBox2DYVR = nullptr;
PFN_xrSetSpaceBoundingBox3DYVR xrSetSpaceBoundingBox3DYVR = nullptr;
PFN_xrSetSpaceSemanticLabelsYVR xrSetSpaceSemanticLabelsYVR = nullptr;
PFN_xrSetSpaceBoundary2DYVR xrSetSpaceBoundary2DYVR = nullptr;
PFN_xrSetSpaceRoomLayoutYVR xrSetSpaceRoomLayoutYVR = nullptr;
PFN_xrSetSpaceContainerYVR xrSetSpaceContainerYVR = nullptr;
PFN_xrResetSceneYVR xrResetSceneYVR = nullptr;
PFN_xrGetSceneStatusYVR xrGetSceneStatusYVR = nullptr;
PFN_xrSetLocateRoomYVR xrSetLocateRoomYVR = nullptr;
PFN_xrGetLocateRoomYVR xrGetLocateRoomYVR = nullptr;
PFN_xrGetSpaceTriangleMeshMETA xrGetSpaceTriangleMeshMETA = nullptr;

PFN_xrGetRecommendedResolution xrGetRecommendedResolution = nullptr;

PFN_xrCreateSwapchainAndroidSurfaceKHR xrCreateSwapchainAndroidSurfaceKhr = nullptr;

PFN_xrCreateMeshDetectorYVR xrCreateMeshDetectorYVR = nullptr;
PFN_xrDestroyMeshDetectorYVR xrDestroyMeshDetectorYVR = nullptr;
PFN_xrBeginRoomSetupYVR xrBeginRoomSetupYVR = nullptr;
PFN_xrEndRoomSetupYVR xrEndRoomSetupYVR = nullptr;
PFN_xrSetSpaceTriangleMeshYVR xrSetSpaceTriangleMeshYVR = nullptr;
PFN_xrBeginBoundaryDetectYVR xrBeginBoundaryDetectYVR = nullptr;
PFN_xrEndBoundaryDetectYVR xrEndBoundaryDetectYVR = nullptr;

PFN_xrCreatePassthroughProviderYVR xrCreatePassthroughProviderYVR = nullptr;
PFN_xrDestroyPassthroughProviderYVR xrDestroyPassthroughProviderYVR = nullptr;
PFN_xrStartPassthroughProviderYVR xrStartPassthroughProviderYVR = nullptr;
PFN_xrStopPassthroughProviderYVR xrStopPassthroughProviderYVR = nullptr;
PFN_xrCreatePassthroughSwapchainYVR xrCreatePassthroughSwapchainYVR = nullptr;
PFN_xrDestroyPassthroughSwapchainYVR xrDestroyPassthroughSwapchainYVR = nullptr;
PFN_xrEnumeratePassthroughSwapchainImagesYVR xrEnumeratePassthroughSwapchainImagesYVR = nullptr;
PFN_xrAcquirePassthroughImageYVR xrAcquirePassthroughImageYVR = nullptr;
PFN_xrSetColorSpaceFB xrSetColorSpaceFB = nullptr;
PFN_xrSetColorSpaceYVR xrSetColorSpaceYVR = nullptr;
PFN_xrCreatePassthroughSwapchainYVR2 xrCreatePassthroughSwapchainYVR2 = nullptr;


PFN_xrSetLargeSpaceMapRecordEnableYVR xrSetLargeSpaceMapRecordEnableYVR = nullptr;
PFN_xrSetMarkerDetectionEnableYVR xrSetMarkerDetectionEnableYVR = nullptr;
PFN_xrClearAllMarkersYVR xrClearAllMarkersYVR = nullptr;
PFN_xrSetLargeSpaceOriginPoseYVR xrSetLargeSpaceOriginPoseYVR = nullptr;
PFN_xrExportLargeSpaceMapYVR xrExportLargeSpaceMapYVR = nullptr;
PFN_xrSetRecenterEnableYVR xrSetRecenterEnableYVR = nullptr;
PFN_xrGetRecenterEnableYVR xrGetRecenterEnableYVR = nullptr;
PFN_xrSetLargeSpaceMapAssemblingEnableYVR xrSetLargeSpaceMapAssemblingEnableYVR = nullptr;
PFN_xrGetLargeSpacePointsCalibParamsYVR xrGetLargeSpacePointsCalibParamsYVR = nullptr;

PFN_xrStartHandCalibrationYVR xrStartHandCalibrationYVR = nullptr;
PFN_xrRegisterHandEventCallbackYVR xrRegisterHandEventCallbackYVR = nullptr;
PFN_xrUnregisterHandEventCallbackYVR xrUnregisterHandEventCallbackYVR = nullptr;
PFN_xrSetSlamCalibrationEnableYVR xrSetSlamCalibrationEnableYVR = nullptr;
PFN_xrStopHandCalibrationYVR xrStopHandCalibrationYVR = nullptr;

PFN_xrGetPlaneDetectionsYVR xrGetPlaneDetectionsYVR = nullptr;
PFN_xrGetPlanePolygonBufferYVR xrGetPlanePolygonBufferYVR = nullptr;

PFN_xrGetRenderScaleYVR xrGetRenderScaleYVR = nullptr;
PFN_xrSetRenderScaleYVR xrSetRenderScaleYVR = nullptr;
PFN_xrGetRenderSharpenYVR xrGetRenderSharpenYVR = nullptr;
PFN_xrSetRenderSharpenYVR xrSetRenderSharpenYVR = nullptr;
PFN_xrGetRenderFovYVR xrGetRenderFovYVR = nullptr;
PFN_xrSetRenderFovYVR xrSetRenderFovYVR = nullptr;
PFN_xrResetRenderFovYVR xrResetRenderFovYVR = nullptr;
PFN_xrGetColorSpaceConfigYVR xrGetColorSpaceConfigYVR = nullptr;
PFN_xrSetColorSpaceConfigYVR xrSetColorSpaceConfigYVR = nullptr;

PFN_xrConvertTimespecTimeToTimeKHR xrConvertTimespecTimeToTimeKHR = nullptr;

PFN_xrSetImageTrackingYVR xrSetImageTrackingYVR = nullptr;
PFN_xrRegisterImageTemplateYVR xrRegisterImageTemplateYVR = nullptr;
PFN_xrUnRegisterImageTemplateYVR xrUnRegisterImageTemplateYVR = nullptr;
PFN_xrCreateImageSpaceYVR xrCreateImageSpaceYVR = nullptr;


PFN_xrCreatePassthroughFB xrCreatePassthroughFB = nullptr;
PFN_xrDestroyPassthroughFB xrDestroyPassthroughFB = nullptr;
PFN_xrPassthroughStartFB xrPassthroughStartFB = nullptr;
PFN_xrPassthroughPauseFB xrPassthroughPauseFB = nullptr;
PFN_xrCreatePassthroughLayerFB xrCreatePassthroughLayerFB = nullptr;
PFN_xrDestroyPassthroughLayerFB xrDestroyPassthroughLayerFB = nullptr;
PFN_xrPassthroughLayerPauseFB xrPassthroughLayerPauseFB = nullptr;
PFN_xrPassthroughLayerResumeFB xrPassthroughLayerResumeFB = nullptr;
PFN_xrPassthroughLayerSetStyleFB xrPassthroughLayerSetStyleFB = nullptr;
PFN_xrCreatePassthroughColorLutMETA xrCreatePassthroughColorLutMETA = nullptr;
PFN_xrDestroyPassthroughColorLutMETA xrDestroyPassthroughColorLutMETA = nullptr;
PFN_xrUpdatePassthroughColorLutMETA xrUpdatePassthroughColorLutMETA = nullptr;

PFN_xrGetDPCompostionStateYVR xrGetDPCompositionStateYVR = nullptr;
PFN_xrStartDPCompostionYVR xrStartDPCompositionYVR = nullptr;
PFN_xrStopDPCompostionYVR xrStopDPCompositionYVR = nullptr;

PFN_xrSetPersistentPassthroughMeshModeInfoYVR xrSetPersistentPassthroughMeshModeInfoYVR = nullptr;
PFN_xrGetPersistentPassthroughMeshModeInfoYVR xrGetPersistentPassthroughMeshModeInfoYVR = nullptr;
PFN_xrSetPersistentPassthroughMeshYVR xrSetPersistentPassthroughMeshYVR = nullptr;
PFN_xrSetPersistentPassthroughMeshTransformYVR xrSetPersistentPassthroughMeshTransformYVR = nullptr;

void OpenXRExtMgr::initExtensions()
{
    const XrInstance& instance = plugin.openxrMgr->program->instance;

    LoadOptionalXrFunction(instance, "xrSetColorSpaceFB", xrSetColorSpaceFB);
    LoadOptionalXrFunction(instance, "xrSetColorSpaceYVR", xrSetColorSpaceYVR);
    LoadOptionalXrFunction(instance, "xrGetSystemPropertyIntYVR", xrGetSystemPropertyIntYVR);
    LoadOptionalXrFunction(instance, "xrGetSystemPropertyFloatYVR", xrGetSystemPropertyFloatYVR);
    LoadOptionalXrFunction(instance, "xrGetSystemPropertyFloatArrayYVR", xrGetSystemPropertyFloatArrayYVR);
    LoadOptionalXrFunction(instance, "xrGetSystemPropertyCharArrayYVR", xrGetSystemPropertyCharArrayYVR);
    LoadOptionalXrFunction(instance, "xrSetSystemPropertyIntYVR", xrSetSystemPropertyIntYVR);
    LoadOptionalXrFunction(instance, "xrSetSystemPropertyFloatYVR", xrSetSystemPropertyFloatYVR);
    LoadOptionalXrFunction(instance, "xrSetForceHeadModeYVR", xrSetForceHeadModeYVR);

    LoadOptionalXrFunction(instance, "xrStartControllerPairingYVR", xrStartControllerPairingYVR);
    LoadOptionalXrFunction(instance, "xrStopControllerPairingYVR", xrStopControllerPairingYVR);
    LoadOptionalXrFunction(instance, "xrRemoveControllerPairingYVR", xrRemoveControllerPairingYVR);
    LoadOptionalXrFunction(instance, "xrQueryControllerPairingStateYVR", xrQueryControllerPairingStateYVR);
    LoadOptionalXrFunction(instance, "xrGetControllerSerialNumberYVR", xrGetControllerSerialNumberYVR);
    LoadOptionalXrFunction(instance, "xrSetPrimaryControllerHandleYVR", xrSetPrimaryControllerHandleYVR);
    LoadOptionalXrFunction(instance, "xrGetPrimaryControllerHandleYVR", xrGetPrimaryControllerHandleYVR);
    LoadOptionalXrFunction(instance, "xrGetControllerChargeStatusYVR", xrGetControllerChargeStatusYVR);

    LoadOptionalXrFunction(instance, "xrPassthroughStartYVR", xrPassthroughStartYVR);
    LoadOptionalXrFunction(instance, "xrPassthroughStopYVR", xrPassthroughStopYVR);

    LoadOptionalXrFunction(instance, "xrTestBoundaryNodeYVR", xrTestBoundaryNodeYVR);
    LoadOptionalXrFunction(instance, "xrTestBoundaryPointYVR", xrTestBoundaryPointYVR);
    LoadOptionalXrFunction(instance, "xrGetBoundaryDimensionsYVR", xrGetBoundaryDimensionsYVR);
    LoadOptionalXrFunction(instance, "xrGetBoundaryVisibleYVR", xrGetBoundaryVisibleYVR);
    LoadOptionalXrFunction(instance, "xrGetBoundaryGeometryPointsCountYVR", xrGetBoundaryGeometryPointsCountYVR);
    LoadOptionalXrFunction(instance, "xrGetBoundaryGeometryYVR", xrGetBoundaryGeometryYVR);
    LoadOptionalXrFunction(instance, "xrGetGroundDistanceYVR", xrGetGroundDistanceYVR);
    LoadOptionalXrFunction(instance, "xrGetBoundaryConfiguredYVR", xrGetBoundaryConfiguredYVR);
    LoadOptionalXrFunction(instance, "xrSetBoundaryControlYVR", xrSetBoundaryControlYVR);
    LoadOptionalXrFunction(instance, "xrGetBoundaryStatusYVR", xrGetBoundaryStatusYVR);

    LoadOptionalXrFunction(instance, "xrGetOpenGLESGraphicsRequirementsKHR", xrGetOpenGLESGraphicsRequirementsKHR);
    LoadOptionalXrFunction(instance, "xrGetVulkanGraphicsRequirements2KHR", xrGetVulkanGraphicsRequirements2KHR);
    LoadOptionalXrFunction(instance, "xrSetAndroidApplicationThreadKHR", xrSetAndroidApplicationThreadKHR);

    LoadOptionalXrFunction(instance, "xrEnumerateDisplayRefreshRatesFB", xrEnumerateDisplayRefreshRatesFB);
    LoadOptionalXrFunction(instance, "xrGetDisplayRefreshRateFB", xrGetDisplayRefreshRateFB);
    LoadOptionalXrFunction(instance, "xrRequestDisplayRefreshRateFB", xrRequestDisplayRefreshRateFB);

    LoadOptionalXrFunction(instance, "xrCreateFoveationProfileFB", xrCreateFoveationProfileFB);
    LoadOptionalXrFunction(instance, "xrDestroyFoveationProfileFB", xrDestroyFoveationProfileFB);
    LoadOptionalXrFunction(instance, "xrUpdateSwapchainFB", xrUpdateSwapchainFB);
    LoadOptionalXrFunction(instance, "xrGetSwapchainStateFB", xrGetSwapchainStateFB);

    LoadOptionalXrFunction(instance, "xrGetVulkanGraphicsDevice2KHR", xrGetVulkanGraphicsDevice2KHR);
    LoadOptionalXrFunction(instance, "xrGetVulkanGraphicsDeviceKHR", xrGetVulkanGraphicsDeviceKHR);
    LoadOptionalXrFunction(instance, "xrCreateVulkanInstanceKHR", xrCreateVulkanInstanceKHR);
    LoadOptionalXrFunction(instance, "xrCreateVulkanDeviceKHR", xrCreateVulkanDeviceKHR);

    LoadOptionalXrFunction(instance, "xrCreateHandTrackerEXT", xrCreateHandTrackerEXT);
    LoadOptionalXrFunction(instance, "xrDestroyHandTrackerEXT", xrDestroyHandTrackerEXT);
    LoadOptionalXrFunction(instance, "xrLocateHandJointsEXT", xrLocateHandJointsEXT);
    LoadOptionalXrFunction(instance, "xrGetHandMeshFB", xrGetHandMeshFB);
    LoadOptionalXrFunction(instance, "xrSetCurrentInputDeviceYVR", xrSetCurrentInputDeviceYVR);
    LoadOptionalXrFunction(instance, "xrGetCurrentInputDeviceYVR", xrGetCurrentInputDeviceYVR);

    LoadOptionalXrFunction(instance, "xrCreateSpatialAnchorFB", xrCreateSpatialAnchorFB);
    LoadOptionalXrFunction(instance, "xrGetSpaceUuidFB", xrGetSpaceUuidFB);
    LoadOptionalXrFunction(instance, "xrEnumerateSpaceSupportedComponentsFB", xrEnumerateSpaceSupportedComponentsFB);
    LoadOptionalXrFunction(instance, "xrSetSpaceComponentStatusFB", xrSetSpaceComponentStatusFB);
    LoadOptionalXrFunction(instance, "xrGetSpaceComponentStatusFB", xrGetSpaceComponentStatusFB);

    LoadOptionalXrFunction(instance, "xrQuerySpacesFB", xrQuerySpacesFB);
    LoadOptionalXrFunction(instance, "xrRetrieveSpaceQueryResultsFB", xrRetrieveSpaceQueryResultsFB);

    LoadOptionalXrFunction(instance, "xrSaveSpaceFB", xrSaveSpaceFB);
    LoadOptionalXrFunction(instance, "xrEraseSpaceFB", xrEraseSpaceFB);

    LoadOptionalXrFunction(instance, "xrCreateSpaceUserFB", xrCreateSpaceUserFB);
    LoadOptionalXrFunction(instance, "xrGetSpaceUserIdFB", xrGetSpaceUserIdFB);
    LoadOptionalXrFunction(instance, "xrDestroySpaceUserFB", xrDestroySpaceUserFB);

    LoadOptionalXrFunction(instance, "xrShareSpacesFB", xrShareSpacesFB);

    LoadOptionalXrFunction(instance, "xrSaveSpaceListFB", xrSaveSpaceListFB);

    LoadOptionalXrFunction(instance, "xrCreatePlaneDetectorEXT", xrCreatePlaneDetectorEXT);
    LoadOptionalXrFunction(instance, "xrDestroyPlaneDetectorEXT", xrDestroyPlaneDetectorEXT);
    LoadOptionalXrFunction(instance, "xrBeginPlaneDetectionEXT", xrBeginPlaneDetectionEXT);
    LoadOptionalXrFunction(instance, "xrGetPlaneDetectionStateEXT", xrGetPlaneDetectionStateEXT);
    LoadOptionalXrFunction(instance, "xrGetPlaneDetectionsEXT", xrGetPlaneDetectionsEXT);
    LoadOptionalXrFunction(instance, "xrGetPlanePolygonBufferEXT", xrGetPlanePolygonBufferEXT);

    LoadOptionalXrFunction(instance, "xrCreateEyeTrackerFB", xrCreateEyeTrackerFB);
    LoadOptionalXrFunction(instance, "xrDestroyEyeTrackerFB", xrDestroyEyeTrackerFB);
    LoadOptionalXrFunction(instance, "xrGetEyeGazesFB", xrGetEyeGazesFB);

    LoadOptionalXrFunction(instance, "xrGetSpaceBoundingBox2DFB", xrGetSpaceBoundingBox2DFB);
    LoadOptionalXrFunction(instance, "xrGetSpaceBoundingBox3DFB", xrGetSpaceBoundingBox3DFB);
    LoadOptionalXrFunction(instance, "xrGetSpaceSemanticLabelsFB", xrGetSpaceSemanticLabelsFB);
    LoadOptionalXrFunction(instance, "xrGetSpaceBoundary2DFB", xrGetSpaceBoundary2DFB);
    LoadOptionalXrFunction(instance, "xrGetSpaceRoomLayoutFB", xrGetSpaceRoomLayoutFB);
    LoadOptionalXrFunction(instance, "xrGetSpaceContainerFB", xrGetSpaceContainerFB);
    LoadOptionalXrFunction(instance, "xrRequestSceneCaptureFB", xrRequestSceneCaptureFB);

    LoadOptionalXrFunction(instance, "xrSetSpaceBoundingBox2DYVR", xrSetSpaceBoundingBox2DYVR);
    LoadOptionalXrFunction(instance, "xrSetSpaceBoundingBox3DYVR", xrSetSpaceBoundingBox3DYVR);
    LoadOptionalXrFunction(instance, "xrSetSpaceSemanticLabelsYVR", xrSetSpaceSemanticLabelsYVR);
    LoadOptionalXrFunction(instance, "xrSetSpaceBoundary2DYVR", xrSetSpaceBoundary2DYVR);
    LoadOptionalXrFunction(instance, "xrSetSpaceRoomLayoutYVR", xrSetSpaceRoomLayoutYVR);
    LoadOptionalXrFunction(instance, "xrSetSpaceContainerYVR", xrSetSpaceContainerYVR);
    LoadOptionalXrFunction(instance, "xrResetSceneYVR", xrResetSceneYVR);
    LoadOptionalXrFunction(instance, "xrGetSceneStatusYVR", xrGetSceneStatusYVR);
    LoadOptionalXrFunction(instance, "xrSetLocateRoomYVR", xrSetLocateRoomYVR);
    LoadOptionalXrFunction(instance, "xrGetLocateRoomYVR", xrGetLocateRoomYVR);
    LoadOptionalXrFunction(instance, "xrGetSpaceTriangleMeshMETA", xrGetSpaceTriangleMeshMETA);

    LoadOptionalXrFunction(instance, "xrGetRecommendedResolution", xrGetRecommendedResolution);

    LoadOptionalXrFunction(instance, "xrCreateSwapchainAndroidSurfaceKHR", xrCreateSwapchainAndroidSurfaceKhr);

    LoadOptionalXrFunction(instance, "xrCreateMeshDetectorYVR", xrCreateMeshDetectorYVR);
    LoadOptionalXrFunction(instance, "xrDestroyMeshDetectorYVR", xrDestroyMeshDetectorYVR);
    LoadOptionalXrFunction(instance, "xrBeginRoomSetupYVR", xrBeginRoomSetupYVR);
    LoadOptionalXrFunction(instance, "xrEndRoomSetupYVR", xrEndRoomSetupYVR);
    LoadOptionalXrFunction(instance, "xrSetSpaceTriangleMeshYVR", xrSetSpaceTriangleMeshYVR);

    LoadOptionalXrFunction(instance, "xrCreatePassthroughProviderYVR", xrCreatePassthroughProviderYVR);
    LoadOptionalXrFunction(instance, "xrDestroyPassthroughProviderYVR", xrDestroyPassthroughProviderYVR);
    LoadOptionalXrFunction(instance, "xrStartPassthroughProviderYVR", xrStartPassthroughProviderYVR);
    LoadOptionalXrFunction(instance, "xrStopPassthroughProviderYVR", xrStopPassthroughProviderYVR);
    LoadOptionalXrFunction(instance, "xrCreatePassthroughSwapchainYVR", xrCreatePassthroughSwapchainYVR);
    LoadOptionalXrFunction(instance, "xrDestroyPassthroughSwapchainYVR", xrDestroyPassthroughSwapchainYVR);
    LoadOptionalXrFunction(instance, "xrEnumeratePassthroughSwapchainImagesYVR", xrEnumeratePassthroughSwapchainImagesYVR);
    LoadOptionalXrFunction(instance, "xrAcquirePassthroughImageYVR", xrAcquirePassthroughImageYVR);
    LoadOptionalXrFunction(instance, "xrBeginBoundaryDetectYVR", xrBeginBoundaryDetectYVR);
    LoadOptionalXrFunction(instance, "xrEndBoundaryDetectYVR", xrEndBoundaryDetectYVR);

    LoadOptionalXrFunction(instance, "xrSetLargeSpaceMapRecordEnableYVR", xrSetLargeSpaceMapRecordEnableYVR);
    LoadOptionalXrFunction(instance, "xrSetMarkerDetectionEnableYVR", xrSetMarkerDetectionEnableYVR);
    LoadOptionalXrFunction(instance, "xrClearAllMarkersYVR", xrClearAllMarkersYVR);
    LoadOptionalXrFunction(instance, "xrSetLargeSpaceOriginPoseYVR", xrSetLargeSpaceOriginPoseYVR);
    LoadOptionalXrFunction(instance, "xrExportLargeSpaceMapYVR", xrExportLargeSpaceMapYVR);
    LoadOptionalXrFunction(instance, "xrSetRecenterEnableYVR", xrSetRecenterEnableYVR);
    LoadOptionalXrFunction(instance, "xrGetRecenterEnableYVR", xrGetRecenterEnableYVR);
    LoadOptionalXrFunction(instance, "xrSetLargeSpaceMapAssemblingEnableYVR", xrSetLargeSpaceMapAssemblingEnableYVR);
    LoadOptionalXrFunction(instance, "xrGetLargeSpacePointsCalibParamsYVR", xrGetLargeSpacePointsCalibParamsYVR);
    LoadOptionalXrFunction(instance, "xrQueryCurrentConnectedControllerTypeYVR", xrQueryCurrentConnectedControllerTypeYVR);

    LoadOptionalXrFunction(instance, "xrStartHandCalibrationYVR", xrStartHandCalibrationYVR);
    LoadOptionalXrFunction(instance, "xrRegisterHandEventCallbackYVR", xrRegisterHandEventCallbackYVR);
    LoadOptionalXrFunction(instance, "xrUnregisterHandEventCallbackYVR", xrUnregisterHandEventCallbackYVR);
    LoadOptionalXrFunction(instance, "xrStopHandCalibrationYVR", xrStopHandCalibrationYVR);


    LoadOptionalXrFunction(instance, "xrSetSlamCalibrationEnableYVR", xrSetSlamCalibrationEnableYVR);

    LoadOptionalXrFunction(instance, "xrGetPlaneDetectionsYVR", xrGetPlaneDetectionsYVR);
    LoadOptionalXrFunction(instance, "xrGetPlanePolygonBufferYVR", xrGetPlanePolygonBufferYVR);

    LoadOptionalXrFunction(instance, "xrGetRenderScaleYVR", xrGetRenderScaleYVR);
    LoadOptionalXrFunction(instance, "xrSetRenderScaleYVR", xrSetRenderScaleYVR);
    LoadOptionalXrFunction(instance, "xrGetRenderSharpenYVR", xrGetRenderSharpenYVR);
    LoadOptionalXrFunction(instance, "xrSetRenderSharpenYVR", xrSetRenderSharpenYVR);
    LoadOptionalXrFunction(instance, "xrGetRenderFovYVR", xrGetRenderFovYVR);
    LoadOptionalXrFunction(instance, "xrSetRenderFovYVR", xrSetRenderFovYVR);
    LoadOptionalXrFunction(instance, "xrResetRenderFovYVR", xrResetRenderFovYVR);
    LoadOptionalXrFunction(instance, "xrGetColorSpaceConfigYVR", xrGetColorSpaceConfigYVR);
    LoadOptionalXrFunction(instance, "xrSetColorSpaceConfigYVR", xrSetColorSpaceConfigYVR);

    LoadOptionalXrFunction(instance, "xrConvertTimespecTimeToTimeKHR", xrConvertTimespecTimeToTimeKHR);

    LoadOptionalXrFunction(instance, "xrSetImageTrackingYVR", xrSetImageTrackingYVR);
    LoadOptionalXrFunction(instance, "xrRegisterImageTemplateYVR", xrRegisterImageTemplateYVR);
    LoadOptionalXrFunction(instance, "xrUnRegisterImageTemplateYVR", xrUnRegisterImageTemplateYVR);
    LoadOptionalXrFunction(instance, "xrCreateImageSpaceYVR", xrCreateImageSpaceYVR);

    LoadOptionalXrFunction(instance, "xrCreatePassthroughFB", xrCreatePassthroughFB);
    LoadOptionalXrFunction(instance, "xrDestroyPassthroughFB", xrDestroyPassthroughFB);
    LoadOptionalXrFunction(instance, "xrPassthroughStartFB", xrPassthroughStartFB);
    LoadOptionalXrFunction(instance, "xrPassthroughPauseFB", xrPassthroughPauseFB);
    LoadOptionalXrFunction(instance, "xrCreatePassthroughLayerFB", xrCreatePassthroughLayerFB);
    LoadOptionalXrFunction(instance, "xrDestroyPassthroughLayerFB", xrDestroyPassthroughLayerFB);
    LoadOptionalXrFunction(instance, "xrPassthroughLayerPauseFB", xrPassthroughLayerPauseFB);
    LoadOptionalXrFunction(instance, "xrPassthroughLayerResumeFB", xrPassthroughLayerResumeFB);
    LoadOptionalXrFunction(instance, "xrPassthroughLayerSetStyleFB", xrPassthroughLayerSetStyleFB);
    LoadOptionalXrFunction(instance, "xrCreatePassthroughColorLutMETA", xrCreatePassthroughColorLutMETA);
    LoadOptionalXrFunction(instance, "xrDestroyPassthroughColorLutMETA", xrDestroyPassthroughColorLutMETA);
    LoadOptionalXrFunction(instance, "xrUpdatePassthroughColorLutMETA", xrUpdatePassthroughColorLutMETA);
    LoadOptionalXrFunction(instance, "xrCreatePassthroughSwapchainYVR2", xrCreatePassthroughSwapchainYVR2);

    LoadOptionalXrFunction(instance, "xrGetDPCompostionStateYVR", xrGetDPCompositionStateYVR);
    YInfo("Obtained xrGetDPCompostionStateYVR address: %p", (void*)xrGetDPCompositionStateYVR);
    LoadOptionalXrFunction(instance, "xrStartDPCompostionYVR", xrStartDPCompositionYVR);
    LoadOptionalXrFunction(instance, "xrStopDPCompostionYVR", xrStopDPCompositionYVR);

    LoadOptionalXrFunction(instance, "xrSetPersistentPassthroughMeshModeInfoYVR", xrSetPersistentPassthroughMeshModeInfoYVR);
    LoadOptionalXrFunction(instance, "xrGetPersistentPassthroughMeshModeInfoYVR", xrGetPersistentPassthroughMeshModeInfoYVR);
    LoadOptionalXrFunction(instance, "xrSetPersistentPassthroughMeshYVR", xrSetPersistentPassthroughMeshYVR);
    LoadOptionalXrFunction(instance, "xrSetPersistentPassthroughMeshTransformYVR", xrSetPersistentPassthroughMeshTransformYVR);

}

std::vector<std::string> OpenXRExtMgr::getInstanceExtensions()
{
    std::vector<std::string> requiredExtensions = {XR_YVR_CONTROLLER_TRACKING_FUNCTIONS_EXTENSION_NAME,
                                                   XR_YVR_BOUNDARY_FUNCTIONS_EXTENSION_NAME,
                                                   XR_YVR_CONFIG_FUNCTIONS_EXTENSION_NAME,
                                                   XR_YVR_FRAME_END_INFO_EXT_EXTENSION_NAME,
                                                   XR_YVR_PASSTHROUGH_EXTENSION_NAME,
                                                   XR_FB_DISPLAY_REFRESH_RATE_EXTENSION_NAME,
                                                   XR_FB_FOVEATION_EXTENSION_NAME,
                                                   XR_FB_FOVEATION_CONFIGURATION_EXTENSION_NAME,
                                                   XR_FB_SWAPCHAIN_UPDATE_STATE_EXTENSION_NAME,
                                                   XR_KHR_COMPOSITION_LAYER_DEPTH_EXTENSION_NAME,
                                                   XR_YVR_SPACE_ACCELERATION_EXT_EXTENSION_NAME,
                                                   XR_YVR_COLOR_SPACE_EXTENSION_NAME,
                                                   XR_FB_HAND_TRACKING_MESH_EXTENSION_NAME,
                                                   XR_EXT_HAND_TRACKING_EXTENSION_NAME,
                                                   XR_YVR_CAMERA_FUNCTIONS_EXTENSION_NAME,
                                                   XR_EXT_LOCAL_FLOOR_EXTENSION_NAME,
                                                   XR_FB_SPATIAL_ENTITY_EXTENSION_NAME,
                                                   XR_FB_SPATIAL_ENTITY_QUERY_EXTENSION_NAME,
                                                   XR_FB_SPATIAL_ENTITY_STORAGE_EXTENSION_NAME,
                                                   XR_FB_SPATIAL_ENTITY_USER_EXTENSION_NAME,
                                                   XR_FB_SPATIAL_ENTITY_SHARING_EXTENSION_NAME,
                                                   XR_FB_SPATIAL_ENTITY_STORAGE_BATCH_EXTENSION_NAME,
                                                   XR_EXT_PLANE_DETECTION_EXTENSION_NAME,
                                                   XR_FB_SCENE_EXTENSION_NAME,
                                                   XR_FB_SPATIAL_ENTITY_CONTAINER_EXTENSION_NAME,
                                                   XR_FB_SCENE_CAPTURE_EXTENSION_NAME,
                                                   XR_YVR_SCENE_EXTENSION_NAME,
                                                   XR_YVR_LOCATE_ROOM_EXTENSION_NAME,
                                                   XR_YVR_SPATIAL_ENTITY_CONTAINER_EXTENSION_NAME,
                                                   XR_YVR_SCENE_CAPTURE_EXTENSION_NAME,
                                                   XR_YVR_HAND_TRACKING_AIM_EXTENSION_NAME,
                                                   XR_KHR_CONVERT_TIMESPEC_TIME_EXTENSION_NAME,
                                                   XR_YVR_IMAGE_TRACKING_EXTENSION_NAME,
                                                   XR_YVR_DP_COMPOSITION_EXTENSION_NAME
        };

    requiredExtensions.push_back(XR_OCULUS_ANDROID_SESSION_STATE_ENABLE_EXTENSION_NAME);
    requiredExtensions.push_back(XR_FB_SPACE_WARP_EXTENSION_NAME);
    requiredExtensions.push_back(XR_FB_EYE_TRACKING_SOCIAL_EXTENSION_NAME);
    requiredExtensions.push_back(XR_FB_COMPOSITION_LAYER_SETTINGS_EXTENSION_NAME);
    requiredExtensions.push_back(XR_YVR_ADAPTIVE_RESOLUTION_EXTENSION_NAME);
    requiredExtensions.push_back(XR_EXT_EYE_GAZE_INTERACTION_EXTENSION_NAME);
    requiredExtensions.push_back(XR_KHR_ANDROID_SURFACE_SWAPCHAIN_EXTENSION_NAME);
    requiredExtensions.push_back(XR_FB_COMPOSITION_LAYER_IMAGE_LAYOUT_EXTENSION_NAME);
    requiredExtensions.push_back(XR_YVR_SPATIAL_ENTITY_MESH_EXTENSION_NAME);
    requiredExtensions.push_back(XR_META_SPATIAL_ENTITY_MESH_EXTENSION_NAME);
    requiredExtensions.push_back(XR_FB_SWAPCHAIN_UPDATE_STATE_ANDROID_SURFACE_EXTENSION_NAME);
    requiredExtensions.push_back(XR_YVR_PASSTHROUGH_CONTENT_EXTENSION_NAME);
    requiredExtensions.push_back(XR_VARJO_FOVEATED_RENDERING_EXTENSION_NAME);
    requiredExtensions.push_back(XR_VARJO_QUAD_VIEWS_EXTENSION_NAME);
    requiredExtensions.push_back(XR_FB_COLOR_SPACE_EXTENSION_NAME);
    requiredExtensions.push_back(XR_YVR_LARGE_SPACE_EXTENSION_NAME);
    requiredExtensions.push_back(XR_YVR_SLAM_TRACKING_CALIBRATION_EXTENSION_NAME);
    requiredExtensions.push_back(XR_YVR_RENDER_SCALE_EXTENSION_NAME);
    requiredExtensions.push_back(XR_FB_PASSTHROUGH_EXTENSION_NAME);
    requiredExtensions.push_back(XR_META_PASSTHROUGH_COLOR_LUT_EXTENSION_NAME);
    requiredExtensions.push_back(XR_YVR_PERSISTENT_PASSTHROUGH_MESH_EXTENSION_NAME);

    std::vector<std::string> supportedExtensions = plugin.openxrMgr->program->supportedExtensions;
    std::sort(requiredExtensions.begin(), requiredExtensions.end());
    std::sort(supportedExtensions.begin(), supportedExtensions.end());
    std::set_intersection(requiredExtensions.begin(), requiredExtensions.end(), supportedExtensions.begin(), supportedExtensions.end(),
                          std::back_inserter(enabledExtensions));

    return enabledExtensions;
}

float OpenXRExtMgr::getFloatProperty(const xrGetSystemPropertyYVR& property)
{
    float ret = 0.0f;
    const XrResult result = OpenXRAPI(xrGetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, property, &ret));
    if (XR_FAILED(result)) return 0.0f;
    return ret;
}

void OpenXRExtMgr::setFloatProperty(const xrSetSystemPropertyYVR& property, float value)
{
    OpenXRAPI(xrSetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, property, value));
}

int OpenXRExtMgr::getIntProperty(const xrGetSystemPropertyYVR& property)
{
    int ret = 0;
    const XrResult result = OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, property, &ret));
    if (XR_FAILED(result)) return 0;
    return ret;
}

void OpenXRExtMgr::getFloatArrayProperty(const xrGetSystemPropertyYVR& property, float* array, int dataLength)
{
    if (array == nullptr || dataLength <= 0) return;

    std::fill(array, array + dataLength, 0.0f);
    int outputDataLength = 0;
    OpenXRAPI(xrGetSystemPropertyFloatArrayYVR(plugin.openxrMgr->program->session, property, array, &outputDataLength));
}

void OpenXRExtMgr::getCharArrayProperty(const xrGetSystemPropertyYVR& property, char* array, int dataLength)
{
    if (array == nullptr || dataLength <= 0) return;

    array[0] = '\0';
    int outputDataLength = 0;
    OpenXRAPI(xrGetSystemPropertyCharArrayYVR(plugin.openxrMgr->program->session, property, array, &outputDataLength));
}

bool OpenXRExtMgr::isEnableTargetExt(string ext)
{
    return std::find(enabledExtensions.begin(), enabledExtensions.end(), ext) != enabledExtensions.end();
}
