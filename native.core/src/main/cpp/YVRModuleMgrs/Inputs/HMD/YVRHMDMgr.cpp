#include "YVRHMDMgr.h"
#include "../../../YVRUnityPlugin.h"
#include "../../../Utilities/YVRMath.h"

void YVRHMDMgr::recenterPose()
{
    AnnounceCallingFunc();
    OpenXRAPI(xrSetSystemPropertyIntYVR(plugin.openxrMgr->program->session,YVR_SYS_PROP_SET_DO_RECENTER,1));
}

void YVRHMDMgr::getRecenterOffset(XrPosef* pose)
{
    AnnounceCallingFunc();
    plugin.openxrMgr->extMgr->getFloatArrayProperty(YVR_SYS_PROP_GET_RECENTER_POSE, reinterpret_cast<float*>(pose), 7);
}

void YVRHMDMgr::getRecenterPose(XrPosef* pose)
{
    AnnounceCallingFunc();
    const XrTime predictedDisplayTime = plugin.inputMgr->getTime(true);
    XrSpaceLocation location = {XR_TYPE_SPACE_LOCATION};
    xrLocateSpace(plugin.inputMgr->localSpace, plugin.inputMgr->appSpace, predictedDisplayTime, &location);
    pose->position = location.pose.position;
    pose->orientation = location.pose.orientation;
}

void YVRHMDMgr::getStageSpacePose(XrPosef* pose)
{
    AnnounceCallingFunc();
    const XrTime predictedDisplayTime = plugin.inputMgr->getTime(true);
    XrSpaceLocation location = {XR_TYPE_SPACE_LOCATION};
    xrLocateSpace(plugin.inputMgr->stageSpace, plugin.inputMgr->appSpace, predictedDisplayTime, &location);
    pose->position = location.pose.position;
    pose->orientation = location.pose.orientation;
}

bool YVRHMDMgr::isUserPresent()
{
    return plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_HMD_MOUNT_STATUS) == 1;
}

void YVRHMDMgr::refreshViews(const bool forRender)
{
    static const bool isUsingQuadViews = plugin.renderMgr->configsMgr->isUsingQuadViews();
    static const XrViewConfigurationType viewType = isUsingQuadViews ? XR_VIEW_CONFIGURATION_TYPE_PRIMARY_QUAD_VARJO
                                                        : XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;

    static constexpr XrViewLocateFoveatedRenderingVARJO viewLocateFoveatedRendering{XR_TYPE_VIEW_LOCATE_FOVEATED_RENDERING_VARJO, nullptr, true};

    static XrViewLocateInfo viewLocateInfo{XR_TYPE_VIEW_LOCATE_INFO,
                                           isUsingQuadViews ? &viewLocateFoveatedRendering : nullptr,
                                           viewType};

    std::vector<XrView>& views = forRender ? renderViews : physicalViews;

    viewLocateInfo.displayTime = plugin.inputMgr->getTime(forRender);
    viewLocateInfo.space = plugin.inputMgr->viewSpace;

    const int viewsCount = plugin.renderMgr->configsMgr->getViewsCount();
    uint32_t viewCountOutput = viewsCount;
    views.resize(viewsCount);

    XrViewState viewState{XR_TYPE_VIEW_STATE};

    OpenXRAPI(xrLocateViews(plugin.openxrMgr->program->session, &viewLocateInfo, &viewState, viewsCount, &viewCountOutput, views.data()));
}

void YVRHMDMgr::refreshHmdPose(const bool forRender)
{
    if (!plugin.lifecycleMgr->isInVrMode() || !plugin.renderMgr->frameBegan)
        return;

    XrSpaceLocation* const hmdPose = forRender ? &hmdRenderPose.location : &hmdPhysicsPose.location;
    XrPosef& leftEyePose = forRender ? lEyeRenderPose : lEyePhysicsPose;
    XrPosef& rightEyePose = forRender ? rEyeRenderPose : rEyePhysicsPose;
    const XrPosef& leftEyeOffset = forRender ? renderViews[0].pose : physicalViews[0].pose;
    const XrPosef& rightEyeOffset = forRender ? renderViews[1].pose : physicalViews[1].pose;

    const XrTime predictedDisplayTime = plugin.inputMgr->getTime(forRender);

    xrLocateSpace(plugin.inputMgr->viewSpace, plugin.inputMgr->appSpace, predictedDisplayTime, hmdPose);

    const XrPosef& centerEyeWorldPose = hmdPose->pose;

    leftEyePose = XrPosef_Multiply(centerEyeWorldPose, leftEyeOffset);
    rightEyePose = XrPosef_Multiply(centerEyeWorldPose, rightEyeOffset);
}

XrPosef YVRHMDMgr::getPredictedTimePose(int64_t time)
{
    XrSpaceLocation hmdPose;
    xrLocateSpace(plugin.inputMgr->viewSpace, plugin.inputMgr->appSpace, time, &hmdPose);
    return hmdPose.pose;
}
