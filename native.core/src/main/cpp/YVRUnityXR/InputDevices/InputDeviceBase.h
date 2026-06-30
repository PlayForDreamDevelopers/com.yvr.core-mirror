#pragma once
#include <unordered_map>
#include <utility>
#include <vector>

#include "../../UnityHeader/XR/IUnityXRInput.h"

class InputDeviceBase
{
public:
    static UnitySubsystemHandle unitySystemHandle;
    virtual ~InputDeviceBase() = default;
    InputDeviceBase();
    virtual void fillDeviceDefinition(UnityXRInputDeviceDefinition* definition) = 0;
    virtual void updateDeviceState(UnityXRInputDeviceState* state, UnityXRInputUpdateType updateType) = 0;
    const std::vector<std::pair<UnityXRInputFeatureUsage, UnityXRInputFeatureType>>& GetRequiredUsages() const;

protected:
    IUnityXRInputInterface* unityXRInputInterface = nullptr;
    std::vector<std::pair<UnityXRInputFeatureUsage, UnityXRInputFeatureType>> requiredUsages{};
    std::unordered_map<UnityXRInputFeatureUsage, UnityXRInputFeatureIndex> usage2IndexMap{};
};
