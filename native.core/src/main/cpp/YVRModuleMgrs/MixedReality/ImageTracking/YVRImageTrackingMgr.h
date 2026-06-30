#pragma once

#include <string>
#include <unordered_map>

#include "openxr/openxr.h"
#include "pfdm_openxr_preview/openxr_yvr_private.h"
#include "TrackedImageInfo.h"
#include "ToTrackImageInfo.h"

typedef void (*OnImageTrackingUpdate)(TrackedImageInfo trackedImageInfo);

class YVRImageTrackingMgr
{
public:
    void switchImageTracking(const bool enable);
    void registerImageTemplate(const ToTrackImageInfo& toTrackImageInfo);
    void unRegisterImageTemplate(const char* imageId);
    void onImageTracked(const XrEventDataImageTrackingUpdateYVR& trackedImageInfo);
    void setImageTrackingUpdateCallback(OnImageTrackingUpdate callback);

private:
    std::unordered_map<std::string, XrSpace> image2XrSpaces{};
    OnImageTrackingUpdate onImageTrackingUpdateCallback = nullptr;
    const XrSpace& getImageSpace(char* imageId);
};
