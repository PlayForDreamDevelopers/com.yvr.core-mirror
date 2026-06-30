#include "OpenXRExtMgr.h"

#include "../YVRUnityPlugin.h"

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

    xrGetInstanceProcAddr(instance, "xrSetColorSpaceFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetColorSpaceFB));
    xrGetInstanceProcAddr(instance, "xrSetColorSpaceYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetColorSpaceYVR));
    xrGetInstanceProcAddr(instance, "xrGetSystemPropertyIntYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSystemPropertyIntYVR));
    xrGetInstanceProcAddr(instance, "xrGetSystemPropertyFloatYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSystemPropertyFloatYVR));
    xrGetInstanceProcAddr(instance, "xrGetSystemPropertyFloatArrayYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSystemPropertyFloatArrayYVR));
    xrGetInstanceProcAddr(instance, "xrGetSystemPropertyCharArrayYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSystemPropertyCharArrayYVR));
    xrGetInstanceProcAddr(instance, "xrSetSystemPropertyIntYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSystemPropertyIntYVR));
    xrGetInstanceProcAddr(instance, "xrSetSystemPropertyFloatYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSystemPropertyFloatYVR));
    xrGetInstanceProcAddr(instance, "xrSetForceHeadModeYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetForceHeadModeYVR));

    xrGetInstanceProcAddr(instance, "xrStartControllerPairingYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrStartControllerPairingYVR));
    xrGetInstanceProcAddr(instance, "xrStopControllerPairingYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrStopControllerPairingYVR));
    xrGetInstanceProcAddr(instance, "xrRemoveControllerPairingYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrRemoveControllerPairingYVR));
    xrGetInstanceProcAddr(instance, "xrQueryControllerPairingStateYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrQueryControllerPairingStateYVR));
    xrGetInstanceProcAddr(instance, "xrGetControllerSerialNumberYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetControllerSerialNumberYVR));
    xrGetInstanceProcAddr(instance, "xrSetPrimaryControllerHandleYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetPrimaryControllerHandleYVR));
    xrGetInstanceProcAddr(instance, "xrGetPrimaryControllerHandleYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetPrimaryControllerHandleYVR));
    xrGetInstanceProcAddr(instance, "xrGetControllerChargeStatusYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetControllerChargeStatusYVR));

    xrGetInstanceProcAddr(instance, "xrPassthroughStartYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrPassthroughStartYVR));
    xrGetInstanceProcAddr(instance, "xrPassthroughStopYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrPassthroughStopYVR));

    xrGetInstanceProcAddr(instance, "xrTestBoundaryNodeYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrTestBoundaryNodeYVR));
    xrGetInstanceProcAddr(instance, "xrTestBoundaryPointYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrTestBoundaryPointYVR));
    xrGetInstanceProcAddr(instance, "xrGetBoundaryDimensionsYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetBoundaryDimensionsYVR));
    xrGetInstanceProcAddr(instance, "xrGetBoundaryVisibleYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetBoundaryVisibleYVR));
    xrGetInstanceProcAddr(instance, "xrGetBoundaryGeometryPointsCountYVR",
                          reinterpret_cast<PFN_xrVoidFunction*>(&xrGetBoundaryGeometryPointsCountYVR));
    xrGetInstanceProcAddr(instance, "xrGetBoundaryGeometryYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetBoundaryGeometryYVR));
    xrGetInstanceProcAddr(instance, "xrGetGroundDistanceYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetGroundDistanceYVR));
    xrGetInstanceProcAddr(instance, "xrGetBoundaryConfiguredYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetBoundaryConfiguredYVR));
    xrGetInstanceProcAddr(instance, "xrSetBoundaryControlYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetBoundaryControlYVR));
    xrGetInstanceProcAddr(instance, "xrGetBoundaryStatusYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetBoundaryStatusYVR));

    xrGetInstanceProcAddr(instance, "xrGetOpenGLESGraphicsRequirementsKHR",
                          reinterpret_cast<PFN_xrVoidFunction*>(&xrGetOpenGLESGraphicsRequirementsKHR));
    xrGetInstanceProcAddr(instance, "xrGetVulkanGraphicsRequirements2KHR",
                          reinterpret_cast<PFN_xrVoidFunction*>(&xrGetVulkanGraphicsRequirements2KHR));
    xrGetInstanceProcAddr(instance, "xrSetAndroidApplicationThreadKHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetAndroidApplicationThreadKHR));

    xrGetInstanceProcAddr(instance, "xrEnumerateDisplayRefreshRatesFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrEnumerateDisplayRefreshRatesFB));
    xrGetInstanceProcAddr(instance, "xrGetDisplayRefreshRateFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetDisplayRefreshRateFB));
    xrGetInstanceProcAddr(instance, "xrRequestDisplayRefreshRateFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrRequestDisplayRefreshRateFB));

    xrGetInstanceProcAddr(instance, "xrCreateFoveationProfileFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateFoveationProfileFB));
    xrGetInstanceProcAddr(instance, "xrDestroyFoveationProfileFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyFoveationProfileFB));
    xrGetInstanceProcAddr(instance, "xrUpdateSwapchainFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrUpdateSwapchainFB));
    xrGetInstanceProcAddr(instance, "xrGetSwapchainStateFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSwapchainStateFB));

    xrGetInstanceProcAddr(instance, "xrGetVulkanGraphicsDevice2KHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetVulkanGraphicsDevice2KHR));
    xrGetInstanceProcAddr(instance, "xrGetVulkanGraphicsDeviceKHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetVulkanGraphicsDeviceKHR));
    xrGetInstanceProcAddr(instance, "xrCreateVulkanInstanceKHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateVulkanInstanceKHR));
    xrGetInstanceProcAddr(instance, "xrCreateVulkanDeviceKHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateVulkanDeviceKHR));

    xrGetInstanceProcAddr(instance, "xrCreateHandTrackerEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateHandTrackerEXT));
    xrGetInstanceProcAddr(instance, "xrDestroyHandTrackerEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyHandTrackerEXT));
    xrGetInstanceProcAddr(instance, "xrLocateHandJointsEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrLocateHandJointsEXT));
    xrGetInstanceProcAddr(instance, "xrGetHandMeshFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetHandMeshFB));
    xrGetInstanceProcAddr(instance, "xrSetCurrentInputDeviceYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetCurrentInputDeviceYVR));
    xrGetInstanceProcAddr(instance, "xrGetCurrentInputDeviceYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetCurrentInputDeviceYVR));

    xrGetInstanceProcAddr(instance, "xrCreateSpatialAnchorFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateSpatialAnchorFB));
    xrGetInstanceProcAddr(instance, "xrGetSpaceUuidFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceUuidFB));
    xrGetInstanceProcAddr(instance, "xrEnumerateSpaceSupportedComponentsFB",
                          reinterpret_cast<PFN_xrVoidFunction*>(&xrEnumerateSpaceSupportedComponentsFB));
    xrGetInstanceProcAddr(instance, "xrSetSpaceComponentStatusFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSpaceComponentStatusFB));
    xrGetInstanceProcAddr(instance, "xrGetSpaceComponentStatusFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceComponentStatusFB));

    xrGetInstanceProcAddr(instance, "xrQuerySpacesFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrQuerySpacesFB));
    xrGetInstanceProcAddr(instance, "xrRetrieveSpaceQueryResultsFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrRetrieveSpaceQueryResultsFB));

    xrGetInstanceProcAddr(instance, "xrSaveSpaceFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrSaveSpaceFB));
    xrGetInstanceProcAddr(instance, "xrEraseSpaceFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrEraseSpaceFB));

    xrGetInstanceProcAddr(instance, "xrCreateSpaceUserFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateSpaceUserFB));
    xrGetInstanceProcAddr(instance, "xrGetSpaceUserIdFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceUserIdFB));
    xrGetInstanceProcAddr(instance, "xrDestroySpaceUserFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroySpaceUserFB));

    xrGetInstanceProcAddr(instance, "xrShareSpacesFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrShareSpacesFB));

    xrGetInstanceProcAddr(instance, "xrSaveSpaceListFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrSaveSpaceListFB));

    xrGetInstanceProcAddr(instance, "xrCreatePlaneDetectorEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreatePlaneDetectorEXT));
    xrGetInstanceProcAddr(instance, "xrDestroyPlaneDetectorEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyPlaneDetectorEXT));
    xrGetInstanceProcAddr(instance, "xrBeginPlaneDetectionEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrBeginPlaneDetectionEXT));
    xrGetInstanceProcAddr(instance, "xrGetPlaneDetectionStateEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetPlaneDetectionStateEXT));
    xrGetInstanceProcAddr(instance, "xrGetPlaneDetectionsEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetPlaneDetectionsEXT));
    xrGetInstanceProcAddr(instance, "xrGetPlanePolygonBufferEXT", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetPlanePolygonBufferEXT));

    xrGetInstanceProcAddr(instance, "xrCreateEyeTrackerFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateEyeTrackerFB));
    xrGetInstanceProcAddr(instance, "xrDestroyEyeTrackerFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyEyeTrackerFB));
    xrGetInstanceProcAddr(instance, "xrGetEyeGazesFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetEyeGazesFB));

    xrGetInstanceProcAddr(instance, "xrGetSpaceBoundingBox2DFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceBoundingBox2DFB));
    xrGetInstanceProcAddr(instance, "xrGetSpaceBoundingBox3DFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceBoundingBox3DFB));
    xrGetInstanceProcAddr(instance, "xrGetSpaceSemanticLabelsFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceSemanticLabelsFB));
    xrGetInstanceProcAddr(instance, "xrGetSpaceBoundary2DFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceBoundary2DFB));
    xrGetInstanceProcAddr(instance, "xrGetSpaceRoomLayoutFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceRoomLayoutFB));
    xrGetInstanceProcAddr(instance, "xrGetSpaceContainerFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceContainerFB));
    xrGetInstanceProcAddr(instance, "xrRequestSceneCaptureFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrRequestSceneCaptureFB));

    xrGetInstanceProcAddr(instance, "xrSetSpaceBoundingBox2DYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSpaceBoundingBox2DYVR));
    xrGetInstanceProcAddr(instance, "xrSetSpaceBoundingBox3DYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSpaceBoundingBox3DYVR));
    xrGetInstanceProcAddr(instance, "xrSetSpaceSemanticLabelsYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSpaceSemanticLabelsYVR));
    xrGetInstanceProcAddr(instance, "xrSetSpaceBoundary2DYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSpaceBoundary2DYVR));
    xrGetInstanceProcAddr(instance, "xrSetSpaceRoomLayoutYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSpaceRoomLayoutYVR));
    xrGetInstanceProcAddr(instance, "xrSetSpaceContainerYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSpaceContainerYVR));
    xrGetInstanceProcAddr(instance, "xrResetSceneYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrResetSceneYVR));
    xrGetInstanceProcAddr(instance, "xrGetSceneStatusYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSceneStatusYVR));
    xrGetInstanceProcAddr(instance, "xrSetLocateRoomYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetLocateRoomYVR));
    xrGetInstanceProcAddr(instance, "xrGetLocateRoomYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetLocateRoomYVR));
    xrGetInstanceProcAddr(instance, "xrGetSpaceTriangleMeshMETA", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetSpaceTriangleMeshMETA));

    xrGetInstanceProcAddr(instance, "xrGetRecommendedResolution", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetRecommendedResolution));

    xrGetInstanceProcAddr(instance, "xrCreateSwapchainAndroidSurfaceKHR", (PFN_xrVoidFunction*)(&xrCreateSwapchainAndroidSurfaceKhr));

    xrGetInstanceProcAddr(instance, "xrCreateMeshDetectorYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateMeshDetectorYVR));
    xrGetInstanceProcAddr(instance, "xrDestroyMeshDetectorYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyMeshDetectorYVR));
    xrGetInstanceProcAddr(instance, "xrBeginRoomSetupYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrBeginRoomSetupYVR));
    xrGetInstanceProcAddr(instance, "xrEndRoomSetupYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrEndRoomSetupYVR));
    xrGetInstanceProcAddr(instance, "xrSetSpaceTriangleMeshYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSpaceTriangleMeshYVR));

    xrGetInstanceProcAddr(instance, "xrCreatePassthroughProviderYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreatePassthroughProviderYVR));
    xrGetInstanceProcAddr(instance, "xrDestroyPassthroughProviderYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyPassthroughProviderYVR));
    xrGetInstanceProcAddr(instance, "xrStartPassthroughProviderYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrStartPassthroughProviderYVR));
    xrGetInstanceProcAddr(instance, "xrStopPassthroughProviderYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrStopPassthroughProviderYVR));
    xrGetInstanceProcAddr(instance, "xrCreatePassthroughSwapchainYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreatePassthroughSwapchainYVR));
    xrGetInstanceProcAddr(instance, "xrDestroyPassthroughSwapchainYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyPassthroughSwapchainYVR));
    xrGetInstanceProcAddr(instance, "xrEnumeratePassthroughSwapchainImagesYVR",
                          reinterpret_cast<PFN_xrVoidFunction*>(&xrEnumeratePassthroughSwapchainImagesYVR));
    xrGetInstanceProcAddr(instance, "xrAcquirePassthroughImageYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrAcquirePassthroughImageYVR));
    xrGetInstanceProcAddr(instance, "xrBeginBoundaryDetectYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrBeginBoundaryDetectYVR));
    xrGetInstanceProcAddr(instance, "xrEndBoundaryDetectYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrEndBoundaryDetectYVR));

    xrGetInstanceProcAddr(instance, "xrSetLargeSpaceMapRecordEnableYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetLargeSpaceMapRecordEnableYVR));
    xrGetInstanceProcAddr(instance, "xrSetMarkerDetectionEnableYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetMarkerDetectionEnableYVR));
    xrGetInstanceProcAddr(instance, "xrClearAllMarkersYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrClearAllMarkersYVR));
    xrGetInstanceProcAddr(instance, "xrSetLargeSpaceOriginPoseYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetLargeSpaceOriginPoseYVR));
    xrGetInstanceProcAddr(instance, "xrExportLargeSpaceMapYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrExportLargeSpaceMapYVR));
    xrGetInstanceProcAddr(instance, "xrSetRecenterEnableYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetRecenterEnableYVR));
    xrGetInstanceProcAddr(instance, "xrGetRecenterEnableYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetRecenterEnableYVR));
    xrGetInstanceProcAddr(instance, "xrSetLargeSpaceMapAssemblingEnableYVR",
                          reinterpret_cast<PFN_xrVoidFunction*>(&xrSetLargeSpaceMapAssemblingEnableYVR));
    xrGetInstanceProcAddr(instance, "xrGetLargeSpacePointsCalibParamsYVR",
                          reinterpret_cast<PFN_xrVoidFunction*>(&xrGetLargeSpacePointsCalibParamsYVR));
    xrGetInstanceProcAddr(instance, "xrQueryCurrentConnectedControllerTypeYVR",
                          reinterpret_cast<PFN_xrVoidFunction*>(&xrQueryCurrentConnectedControllerTypeYVR));

    xrGetInstanceProcAddr(instance, "xrStartHandCalibrationYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrStartHandCalibrationYVR));
    xrGetInstanceProcAddr(instance, "xrRegisterHandEventCallbackYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrRegisterHandEventCallbackYVR));
    xrGetInstanceProcAddr(instance, "xrUnregisterHandEventCallbackYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrUnregisterHandEventCallbackYVR));
    xrGetInstanceProcAddr(instance, "xrStopHandCalibrationYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrStopHandCalibrationYVR));


    xrGetInstanceProcAddr(instance, "xrSetSlamCalibrationEnableYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetSlamCalibrationEnableYVR));

    xrGetInstanceProcAddr(instance, "xrGetPlaneDetectionsYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetPlaneDetectionsYVR));
    xrGetInstanceProcAddr(instance, "xrGetPlanePolygonBufferYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetPlanePolygonBufferYVR));

    xrGetInstanceProcAddr(instance, "xrGetRenderScaleYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetRenderScaleYVR));
    xrGetInstanceProcAddr(instance, "xrSetRenderScaleYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetRenderScaleYVR));
    xrGetInstanceProcAddr(instance, "xrGetRenderSharpenYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetRenderSharpenYVR));
    xrGetInstanceProcAddr(instance, "xrSetRenderSharpenYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetRenderSharpenYVR));
    xrGetInstanceProcAddr(instance, "xrGetRenderFovYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetRenderFovYVR));
    xrGetInstanceProcAddr(instance, "xrSetRenderFovYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetRenderFovYVR));
    xrGetInstanceProcAddr(instance, "xrResetRenderFovYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrResetRenderFovYVR));
    xrGetInstanceProcAddr(instance, "xrGetColorSpaceConfigYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetColorSpaceConfigYVR));
    xrGetInstanceProcAddr(instance, "xrSetColorSpaceConfigYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetColorSpaceConfigYVR));

    xrGetInstanceProcAddr(instance, "xrConvertTimespecTimeToTimeKHR", reinterpret_cast<PFN_xrVoidFunction*>(&xrConvertTimespecTimeToTimeKHR));

    xrGetInstanceProcAddr(instance, "xrSetImageTrackingYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetImageTrackingYVR));
    xrGetInstanceProcAddr(instance, "xrRegisterImageTemplateYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrRegisterImageTemplateYVR));
    xrGetInstanceProcAddr(instance, "xrUnRegisterImageTemplateYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrUnRegisterImageTemplateYVR));
    xrGetInstanceProcAddr(instance, "xrCreateImageSpaceYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreateImageSpaceYVR));

    xrGetInstanceProcAddr(instance, "xrCreatePassthroughFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreatePassthroughFB));
    xrGetInstanceProcAddr(instance, "xrDestroyPassthroughFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyPassthroughFB));
    xrGetInstanceProcAddr(instance, "xrPassthroughStartFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrPassthroughStartFB));
    xrGetInstanceProcAddr(instance, "xrPassthroughPauseFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrPassthroughPauseFB));
    xrGetInstanceProcAddr(instance, "xrCreatePassthroughLayerFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreatePassthroughLayerFB));
    xrGetInstanceProcAddr(instance, "xrDestroyPassthroughLayerFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyPassthroughLayerFB));
    xrGetInstanceProcAddr(instance, "xrPassthroughLayerPauseFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrPassthroughLayerPauseFB));
    xrGetInstanceProcAddr(instance, "xrPassthroughLayerResumeFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrPassthroughLayerResumeFB));
    xrGetInstanceProcAddr(instance, "xrPassthroughLayerSetStyleFB", reinterpret_cast<PFN_xrVoidFunction*>(&xrPassthroughLayerSetStyleFB));
    xrGetInstanceProcAddr(instance, "xrCreatePassthroughColorLutMETA", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreatePassthroughColorLutMETA));
    xrGetInstanceProcAddr(instance, "xrDestroyPassthroughColorLutMETA", reinterpret_cast<PFN_xrVoidFunction*>(&xrDestroyPassthroughColorLutMETA));
    xrGetInstanceProcAddr(instance, "xrUpdatePassthroughColorLutMETA", reinterpret_cast<PFN_xrVoidFunction*>(&xrUpdatePassthroughColorLutMETA));
    xrGetInstanceProcAddr(instance, "xrCreatePassthroughSwapchainYVR2", reinterpret_cast<PFN_xrVoidFunction*>(&xrCreatePassthroughSwapchainYVR2));

    xrGetInstanceProcAddr(instance, "xrGetDPCompostionStateYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetDPCompositionStateYVR));
    YInfo("Obtained xrGetDPCompostionStateYVR address: %p", (void*)xrGetDPCompositionStateYVR);
    xrGetInstanceProcAddr(instance, "xrStartDPCompostionYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrStartDPCompositionYVR));
    xrGetInstanceProcAddr(instance, "xrStopDPCompostionYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrStopDPCompositionYVR));

    xrGetInstanceProcAddr(instance, "xrSetPersistentPassthroughMeshModeInfoYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetPersistentPassthroughMeshModeInfoYVR));
    xrGetInstanceProcAddr(instance, "xrGetPersistentPassthroughMeshModeInfoYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrGetPersistentPassthroughMeshModeInfoYVR));
    xrGetInstanceProcAddr(instance, "xrSetPersistentPassthroughMeshYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetPersistentPassthroughMeshYVR));
    xrGetInstanceProcAddr(instance, "xrSetPersistentPassthroughMeshTransformYVR", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetPersistentPassthroughMeshTransformYVR));

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
    float ret;
    OpenXRAPI(xrGetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, property, &ret));
    return ret;
}

void OpenXRExtMgr::setFloatProperty(const xrSetSystemPropertyYVR& property, float value)
{
    OpenXRAPI(xrSetSystemPropertyFloatYVR(plugin.openxrMgr->program->session, property, value));
}

int OpenXRExtMgr::getIntProperty(const xrGetSystemPropertyYVR& property)
{
    int ret;
    OpenXRAPI(xrGetSystemPropertyIntYVR(plugin.openxrMgr->program->session, property, &ret));
    return ret;
}

void OpenXRExtMgr::getFloatArrayProperty(const xrGetSystemPropertyYVR& property, float* array, int dataLength)
{
    int outputDataLength = 0;
    xrGetSystemPropertyFloatArrayYVR(plugin.openxrMgr->program->session, property, array, &outputDataLength);
}

void OpenXRExtMgr::getCharArrayProperty(const xrGetSystemPropertyYVR& property, char* array, int dataLength)
{
    int outputDataLength = 0;
    xrGetSystemPropertyCharArrayYVR(plugin.openxrMgr->program->session, property, array, &outputDataLength);
}

bool OpenXRExtMgr::isEnableTargetExt(string ext)
{
    return std::find(enabledExtensions.begin(), enabledExtensions.end(), ext) != enabledExtensions.end();
}
