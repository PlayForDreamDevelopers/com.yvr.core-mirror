#include "YVRImageTrackingMgr.h"
#include "../../../YVRUnityPlugin.h"
#include "../../../Utilities/YVRCoordinateConverter.h"

void YVRImageTrackingMgr::switchImageTracking(const bool enable)
{
    AnnounceCallingFunc();
    xrSetImageTrackingYVR(plugin.openxrMgr->program->session, enable ? XR_TRUE : XR_FALSE);
    AnnounceCalledFunc();
}

void YVRImageTrackingMgr::registerImageTemplate(const ToTrackImageInfo& toTrackImageInfo)
{
    AnnounceCallingFunc();

    XrImageTemplateInfoYVR imageTemplateInfo{XR_TYPE_IMAGE_TEMPLATE_INFO_YVR, nullptr};
    imageTemplateInfo.physicalWidth = toTrackImageInfo.physicalWidth;
    imageTemplateInfo.physicalHeight = toTrackImageInfo.physicalHeight;
    imageTemplateInfo.imageId = strdup(toTrackImageInfo.imageId);
    imageTemplateInfo.templatePath = strdup(toTrackImageInfo.templatePath);
    YDebug("register id %s and path is %s", imageTemplateInfo.imageId, imageTemplateInfo.templatePath);
    OpenXRAPI(xrRegisterImageTemplateYVR(plugin.openxrMgr->program->session, imageTemplateInfo));
    AnnounceCalledFunc();
}

void YVRImageTrackingMgr::unRegisterImageTemplate(const char* imageId)
{
    YDebug("Unregister id is %s", imageId);
    image2XrSpaces.erase(imageId);

    OpenXRAPI(xrUnRegisterImageTemplateYVR(plugin.openxrMgr->program->session, const_cast<char*>(imageId)));
}


void YVRImageTrackingMgr::onImageTracked(const XrEventDataImageTrackingUpdateYVR& trackedImageInfo)
{
    if (onImageTrackingUpdateCallback == nullptr) { return; }

    TrackedImageInfo info{};
    info.isActive = trackedImageInfo.isActive;
    info.confidence = trackedImageInfo.confidence;
    info.size = trackedImageInfo.size;

    info.time = trackedImageInfo.time;
    strcpy(info.imageId, trackedImageInfo.imageId);

    if (info.isActive)
    {
        const XrSpace& space = getImageSpace(info.imageId);

        XrSpaceLocation location{XR_TYPE_SPACE_LOCATION};
        xrLocateSpace(space, plugin.inputMgr->appSpace, plugin.inputMgr->getTime(true), &location);
        info.pose = ConvertPoseBetweenOpenXRandUnity(location.pose);
        onImageTrackingUpdateCallback(info);
    }
}

const XrSpace& YVRImageTrackingMgr::getImageSpace(char* imageId)
{
    AnnounceCallingFunc();
    const auto imageIdStr = string(imageId);
    if (image2XrSpaces.find(imageIdStr) != image2XrSpaces.end())
    {
        return image2XrSpaces[imageIdStr];
    }

    image2XrSpaces[imageIdStr] = {};
    XrImageSpaceCreateInfoYVR createInfo{XR_TYPE_IMAGE_SPACE_CREATE_INFO_YVR, nullptr};
    createInfo.imageId = imageId;
    createInfo.poseInImageSpace = {0, 0, 0, 1, 0, 0, 0};

    xrCreateImageSpaceYVR(plugin.openxrMgr->program->session, &createInfo, &image2XrSpaces[imageIdStr]);
    AnnounceCalledFunc();
    return image2XrSpaces[imageIdStr];
}


void YVRImageTrackingMgr::setImageTrackingUpdateCallback(OnImageTrackingUpdate callback)
{
    AnnounceCallingFunc();
    this->onImageTrackingUpdateCallback = callback;
    AnnounceCalledFunc();
}
