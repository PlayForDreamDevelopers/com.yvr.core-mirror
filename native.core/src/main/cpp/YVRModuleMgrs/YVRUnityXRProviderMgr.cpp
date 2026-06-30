#include "YVRUnityXRProviderMgr.h"

#include "../UnityHeader/IUnityGraphicsVulkan.h"
#include "../YVRUnityXR/YVRDisplayProvider.h"
#include "../YVRUnityXR/YVRInputProvider.h"
#include "../YVRUnityXR/YVRMeshingProvider.h"
#include "../YVRUnityXR/InputDevices/InputDeviceBase.h"


void YVRUnityXRProviderMgr::initialize(IUnityInterfaces* unityInterfaces)
{
    this->unityInterfaces = unityInterfaces;
    UnityXRAPI(plugin.unityXRProviderMgr->loadDisplaySubsystem());
    UnityXRAPI(plugin.unityXRProviderMgr->loadTrackingSubsystem());
    UnityXRAPI(plugin.unityXRProviderMgr->loadMeshSubsystem());
}

void YVRUnityXRProviderMgr::initializeDisplaySubsystemLifecycle(UnityLifecycleProvider& displayLifecycleProvider)
{
    AnnounceCallingFunc();
    displayLifecycleProvider.Initialize = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode
    {
        YVRUnityXRProviderMgr& providerMgr = GetYVRProviderMgr(userData);

        providerMgr.displayProvider = new YVRDisplayProvider(providerMgr, handle);
        return providerMgr.displayProvider->initialize();
    };

    displayLifecycleProvider.Start = [](UnitySubsystemHandle, void* userData) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& providerMgr = GetYVRProviderMgr(userData);
        return providerMgr.displayProvider->start();
    };

    displayLifecycleProvider.Stop = [](UnitySubsystemHandle, void* userData) -> void
    {
        const YVRUnityXRProviderMgr& providerMgr = GetYVRProviderMgr(userData);
        return providerMgr.displayProvider->stop();
    };

    displayLifecycleProvider.Shutdown = [](UnitySubsystemHandle, void* userData) -> void
    {
        const YVRUnityXRProviderMgr& providerMgr = GetYVRProviderMgr(userData);
        return providerMgr.displayProvider->shutDown();
    };
}

void YVRUnityXRProviderMgr::initializeTrackingSubsystemLifecycle(UnityLifecycleProvider& trackingLifeCycleProvider)
{
    AnnounceCallingFunc();
    trackingLifeCycleProvider.Initialize = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode
    {
        YVRUnityXRProviderMgr& providerMgr = GetYVRProviderMgr(userData);
        providerMgr.inputProvider = new YVRInputProvider(providerMgr, handle);
        return providerMgr.inputProvider->initialize();
    };

    trackingLifeCycleProvider.Start = [](UnitySubsystemHandle, void* userData) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& providerMgr = GetYVRProviderMgr(userData);
        return providerMgr.inputProvider->start();
    };

    trackingLifeCycleProvider.Stop = [](UnitySubsystemHandle, void* userData) -> void
    {
        const YVRUnityXRProviderMgr& providerMgr = GetYVRProviderMgr(userData);
        return providerMgr.inputProvider->stop();
    };

    trackingLifeCycleProvider.Shutdown = [](UnitySubsystemHandle, void* userData) -> void
    {
        const YVRUnityXRProviderMgr& providerMgr = GetYVRProviderMgr(userData);
        return providerMgr.inputProvider->shutDown();
    };
}

void YVRUnityXRProviderMgr::initializeMeshSubsystemLifecycle(UnityLifecycleProvider& meshLifeCycleProvider)
{
    AnnounceCallingFunc();
    meshLifeCycleProvider.Initialize = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode
    {
        YVRUnityXRProviderMgr& providerContext = GetYVRProviderMgr(userData);

        providerContext.meshProvider = new YVRMeshingProvider(providerContext, handle);
        YInfo("new meshing provider");
        return providerContext.meshProvider->initialize();
    };

    meshLifeCycleProvider.Start = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& providerContext = GetYVRProviderMgr(userData);
        YInfo("new meshing provider start");
        return providerContext.meshProvider->start();
    };

    meshLifeCycleProvider.Stop = [](UnitySubsystemHandle handle, void* userData) -> void
    {
        const YVRUnityXRProviderMgr& providerContext = GetYVRProviderMgr(userData);
        return providerContext.meshProvider->stop();
    };

    meshLifeCycleProvider.Shutdown = [](UnitySubsystemHandle handle, void* userData) -> void
    {
        const YVRUnityXRProviderMgr& providerContext = GetYVRProviderMgr(userData);
        return providerContext.meshProvider->shutDown();
    };
}

UnitySubsystemErrorCode YVRUnityXRProviderMgr::loadDisplaySubsystem()
{
    AnnounceCallingFunc();
    this->displayInterface = this->unityInterfaces->Get<IUnityXRDisplayInterface>();
    if (this->displayInterface == nullptr) return kUnitySubsystemErrorCodeFailure;

    UnityLifecycleProvider displayLifeCycleProvider{};
    displayLifeCycleProvider.userData = this;
    initializeDisplaySubsystemLifecycle(displayLifeCycleProvider);

    return this->displayInterface->RegisterLifecycleProvider("YVR XR SDK", "Display", &displayLifeCycleProvider);
}

UnitySubsystemErrorCode YVRUnityXRProviderMgr::loadTrackingSubsystem()
{
    AnnounceCallingFunc();
    this->inputInterface = this->unityInterfaces->Get<IUnityXRInputInterface>();
    if (this->inputInterface == nullptr) return kUnitySubsystemErrorCodeFailure;

    UnityLifecycleProvider trackingLifeCycleProvider{};
    trackingLifeCycleProvider.userData = this;
    initializeTrackingSubsystemLifecycle(trackingLifeCycleProvider);

    return this->inputInterface->RegisterLifecycleProvider("YVR XR SDK", "Tracking", &trackingLifeCycleProvider);
}


UnitySubsystemErrorCode YVRUnityXRProviderMgr::loadMeshSubsystem()
{
    AnnounceCallingFunc();
    this->meshInterface = this->unityInterfaces->Get<IUnityXRMeshInterface>();
    if (this->meshInterface == nullptr) return kUnitySubsystemErrorCodeFailure;

    UnityLifecycleProvider meshLifeCycleProvider{};
    meshLifeCycleProvider.userData = this;
    initializeMeshSubsystemLifecycle(meshLifeCycleProvider);
    const UnitySubsystemErrorCode result = this->meshInterface->RegisterLifecycleProvider("YVR XR SDK", "Meshing", &meshLifeCycleProvider);
    YInfo("UnitySubsystemErrorCode result %d", result);
    return result;
}

void YVRUnityXRProviderMgr::setXrUserDefinedSettings(YVRXRUserDefinedSettings* userDefinedSettings)
{
    plugin.renderMgr->configsMgr->setMonoscopic(userDefinedSettings->useMonoscopic);
    plugin.renderMgr->configsMgr->seStereoRenderMode(static_cast<YVRStereoRenderMode>(userDefinedSettings->stereoRenderingMode));
    plugin.renderMgr->configsMgr->setResolutionScale(1.0);
    plugin.renderMgr->configsMgr->setDepthBufferSize(userDefinedSettings->use16BitDepthBuffer ? Bit_16 : Bit_24);
    plugin.renderMgr->configsMgr->setP3ColorSpace(userDefinedSettings->isP3);

    // if using gamma space, the render texture type should be linear
    // if using linear space, the render texture type should be sRGB
    plugin.renderMgr->configsMgr->setColorSpace(userDefinedSettings->useLinearColorSpace ? XR_COLOR_SPACE_SRGB_YVR
                                                    : XR_COLOR_SPACE_Linear_YVR);

    plugin.renderMgr->configsMgr->setAppSWEnable(userDefinedSettings->useAppSW);
    plugin.renderMgr->configsMgr->setOptimizeBufferDiscards(userDefinedSettings->optimizeBufferDiscards);
    plugin.renderMgr->configsMgr->setPassthroughProviderEnable(userDefinedSettings->enablePassthroughProvider);
    plugin.renderMgr->configsMgr->setAutoResolve(userDefinedSettings->autoResolve);
    plugin.renderMgr->configsMgr->setOuterPassResolutionScale(userDefinedSettings->outerPassRenderScale);
    plugin.renderMgr->configsMgr->setInnerPassResolutionScale(userDefinedSettings->innerPassRenderScale);
    plugin.renderMgr->configsMgr->setOuterExtraPassResolutionScale(userDefinedSettings->outerExtraPassRenderScale);
    plugin.renderMgr->configsMgr->setInnerExtraPassResolutionScale(userDefinedSettings->innerExtraPassRenderScale);
    plugin.renderMgr->configsMgr->setExtraRenderPass(userDefinedSettings->extraRenderPass);
    plugin.renderMgr->configsMgr->setExtraRenderPassDepth(userDefinedSettings->extraRenderPassDepth);
}
