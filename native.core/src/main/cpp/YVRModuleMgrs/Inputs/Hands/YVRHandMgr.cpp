#include "YVRHandMgr.h"

#include "openxr_yvr_private.h"
#include "../../../Utilities/YVRLog.h"
#include "../../../YVRUnityPlugin.h"

YVRHandMgr::YVRHandMgr() = default;
YVRHandMgr::~YVRHandMgr()
{
    if (handTrackerL != XR_NULL_HANDLE)
    {
        OpenXRAPI(xrDestroyHandTrackerEXT(handTrackerL));
        handTrackerL = XR_NULL_HANDLE;
    }
    if (handTrackerR != XR_NULL_HANDLE)
    {
        OpenXRAPI(xrDestroyHandTrackerEXT(handTrackerR));
        handTrackerR = XR_NULL_HANDLE;
    }
}

void YVRHandMgr::initHandTracking()
{
    XrHandTrackerCreateInfoEXT createInfo{XR_TYPE_HAND_TRACKER_CREATE_INFO_EXT};
    createInfo.handJointSet = XR_HAND_JOINT_SET_DEFAULT_EXT;
    createInfo.hand = XR_HAND_LEFT_EXT;
    OpenXRAPI(xrCreateHandTrackerEXT(plugin.openxrMgr->program->session, &createInfo, &handTrackerL));
    createInfo.hand = XR_HAND_RIGHT_EXT;
    OpenXRAPI(xrCreateHandTrackerEXT(plugin.openxrMgr->program->session, &createInfo, &handTrackerR));

    handDataL.info.baseSpace = plugin.inputMgr->appSpace;
    handDataR.info.baseSpace = plugin.inputMgr->appSpace;

    isInit = true;
    AnnounceCallingFunc();
}

void YVRHandMgr::refreshHandPose(bool forRender)
{
    if (!isHandTrackingEnable) return;
    if (!isInit) initHandTracking();

    handDataL.info.time = handDataR.info.time = plugin.inputMgr->getTime(forRender);
    OpenXRAPI(xrLocateHandJointsEXT(handTrackerL, &handDataL.info, &handDataL.locations));
    OpenXRAPI(xrLocateHandJointsEXT(handTrackerR, &handDataR.info, &handDataR.locations));
}


void YVRHandMgr::setHandTrackingEnable(bool enabled)
{
    AnnounceCallingFunc();
    isHandTrackingEnable = enabled;
}
