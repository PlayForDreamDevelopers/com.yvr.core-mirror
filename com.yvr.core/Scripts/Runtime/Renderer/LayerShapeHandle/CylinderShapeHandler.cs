using UnityEngine;
using UnityEngine.XR;
#if XR_CORE_UTILS
using Unity.XR.CoreUtils;
#endif

namespace YVR.Core
{
    public class CylinderShapeHandler : ILayerShapeHandler
    {
        public static InputDevice centerEyeDevice => InputDevices.GetDeviceAtXRNode(XRNode.Head);
        public void HandleLayerPose(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            int renderLayerId = data.renderLayerId;
            Transform transform = data.layerTransform;
            float angle = data.centralAngle;
            Transform camera = Camera.main.transform;
            float radius = CalculateRadius(angle, transform.lossyScale.x);
            Vector3 origin = transform.position - (transform.forward * radius);
            XRPose originPose = new XRPose() {orientation = transform.rotation, position = origin};
            XRPose xrHeadPose = XRPose.identity;
            centerEyeDevice.TryGetFeatureValue(CommonUsages.centerEyePosition, out xrHeadPose.position);
            centerEyeDevice.TryGetFeatureValue(CommonUsages.centerEyeRotation, out xrHeadPose.orientation);
            XRPose pose = new XRPose();
            pose = xrHeadPose * camera.ToYVRPose().Inverse() * originPose;
            layerHandle.SetLayerPose(renderLayerId, pose);
        }

        public void HandleLayerShape(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            int renderLayerId = (int) data.renderLayerId;
            Transform transform = data.layerTransform;
            float angle = data.centralAngle;

            float radius = CalculateRadius(angle, transform.lossyScale.x);
            float ratio = transform.lossyScale.x / transform.lossyScale.y;
            float centralAngle = angle * Mathf.Deg2Rad;
            layerHandle.SetLayerCylinderParam(renderLayerId, radius, centralAngle, ratio);
        }

        private float CalculateRadius(float angle, float arcLength)
        {
            if (angle <= 0) return arcLength / 2f;
            return arcLength / ((2 * Mathf.PI) * (angle / 360f));
        }
    }
}