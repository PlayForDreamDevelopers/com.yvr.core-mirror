#pragma once
#include <string>

#include "../YVRUnityPlugin.h"
#include "../UnityHeader/IUnityProfiler.h"

inline void CreateMarker(const UnityProfilerMarkerDesc** marker, const std::string& name)
{
    if (plugin.developmentBuildMode)
        plugin.unityProfiler->CreateMarker(marker, name.c_str(), kUnityProfilerCategoryOther, kUnityProfilerMarkerFlagDefault, 0);
}

inline void BeginSample(const UnityProfilerMarkerDesc* marker)
{
    if (plugin.developmentBuildMode)
        plugin.unityProfiler->BeginSample(marker);
}

inline void EndSample(const UnityProfilerMarkerDesc* marker)
{
    if (plugin.developmentBuildMode)
        plugin.unityProfiler->EndSample(marker);
}
