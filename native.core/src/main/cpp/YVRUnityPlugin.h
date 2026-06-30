#pragma once

#include "UnityHeader/IUnityProfiler.h"

#include "YVRModuleMgrs/Render/RenderTasks/GfxTasksMgr.h"
#include "YVRModuleMgrs/Render/YVRRenderMgr.h"
#include "YVRModuleMgrs/Events/YVREventsMgr.h"
#include "YVRModuleMgrs/YVRLifecycleMgr.h"
#include "YVRModuleMgrs/YVROpenXRMgr.h"
#include "YVRModuleMgrs/Inputs/YVRInputMgr.h"
#include "YVRModuleMgrs/YVRUnityXRProviderMgr.h"
#include "YVRModuleMgrs/Inputs/Hands/YVRHandMgr.h"
#include "YVRModuleMgrs/Inputs/Hands/YVRHandCalibrationMgr.h"
#include "YVRModuleMgrs/YVRSpatialAnchorMgr.h"
#include "YVRModuleMgrs/YVRPlaneDetectionMgr.h"
#include "YVRModuleMgrs/YVRSceneMgr.h"
#include "YVRModuleMgrs/YVRMeshMgr.h"
#include "YVRModuleMgrs/YVRPassthroughProviderMgr.h"
#include "YVRModuleMgrs/YVRPassthroughMeshMgr.h"
#include "YVRModuleMgrs/YVRLargeSpatialMgr.h"
#include "YVRModuleMgrs/Inputs/Controllers/YVRControllersMgr.h"
#include "YVRModuleMgrs/Inputs/HMD/YVRHMDMgr.h"
#include "YVRModuleMgrs/Inputs/EyeTracking/YVREyeTrackingMgr.h"
#include "YVRModuleMgrs/MixedReality/ImageTracking/YVRImageTrackingMgr.h"

class YVRUnityPlugin
{
public:
    bool developmentBuildMode;
    YVRControllersMgr* controllersMgr;
    YVRHMDMgr* hmdMgr;

    YVRInputMgr* inputMgr;
    YVRRenderingMgr* renderMgr;
    YVRLifecycleMgr* lifecycleMgr;
    YVREventsMgr* eventsMgr;
    YVRUnityXRProviderMgr* unityXRProviderMgr;
    GfxTasksMgr* gfxTasksMgr;
    YVROpenXRMgr* openxrMgr;
    YVRHandMgr* handMgr;
    YVRSpatialAnchorMgr* spatialAnchorMgr;
    YVRPlaneDetectionMgr* planeDetectionMgr;
    YVREyeTrackingMgr* eyeTrackingMgr;
    YVRSceneMgr* sceneMgr;
    YVRMeshMgr* meshMgr;
    YVRPassthroughProviderMgr* passThroughProviderMgr;
    YVRPassthroughMeshMgr* passthroughMeshMgr;
    YVRLargeSpatialMgr* largeSpatialMgr;
    YVRHandCalibrationMgr* handCalibrationMgr;
    YVRImageTrackingMgr* imageTrackingMgr;

    jobject activity{};
    JavaVM* javaVm{};
    IUnityProfiler* unityProfiler = nullptr;

    YVRUnityPlugin()
    {
        developmentBuildMode = false;
        controllersMgr = new YVRControllersMgr();
        hmdMgr = new YVRHMDMgr();
        inputMgr = new YVRInputMgr();
        renderMgr = new YVRRenderingMgr();
        lifecycleMgr = new YVRLifecycleMgr();
        eventsMgr = new YVREventsMgr();
        unityXRProviderMgr = new YVRUnityXRProviderMgr();
        gfxTasksMgr = new GfxTasksMgr();
        openxrMgr = new YVROpenXRMgr();
        handMgr = new YVRHandMgr();
        spatialAnchorMgr = new YVRSpatialAnchorMgr();
        planeDetectionMgr = new YVRPlaneDetectionMgr;
        eyeTrackingMgr = new YVREyeTrackingMgr();
        sceneMgr = new YVRSceneMgr();
        meshMgr = new YVRMeshMgr();
        passThroughProviderMgr = new YVRPassthroughProviderMgr();
        passthroughMeshMgr = new YVRPassthroughMeshMgr();
        largeSpatialMgr = new YVRLargeSpatialMgr();
        handCalibrationMgr = new YVRHandCalibrationMgr();
        imageTrackingMgr = new YVRImageTrackingMgr;
    }
};

extern YVRUnityPlugin plugin; // Global plugin variable
