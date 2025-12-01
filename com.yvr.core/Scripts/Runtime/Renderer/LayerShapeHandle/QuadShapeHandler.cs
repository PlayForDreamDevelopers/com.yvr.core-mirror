using Unity.Profiling;
using UnityEngine;
using YVR.Core;
#if XR_CORE_UTILS
using Unity.XR.CoreUtils;
#endif

namespace YVR.Core
{
    public class QuadShapeHandler : ILayerShapeHandler
    {
        private static readonly ProfilerMarker s_HandleQuadLayerPoseMarker = new("HandleQuadLayerPose");

        private static Camera s_CenterCamera;

        private static Camera centerCamera
        {
            get
            {
                if (s_CenterCamera == null)
                {
#if XR_CORE_UTILS
                    s_CenterCamera = Object.FindObjectOfType<XROrigin>()?.Camera;
#endif
                    s_CenterCamera ??= Camera.main;
                }

                return s_CenterCamera;
            }
        }

        public void HandleLayerPose(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            using (s_HandleQuadLayerPoseMarker.Auto())
            {
                int renderLayerId = data.renderLayerId;
                Transform transform = data.layerTransform;
                Rect destRect = data.destRect;

                destRect.width = Mathf.Clamp(destRect.width, 0f, 1f - destRect.x);
                destRect.height = Mathf.Clamp(destRect.height, 0f, 1f - destRect.y);

                Vector3 lossyScale = transform.lossyScale;
                float offsetX = -lossyScale.x / 2f + destRect.x * lossyScale.x +
                                destRect.width * lossyScale.x / 2f;
                float offsetY = -lossyScale.y / 2f + destRect.y * lossyScale.y +
                                destRect.height * lossyScale.y / 2f;

                var offsetPose = new XRPose
                    {position = new Vector3(offsetX, offsetY), orientation = Quaternion.identity};

                XRPose pose = transform.ToXRTrackingSpacePose(centerCamera, offsetPose);

                layerHandle.SetLayerPose(renderLayerId, pose);
            }
        }

        public void HandleLayerShape(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            int renderLayerId = data.renderLayerId;
            Transform transform = data.layerTransform;
            Rect destRect = data.destRect;
            destRect.width = Mathf.Clamp(destRect.width, 0f, 1f - destRect.x);
            destRect.height = Mathf.Clamp(destRect.height, 0f, 1f - destRect.y);

            float sizeX = destRect.width * transform.lossyScale.x;
            float sizeY = destRect.height * transform.lossyScale.y;

            layerHandle.SetLayerSize(renderLayerId, new XRSize(sizeX, sizeY));
        }
    }
}