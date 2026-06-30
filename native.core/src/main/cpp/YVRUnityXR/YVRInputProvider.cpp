// ReSharper disable CppInconsistentNaming
#include "../YVRUnityPlugin.h"
#include "../Utilities/YVRProfiler.h"
#include "../Utilities/YVRCoordinateConverter.h"
#include "../Utilities/YVRTypeConverter.h"
#include "YVRInputProvider.h"
#include "InputDevices/ControllerInputDevice.h"
#include "InputDevices/HMDInputDevice.h"
#include "InputDevices/EyeTrackingInputDevice.h"

YVRInputProvider::YVRInputProvider(YVRUnityXRProviderMgr& context, UnitySubsystemHandle handle)
    : YVRProviderBase(context, handle)
{
    deviceCreationInfos = {
            {false, &ControllerInputDevice::isLeftControllerConnected, &ControllerInputDevice::getLeftControllerId,
             &ControllerInputDevice::getLeftControllerDevice}, // Left Controller
            {false, &ControllerInputDevice::isRightControllerConnected, &ControllerInputDevice::getRightControllerId,
             &ControllerInputDevice::getRightControllerDevice}, // Right Controller
            {false, &HMDInputDevice::isHMDConnected, &HMDInputDevice::getHMDId, &HMDInputDevice::getHMDDevice}, // Hmd
            {false, &EyeTrackingInputDevice::isEyeTrackingConnected, &EyeTrackingInputDevice::getEyeTrackingId,
             &EyeTrackingInputDevice::getEyeTrackingDevice} // EyeTracking
        };
}

UnitySubsystemErrorCode YVRInputProvider::initialize()
{
    AnnounceCallingFunc();

    CreateMarker(&dynamicTickMarker, "NativeCore-DynamicTick");
    CreateMarker(&beforeRenderTickMarker, "NativeCore-BeforeRenderTick");
    CreateMarker(&dynamicDeviceUpdateStateMarker, "NativeCore-DynamicDeviceUpdateState");
    CreateMarker(&beforeRenderDeviceUpdateStateMarker, "NativeCore-BeforeRenderDeviceUpdateStateMarker");

    UnityXRInputProvider unityTrackingProvider{&m_Context};
    initializeUnityTrackingLifecycle(unityTrackingProvider);

    UnityXRAPI(m_Context.inputInterface->RegisterInputProvider(m_SystemHandle, &unityTrackingProvider));

    return kUnitySubsystemErrorCodeSuccess;
}

void YVRInputProvider::initializeUnityTrackingLifecycle(UnityXRInputProvider& unityTrackingProvider)
{
    AnnounceCallingFunc();

    unityTrackingProvider.Tick = [](UnitySubsystemHandle, void* userData, const UnityXRInputUpdateType updateType) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& unityXrProviderMgr = GetYVRProviderMgr(userData);
        return unityXrProviderMgr.inputProvider->onTick(updateType);
    };

    unityTrackingProvider.FillDeviceDefinition = [](UnitySubsystemHandle, void* userData, const UnityXRInternalInputDeviceId deviceId,
                                                    UnityXRInputDeviceDefinition* definition) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& unityXrProviderMgr = GetYVRProviderMgr(userData);
        return unityXrProviderMgr.inputProvider->onFillDeviceDefinition(deviceId, definition);
    };

    unityTrackingProvider.HandleEvent = [](UnitySubsystemHandle, void*, unsigned int, UnityXRInternalInputDeviceId,
                                           void*, unsigned int) -> UnitySubsystemErrorCode
    {
        return kUnitySubsystemErrorCodeFailure;
    };

    unityTrackingProvider.UpdateDeviceState = [](UnitySubsystemHandle, void* userData, const UnityXRInternalInputDeviceId deviceId,
                                                 const UnityXRInputUpdateType updateType, UnityXRInputDeviceState* state) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& unityXrProviderMgr = GetYVRProviderMgr(userData);
        return unityXrProviderMgr.inputProvider->onUpdateDevicesState(deviceId, updateType, state);
    };

    unityTrackingProvider.HandleHapticImpulse = [](UnitySubsystemHandle, void*, const UnityXRInternalInputDeviceId deviceId, int,
                                                   const float amplitude, const float duration) -> UnitySubsystemErrorCode
    {
        plugin.controllersMgr->setControllerVibration(deviceId == device_LEFT_CONTROLLER, amplitude, -1, static_cast<int>(duration * 1000 * 1000 * 1000));
        return kUnitySubsystemErrorCodeSuccess;
    };

    unityTrackingProvider.QueryHapticCapabilities = [](UnitySubsystemHandle, void*, UnityXRInternalInputDeviceId device_id,
                                                       UnityXRHapticCapabilities* capabilities) -> UnitySubsystemErrorCode
    {
        *capabilities = {1, true, false, 1, 0, 0};
        return kUnitySubsystemErrorCodeSuccess;
    };

    unityTrackingProvider.HandleHapticStop = [](UnitySubsystemHandle, void*,
                                                const UnityXRInternalInputDeviceId deviceId) -> UnitySubsystemErrorCode
    {
        plugin.controllersMgr->stopControllerVibration(deviceId == device_LEFT_CONTROLLER);
        return kUnitySubsystemErrorCodeSuccess;
    };

    unityTrackingProvider.HandleSetTrackingOriginMode = [](const UnitySubsystemHandle handle, void* userData,
                                                           const UnityXRInputTrackingOriginModeFlags trackingOriginMode) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& unityXrProviderMgr = GetYVRProviderMgr(userData);

        plugin.inputMgr->setTrackingSpace(UnityTrackingMode2OpenXR(trackingOriginMode));
        const IUnityXRInputInterface unityInputInterface = *unityXrProviderMgr.inputInterface;
        unityInputInterface.InputSubsystem_TrackingOriginUpdated(handle);
        return kUnitySubsystemErrorCodeSuccess;
    };

    unityTrackingProvider.QueryTrackingOriginMode = [](UnitySubsystemHandle, void*,
                                                       UnityXRInputTrackingOriginModeFlags* trackingOriginMode) -> UnitySubsystemErrorCode
    {
        const XrReferenceSpaceType trackingSpace = plugin.inputMgr->getTrackingSpace();
        *trackingOriginMode = OpenXRTrackingMode2Unity(trackingSpace);

        return kUnitySubsystemErrorCodeSuccess;
    };

    unityTrackingProvider.QuerySupportedTrackingOriginModes = [](UnitySubsystemHandle, void*,
                                                                 UnityXRInputTrackingOriginModeFlags* supportedModes) -> UnitySubsystemErrorCode
    {
        *supportedModes = static_cast<UnityXRInputTrackingOriginModeFlags>(
            kUnityXRInputTrackingOriginModeUnknown | kUnityXRInputTrackingOriginModeDevice |
            kUnityXRInputTrackingOriginModeFloor | kUnityXRInputTrackingOriginModeTrackingReference);
        return kUnitySubsystemErrorCodeSuccess;
    };

    unityTrackingProvider.HandleRecenter = [](UnitySubsystemHandle, void*) -> UnitySubsystemErrorCode
    {
        plugin.hmdMgr->recenterPose();
        return kUnitySubsystemErrorCodeSuccess;
    };
}

UnitySubsystemErrorCode YVRInputProvider::start()
{
    AnnounceCallingFunc();

    if (startNeverCalled)
    {
        startNeverCalled = false;
        plugin.lifecycleMgr->onUnityMainThreadStart();
    }

    plugin.lifecycleMgr->onUnityMainThreadResume();
    return kUnitySubsystemErrorCodeSuccess;
}

void YVRInputProvider::stop()
{
    AnnounceCallingFunc();
    // We do not disconnect the HMD device here, as this will invalidate the Input System Record mechanism (for unknown reasons)
    plugin.lifecycleMgr->onUnityMainThreadPause();
}

void YVRInputProvider::shutDown() { plugin.lifecycleMgr->onUnityMainThreadStop(); }

UnitySubsystemErrorCode YVRInputProvider::onTick(const UnityXRInputUpdateType updateType)
{
    const bool isDynamicUpdate = updateType == kUnityXRInputUpdateTypeDynamic;
    BeginSample(isDynamicUpdate ? dynamicTickMarker : beforeRenderTickMarker);

    if (isDynamicUpdate)
    {
        plugin.lifecycleMgr->onUnityDynamicTick();
        plugin.inputMgr->refreshActionState();
        plugin.inputMgr->refreshPhysicsPoseData();

        for (auto& deviceCreationInfo : deviceCreationInfos)
        {
            if (!deviceCreationInfo.wasConnected && deviceCreationInfo.isConnected())
            {
                const int deviceId = deviceCreationInfo.getDeviceId();
                VRAPI(m_Context.inputInterface->InputSubsystem_DeviceConnected(m_SystemHandle, deviceId));
                connectedDevices[deviceId] = deviceCreationInfo.getDevice();

                deviceCreationInfo.wasConnected = true;
            }

            if (deviceCreationInfo.wasConnected && !deviceCreationInfo.isConnected())
            {
                const int deviceId = deviceCreationInfo.getDeviceId();
                VRAPI(m_Context.inputInterface->InputSubsystem_DeviceDisconnected(m_SystemHandle, deviceId));
                const auto device = connectedDevices[deviceId];
                connectedDevices.erase(deviceId);
                delete device;
                deviceCreationInfo.wasConnected = false;
            }
        }
    }
    else
    {
        plugin.lifecycleMgr->onUnityBeforeRenderTick();
        plugin.inputMgr->refreshRenderPoseData();
    }

    EndSample(isDynamicUpdate ? dynamicTickMarker : beforeRenderTickMarker);

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::onFillDeviceDefinition(UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceDefinition* definition)
{
    AnnounceCallingFunc();

    if (connectedDevices.find(deviceId) != connectedDevices.end())
        connectedDevices[deviceId]->fillDeviceDefinition(definition);

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRInputProvider::onUpdateDevicesState(UnityXRInternalInputDeviceId deviceId, UnityXRInputUpdateType updateType,
                                                               UnityXRInputDeviceState* state)
{
    if (plugin.inputMgr->isBlockingInteractionData) return kUnitySubsystemErrorCodeSuccess;

    const bool isDynamic = updateType == kUnityXRInputUpdateTypeDynamic;
    BeginSample(isDynamic ? dynamicDeviceUpdateStateMarker : beforeRenderDeviceUpdateStateMarker);

    if (connectedDevices.find(deviceId) != connectedDevices.end())
        connectedDevices[deviceId]->updateDeviceState(state, updateType);

    EndSample(isDynamic ? dynamicDeviceUpdateStateMarker : beforeRenderDeviceUpdateStateMarker);
    return kUnitySubsystemErrorCodeSuccess;
}

YVRInputProvider::~YVRInputProvider() = default;
