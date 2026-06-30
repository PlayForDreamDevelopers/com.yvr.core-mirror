using UnityEngine;
using System;
using UnityEngine.Internal;
using YVR.Core.ImageTracking;

namespace YVR.Core
{
    /// <summary>
    /// Stub implementation of YVRPlugin for unsupported editor platforms (Linux, macOS).
    /// This prevents DllNotFoundException errors when developing on non-Windows platforms.
    /// All methods return default values and do not perform any native operations.
    /// </summary>
    [ExcludeFromDocs]
    public class YVRPluginStub : YVRPlugin
    {
        private static bool s_WarningLogged = false;

        public static YVRPluginStub Create()
        {
            if (!s_WarningLogged)
            {
                Debug.LogWarning("YVR Plugin: Running on unsupported editor platform. Native plugin features will be disabled. " +
                                 "Deploy to an Android device or use Windows editor for full functionality.");
                s_WarningLogged = true;
            }
            return new YVRPluginStub();
        }

        public override void SetVSyncCount(YVRQualityManager.VSyncCount vSyncCount)
        {
            QualitySettings.vSyncCount = (int)vSyncCount;
        }

        public override void GetEyeResolution(ref Vector2 resolution)
        {
            resolution.x = Screen.width;
            resolution.y = Screen.height;
        }

        public override void GetEyeFov(int eyeSide, ref YVRCameraRenderer.EyeFov eyeFov)
        {
            eyeFov.UpFov = eyeFov.DownFov = eyeFov.LeftFov = eyeFov.RightFov = 45;
        }

        public override void GetCurrentInputDevice(ref ActiveInputDevice inputDevice)
        {
            inputDevice = ActiveInputDevice.ControllerActive;
        }

        public override void GetHandJointLocations(HandType handType, ref HandJointLocations jointLocations)
        {
            // Return empty/inactive hand data
            if (jointLocations.jointLocations == null || jointLocations.jointVelocities == null)
            {
                jointLocations.jointLocations = new HandJointLocation[(int)HandJoint.JointMax];
                jointLocations.jointVelocities = new HandJointVelocity[(int)HandJoint.JointMax];
            }
            jointLocations.isActive = 0;
            jointLocations.jointCount = 0;
        }

        public override bool GetHandEnable() { return false; }
    }
}
