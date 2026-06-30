#include "YVREyeTrackingMgr.h"
#include "../../../YVRUnityPlugin.h"


YVREyeTrackingMgr::YVREyeTrackingMgr()
{
    eyeGazeInteractionProperties = {XR_TYPE_SYSTEM_EYE_GAZE_INTERACTION_PROPERTIES_EXT};
    systemProperties = {XR_TYPE_SYSTEM_PROPERTIES};
    systemProperties.next = &eyeGazeInteractionProperties;

    eyeGazeSampleTime = {XR_TYPE_EYE_GAZE_SAMPLE_TIME_EXT};
    eyeGazeLocation = {XR_TYPE_SPACE_LOCATION, &eyeGazeSampleTime};
}

YVREyeTrackingMgr::~YVREyeTrackingMgr() {}

bool YVREyeTrackingMgr::getEyeTrackingEnable()
{
    if (!getEyeTrackingSupports()) return false;
    int propData = plugin.openxrMgr->extMgr->getIntProperty(YVR_SYS_PROP_GET_EYE_TRACKING_ENABLED);
    propData = propData & 0x0F;
    return propData == 1;
}

bool YVREyeTrackingMgr::getEyeTrackingSupports()
{
    OpenXRAPI(xrGetSystemProperties(plugin.openxrMgr->program->instance, plugin.openxrMgr->program->systemId, &systemProperties));
    return eyeGazeInteractionProperties.supportsEyeGazeInteraction;
}

void YVREyeTrackingMgr::createActions()
{
    const XrSession& session = plugin.openxrMgr->program->session;
    const XrInstance& instance = plugin.openxrMgr->program->instance;
    XrActionSetCreateInfo actionSetInfo{XR_TYPE_ACTION_SET_CREATE_INFO};
    actionSetInfo.priority = 0;

    strcpy(actionSetInfo.actionSetName, "eyetracking");
    strcpy(actionSetInfo.localizedActionSetName, "EyeTracking");
    OpenXRAPI(xrCreateActionSet(instance, &actionSetInfo, &actionCtx.actionSet))

    XrActionCreateInfo actionInfo{XR_TYPE_ACTION_CREATE_INFO};
    actionInfo.actionType = XR_ACTION_TYPE_POSE_INPUT;
    strcpy(actionInfo.actionName, "eye_gaze_pose");
    strcpy(actionInfo.localizedActionName, "Eye Gaze Pose");
    OpenXRAPI(xrCreateAction(actionCtx.actionSet, &actionInfo, &actionCtx.eyeGazePoseAction));

    XrPath eyeGazePosePath;
    OpenXRAPI(xrStringToPath(instance, "/user/eyes_ext/input/gaze_ext/pose", &eyeGazePosePath));

    XrPath eyeGazeInteractionProfilePath;
    OpenXRAPI(xrStringToPath(instance, "/interaction_profiles/ext/eye_gaze_interaction", &eyeGazeInteractionProfilePath));

    const XrActionSuggestedBinding eyeBindings = {actionCtx.eyeGazePoseAction, eyeGazePosePath};

    XrInteractionProfileSuggestedBinding eyeSuggestedBindings{XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING};
    eyeSuggestedBindings.interactionProfile = eyeGazeInteractionProfilePath;
    eyeSuggestedBindings.suggestedBindings = &eyeBindings;
    eyeSuggestedBindings.countSuggestedBindings = 1;
    OpenXRAPI(xrSuggestInteractionProfileBindings(instance, &eyeSuggestedBindings));

    XrActionSpaceCreateInfo eyeGazeActionSpaceInfo{XR_TYPE_ACTION_SPACE_CREATE_INFO};
    eyeGazeActionSpaceInfo.action = actionCtx.eyeGazePoseAction;
    eyeGazeActionSpaceInfo.poseInActionSpace.orientation.w = 1.f;
    OpenXRAPI(xrCreateActionSpace(session, &eyeGazeActionSpaceInfo, &actionCtx.eyeGazeSpace));
}

void YVREyeTrackingMgr::updateEyeTrackingPose(const bool forRender)
{
    const XrTime predictedDisplayTime = plugin.inputMgr->getTime(forRender);
    xrLocateSpace(actionCtx.eyeGazeSpace, plugin.inputMgr->appSpace, predictedDisplayTime, &eyeGazeLocation);
}


void YVREyeTrackingMgr::destroyActions()
{
    OpenXRAPI(xrDestroyAction(actionCtx.eyeGazePoseAction))
    OpenXRAPI(xrDestroyActionSet(actionCtx.actionSet));
}
