#pragma once

#include <functional>

#include "../UnityHeader/XR/IUnityXRInput.h"
#include "YVRProviderBase.h"
#include "../UnityHeader/IUnityProfiler.h"
#include "InputDevices/InputDeviceBase.h"

struct DeviceCreationInfo
{
    bool wasConnected;
    std::function<bool()> isConnected;
    std::function<int()> getDeviceId;
    std::function<InputDeviceBase*()> getDevice;
};

class YVRInputProvider : YVRProviderBase
{
public:
    YVRInputProvider(YVRUnityXRProviderMgr& context, UnitySubsystemHandle handle);
    ~YVRInputProvider() override;

    UnitySubsystemErrorCode initialize() override;
    UnitySubsystemErrorCode start() override;
    void stop() override;
    void shutDown() override;

private:
    std::unordered_map<UnityXRInputFeatureUsage, UnityXRInputFeatureIndex> hmdUsage2IndexMap{};
    std::unordered_map<UnityXRInputFeatureUsage, UnityXRInputFeatureIndex> eyesUsage2IndexMap{};
    std::unordered_map<UnityXRInputFeatureUsage, UnityXRInputFeatureIndex> interactionEyesUsage2IndexMap{};

    std::vector<DeviceCreationInfo> deviceCreationInfos{};
    std::unordered_map<int, InputDeviceBase*> connectedDevices{};

    void initializeUnityTrackingLifecycle(UnityXRInputProvider& unityTrackingProvider);

    UnitySubsystemErrorCode onTick(UnityXRInputUpdateType updateType);
    UnitySubsystemErrorCode onFillDeviceDefinition(UnityXRInternalInputDeviceId deviceId, UnityXRInputDeviceDefinition* definition);
    UnitySubsystemErrorCode onUpdateDevicesState(UnityXRInternalInputDeviceId deviceId, UnityXRInputUpdateType updateType,
                                                 UnityXRInputDeviceState* state);

    bool startNeverCalled = true;


    const UnityProfilerMarkerDesc* dynamicTickMarker = nullptr;
    const UnityProfilerMarkerDesc* beforeRenderTickMarker = nullptr;
    const UnityProfilerMarkerDesc* dynamicDeviceUpdateStateMarker = nullptr;
    const UnityProfilerMarkerDesc* beforeRenderDeviceUpdateStateMarker = nullptr;
};
