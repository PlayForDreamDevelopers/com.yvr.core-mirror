#include "YVRTrackingMgr.h"

using namespace YVR;

YVRTrackingMgr yvrTrackingMgr;

void YVRTrackingMgr::convertHmdPose2EyePose(yvrPoseState* targetPose, int targetEye)
{
    float halfIPD = 60 * 0.5f;
    targetPose->pose.position =(Quatf)targetPose->pose.orientation * Vector3f(halfIPD * (targetEye == LeftEye ? -1.0f : 1.0f), 0, 0) + (Vector3f)targetPose->pose.position;
}

void YVRTrackingMgr::convertOpenGlPose2UnityPose(yvrPosef* pose)
{
    convertOpenGlVector2UnityVector(&pose->position);
    convertOpenGlQuaternion2UnityQuaternion(&pose->orientation);
}

void YVRTrackingMgr::convertOpenGlPoseState2UnityPoseState(yvrPoseState* poseState)
{
    convertOpenGlPose2UnityPose(&poseState->pose);
    convertOpenGlVector2UnityVector(&poseState->angularVelocity);
    convertOpenGlVector2UnityVector(&poseState->linearVelocity);
    convertOpenGlVector2UnityVector(&poseState->angularAcceleration);
    convertOpenGlVector2UnityVector(&poseState->linearAcceleration);
}

void YVRTrackingMgr::convertOpenGlVector2UnityVector(yvrVector3f* vector) { vector->z *= -1; }

void YVRTrackingMgr::convertOpenGlQuaternion2UnityQuaternion(yvrQuatf* quatf)
{
    quatf->x *= -1;
    quatf->y *= -1;
}

void YVRTrackingMgr::convertNativePose2UnityPose(yvrPoseState* headPose, yvrPoseState* lEyePose, yvrPoseState* rEyePose,
    yvrPoseState* lControllerPose, yvrPoseState* rControllerPose)
{
    convertOpenGlPoseState2UnityPoseState(headPose);  // now the data in lEyePose is hmdPose actually
    convertOpenGlPoseState2UnityPoseState(lControllerPose);
    convertOpenGlPoseState2UnityPoseState(rControllerPose);

    memcpy(lEyePose, headPose, sizeof(yvrPoseState));
    memcpy(rEyePose, headPose, sizeof(yvrPoseState));

    convertHmdPose2EyePose(lEyePose, LeftEye);
    convertHmdPose2EyePose(rEyePose, RightEye);
}

void YVRTrackingMgr::getRenderingTrackingData(yvrPoseState* headPose, yvrPoseState* lEyePose, yvrPoseState* rEyePose, yvrPoseState* lControllerPose,
    yvrPoseState* rControllerPose)
{
    convertNativePose2UnityPose(headPose, lEyePose, rEyePose, lControllerPose, rControllerPose);
}

void YVRTrackingMgr::getPhysicsTrackingData(yvrPoseState* headPose, yvrPoseState* lEyePose, yvrPoseState* rEyePose, yvrPoseState* lControllerPose,
    yvrPoseState* rControllerPose)
{
    yvrHMDState yvrHMD;
    if (plugin.previewToolService->GetTrackingData(&yvrHMD))
    {
        memcpy(&hmdState, &yvrHMD, sizeof(yvrHMDState));
        memcpy(headPose, &yvrHMD.m_HeadPose, sizeof(yvrPoseState));
        memcpy(lControllerPose, &yvrHMD.m_LControllerPose, sizeof(yvrPoseState));
        memcpy(rControllerPose, &yvrHMD.m_RControllerPose, sizeof(yvrPoseState));
        convertNativePose2UnityPose(headPose, lEyePose, rEyePose, lControllerPose, rControllerPose);
        plugin.yvrHandMgr->SetHandJontLocations(&yvrHMD);
        plugin.renderConfigsMgr->setEyeBias(yvrHMD.rfrustumPose, yvrHMD.lfrustumPose);
    }
}

void YVRTrackingMgr::getControllerState(yvrControllerState* lControllState, yvrControllerState* rControllerState)
{
    if (&hmdState.m_YVRControllerConnectedInfo.left_connected)
    {
        memcpy(lControllState, &hmdState.m_LControllerState, sizeof(yvrControllerState));
    }
    if (&hmdState.m_YVRControllerConnectedInfo.right_connected)
    {
        memcpy(rControllerState, &hmdState.m_RControllerState, sizeof(yvrControllerState));
    }
}