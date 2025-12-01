using UnityEngine;
using UnityEngine.XR;
#if XR_CORE_UTILS
using Unity.XR.CoreUtils;
#endif

namespace YVR.Core
{
    public class Equirect2ShapeHandler : ILayerShapeHandler
    {
        private const float PI = 3.14159265358979323846f;
        public static InputDevice centerEyeDevice => InputDevices.GetDeviceAtXRNode(XRNode.Head);

        public void HandleLayerPose(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            int renderLayerId = data.renderLayerId;
            Transform transform = data.layerTransform;
            XRPose pose = new XRPose();
            Transform camera = Camera.main.transform;
            XRPose xrHeadPose = XRPose.identity;
            centerEyeDevice.TryGetFeatureValue(CommonUsages.centerEyePosition, out xrHeadPose.position);
            centerEyeDevice.TryGetFeatureValue(CommonUsages.centerEyeRotation, out xrHeadPose.orientation);
            XRPose headSpacePose = camera.ToYVRPose().Inverse() * transform.ToYVRPose();
            pose = xrHeadPose * headSpacePose;
            layerHandle.SetLayerPose(renderLayerId, pose);
        }

        public void HandleLayerShape(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            int renderLayerId = data.renderLayerId;
            float radius = data.radius;
            Rect destRect = data.destRect;

            float deg2Rad = (PI / 180f);
            float lowerVerticalAngle = (-90f + 180f * destRect.y) * deg2Rad;
            float upperVerticalAngle = lowerVerticalAngle + (180f * destRect.height) * deg2Rad;
            float centralHorizontalAngle = 360f * destRect.width * deg2Rad;

            layerHandle.SetLayerEquirect2Param(renderLayerId, radius, centralHorizontalAngle, upperVerticalAngle, lowerVerticalAngle);
        }
    }
}