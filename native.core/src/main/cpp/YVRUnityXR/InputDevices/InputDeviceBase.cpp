#include "InputDeviceBase.h"
#include "../../YVRUnityPlugin.h"

InputDeviceBase::InputDeviceBase()
{
    unityXRInputInterface = plugin.unityXRProviderMgr->inputInterface;
}


const std::vector<std::pair<UnityXRInputFeatureUsage, UnityXRInputFeatureType>>& InputDeviceBase::GetRequiredUsages() const
{
    return requiredUsages;
}
