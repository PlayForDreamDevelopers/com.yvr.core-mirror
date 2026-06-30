#include "YVRInputMgr.h"
#include "../../YVRUnityPlugin.h"
#include "../../Utilities/YVRTime.h"

YVRInputMgr::YVRInputMgr() = default;
YVRInputMgr::~YVRInputMgr() = default;

void YVRInputMgr::refreshRenderPoseData()
{
    plugin.hmdMgr->refreshHmdPose(true);

    plugin.controllersMgr->refreshControllerPose(true);
    plugin.eyeTrackingMgr->updateEyeTrackingPose(true);
    plugin.handMgr->refreshHandPose(true);
}

void YVRInputMgr::refreshPhysicsPoseData()
{
    // For physics pose, we need to refresh the views first
    // For render pose, the views has been refreshed in Unity populateNextFrameDesc part
    plugin.hmdMgr->refreshViews(false);
    plugin.hmdMgr->refreshHmdPose(false);

    plugin.controllersMgr->refreshControllerPose(false);
}

XrTime YVRInputMgr::getTime(const bool forRender)
{
    XrTime time = forRender ? plugin.renderMgr->frameState.predictedDisplayTime : GetCurrentTime(CLOCK_MONOTONIC);
    if(time<=0) time = GetCurrentTime(CLOCK_MONOTONIC);
    return time;
}

void YVRInputMgr::getCurrentInputDevice(XrInputDeviceFlagsYVR* inputDevice)
{
    OpenXRAPI(xrGetCurrentInputDeviceYVR(plugin.openxrMgr->program->instance, inputDevice));
}

void YVRInputMgr::setTrackingSpace(XrReferenceSpaceType trackingSpace)
{
    AnnounceCallingFunc();
    this->trackingSpace = trackingSpace;
    createAppSpace();
}

XrReferenceSpaceType YVRInputMgr::getTrackingSpace()
{
    AnnounceCallingFunc();
    return this->trackingSpace;
}

void YVRInputMgr::createAppSpace()
{
    if (appSpace != XR_NULL_HANDLE) { OpenXRAPI(xrDestroySpace(appSpace)); }

    XrReferenceSpaceCreateInfo appSpaceCreateInfo{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
    appSpaceCreateInfo.referenceSpaceType = trackingSpace;
    appSpaceCreateInfo.poseInReferenceSpace = XrPosef{0, 0, 0, 1};
    OpenXRAPI(xrCreateReferenceSpace(plugin.openxrMgr->program->session,&appSpaceCreateInfo,&appSpace));
}

void YVRInputMgr::createViewSpace()
{
    XrReferenceSpaceCreateInfo viewSpaceCreateInfo{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
    viewSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_VIEW;
    viewSpaceCreateInfo.poseInReferenceSpace = XrPosef{0, 0, 0, 1};
    OpenXRAPI(xrCreateReferenceSpace(plugin.openxrMgr->program->session,&viewSpaceCreateInfo,&viewSpace));
}

void YVRInputMgr::createLocalSpace()
{
    if (localSpace != XR_NULL_HANDLE) { OpenXRAPI(xrDestroySpace(localSpace)); }
    XrReferenceSpaceCreateInfo localSpaceCreateInfo{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
    localSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_LOCAL;
    localSpaceCreateInfo.poseInReferenceSpace = XrPosef{0, 0, 0, 1};
    OpenXRAPI(xrCreateReferenceSpace(plugin.openxrMgr->program->session,&localSpaceCreateInfo,&localSpace));
}

void YVRInputMgr::createStageSpace()
{
    if (stageSpace != XR_NULL_HANDLE) { OpenXRAPI(xrDestroySpace(stageSpace)); }

    XrReferenceSpaceCreateInfo stageSpaceCreateInfo{XR_TYPE_REFERENCE_SPACE_CREATE_INFO};
    stageSpaceCreateInfo.referenceSpaceType = XR_REFERENCE_SPACE_TYPE_STAGE;
    stageSpaceCreateInfo.poseInReferenceSpace = XrPosef{0, 0, 0, 1};
    OpenXRAPI(xrCreateReferenceSpace(plugin.openxrMgr->program->session,&stageSpaceCreateInfo,&stageSpace));
}

void YVRInputMgr::createActions()
{
    AnnounceCallingFunc();

    plugin.controllersMgr->createActions();
    plugin.eyeTrackingMgr->createActions();

    XrSessionActionSetsAttachInfo attachInfo{XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO};
    const XrActionSet actionSets[] = {plugin.controllersMgr->actionCtx.actionSet, plugin.eyeTrackingMgr->actionCtx.actionSet};
    attachInfo.countActionSets = 2;
    attachInfo.actionSets = actionSets;
    OpenXRAPI(xrAttachSessionActionSets(plugin.openxrMgr->program->session, &attachInfo));

    AnnounceCalledFunc();
}

void YVRInputMgr::refreshActionState()
{
    if (!plugin.lifecycleMgr->isFocusing()) return;

    static std::vector<XrActiveActionSet> activeActionSets{
            {plugin.controllersMgr->actionCtx.actionSet, XR_NULL_PATH},
            {plugin.eyeTrackingMgr->actionCtx.actionSet, XR_NULL_PATH}
        };
    static XrActionsSyncInfo syncInfo{XR_TYPE_ACTIONS_SYNC_INFO, nullptr, 2, &activeActionSets[0]};

    OpenXRAPI(xrSyncActions(plugin.openxrMgr->program->session, &syncInfo));

    plugin.controllersMgr->syncControllerChargeState();
}

void YVRInputMgr::destroyActions()
{
    plugin.controllersMgr->destroyActions();
    plugin.eyeTrackingMgr->destroyActions();
}

void YVRInputMgr::destroyAppSpace()
{
    if (appSpace != XR_NULL_HANDLE)
    {
        AnnounceCallingFunc();
        OpenXRAPI(xrDestroySpace(appSpace));
        appSpace = XR_NULL_HANDLE;
    }
}

void YVRInputMgr::destroyViewSpace()
{
    if (viewSpace != XR_NULL_HANDLE)
    {
        AnnounceCallingFunc();
        OpenXRAPI(xrDestroySpace(viewSpace));
        viewSpace = XR_NULL_HANDLE;
    }
}

void YVRInputMgr::destroyLocalSpace()
{
    if (localSpace != XR_NULL_HANDLE)
    {
        AnnounceCallingFunc();
        OpenXRAPI(xrDestroySpace(localSpace));
        localSpace = XR_NULL_HANDLE;
    }
}

void YVRInputMgr::destroyStageSpace()
{
    if (stageSpace != XR_NULL_HANDLE)
    {
        AnnounceCallingFunc();
        OpenXRAPI(xrDestroySpace(stageSpace));
        stageSpace = XR_NULL_HANDLE;
    }
}
