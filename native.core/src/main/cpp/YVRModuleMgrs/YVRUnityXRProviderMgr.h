#pragma once

#include "../UnityHeader/IUnityInterface.h"
#include "../YVRUnityXR/YVRProviderBase.h"

struct IUnityXRDisplayInterface;
struct IUnityXRInputInterface;
struct IUnityXRMeshInterface;

struct YVRXRUserDefinedSettings;

class YVRInputProvider;
class YVRDisplayProvider;
class YVRMeshingProvider;

struct YVRXRUserDefinedSettings
{
    bool use16BitDepthBuffer;
    bool useMonoscopic;
    bool useLinearColorSpace;
    bool useAppSW;
    bool optimizeBufferDiscards;
    bool enablePassthroughProvider;
    bool autoResolve;
    bool extraRenderPass;
    bool isP3;
    int extraRenderPassDepth;
    short stereoRenderingMode;
    float outerPassRenderScale;
    float innerPassRenderScale;
    float outerExtraPassRenderScale;
    float innerExtraPassRenderScale;
};

class YVRUnityXRProviderMgr
{
public:
    YVRDisplayProvider* displayProvider;
    YVRInputProvider* inputProvider;
    YVRMeshingProvider* meshProvider;
    IUnityInterfaces* unityInterfaces;
    IUnityXRInputInterface* inputInterface;
    IUnityXRDisplayInterface* displayInterface;
    IUnityXRMeshInterface* meshInterface;

    void initialize(IUnityInterfaces* unityInterfaces);
    void setXrUserDefinedSettings(YVRXRUserDefinedSettings* userDefinedSettings);

private:
    UnitySubsystemErrorCode loadDisplaySubsystem();
    UnitySubsystemErrorCode loadTrackingSubsystem();
    UnitySubsystemErrorCode loadMeshSubsystem();
    void initializeTrackingSubsystemLifecycle(UnityLifecycleProvider& trackingLifeCycleProvider);
    void initializeDisplaySubsystemLifecycle(UnityLifecycleProvider& displayLifecycleProvider);
    void initializeMeshSubsystemLifecycle(UnityLifecycleProvider& meshLifecycleProvider);
};
