#pragma once
#include "YVR_Lib_Types.h"
#include "WriteLog.h"
#include "Headers/yvrtc_interface.h"
#include "YVRMath.h"
#include <queue>
#include "YVRUnityPlugin.h"

class YVRTrackingMgr
{
public:
    yvrHMDState hmdState{};
    void getRenderingTrackingData(yvrPoseState* headPose, yvrPoseState* lEyePose, yvrPoseState* rEyePose, yvrPoseState* lControllerPose,
        yvrPoseState* rControllerPose);
    void getPhysicsTrackingData(yvrPoseState* headPose, yvrPoseState* lEyePose, yvrPoseState* rEyePose, yvrPoseState* lControllerPose,
        yvrPoseState* rControllerPose);
    void getControllerState(yvrControllerState* lControllerState, yvrControllerState* rControllerState);

    std::queue<uint64_t> yvrHMDPoseIndexQueue{};

private:

    void convertNativePose2UnityPose(yvrPoseState* headPose, yvrPoseState* lEyePose, yvrPoseState* rEyePose, yvrPoseState* lControllerPose,
        yvrPoseState* rControllerPose);
    void convertOpenGlPose2UnityPose(yvrPosef* pose);
    void convertOpenGlPoseState2UnityPoseState(yvrPoseState* poseState);
    void convertOpenGlVector2UnityVector(yvrVector3f* vector);
    void convertOpenGlQuaternion2UnityQuaternion(yvrQuatf* quatf);

    void convertHmdPose2EyePose(yvrPoseState* targetPose, int targetEye);
};

extern YVRTrackingMgr yvrTrackingMgr;