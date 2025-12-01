using UnityEngine;
using YVR.Core;
#if XR_CORE_UTILS
using Unity.XR.CoreUtils;
#endif

namespace YVR.Core
{
    public class EquirectShapeHandler : ILayerShapeHandler
    {
        public void HandleLayerPose(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            int renderLayerId = data.renderLayerId;
            Transform transform = data.layerTransform;
            YVRManager yvrManager = data.yvrManager;
            XRPose pose = new XRPose();
#if XR_CORE_UTILS
        if (GameObject.FindObjectOfType<XROrigin>() != null)
        {
            pose = transform.ToXRTrackingSpacePose(GameObject.FindObjectOfType<XROrigin>().Camera);
        }
        else
        {
            pose = transform.ToXRTrackingSpacePose(yvrManager.cameraRenderer.centerEyeCamera);
        }
#else
            pose = transform.ToXRTrackingSpacePose(yvrManager.cameraRenderer.centerEyeCamera);
#endif

            layerHandle.SetLayerPose(renderLayerId, pose);
        }

        public void HandleLayerShape(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            int renderLayerId = data.renderLayerId;
            float radius = data.radius;
            Rect destRect = data.destRect;
            layerHandle.SetLayerEquirectRadius(renderLayerId, radius);
            layerHandle.SetLayerEquirectOffset(renderLayerId, destRect);
        }
    }
}