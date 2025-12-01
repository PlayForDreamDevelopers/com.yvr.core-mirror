# if XR_COMPOSITION_LAYERS
using System;
using Unity.XR.CompositionLayers.Extensions;
using Unity.XR.CompositionLayers.Layers;
using Unity.XR.CompositionLayers.Services;
using UnityEngine;

namespace YVR.Core.XRCompositionLayers
{
    public class XRProjectionLayer : YVRCustomLayerHandler
    {
        public XRProjectionLayer(IYVRLayerHandle yvrLayerHandle) : base(yvrLayerHandle) { }

        protected override void InitCompositionLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            m_LayerInfo = layerInfo;
            var layerData = layerInfo.Layer.LayerData;
            m_LayerDataType = layerData.GetType();

            GetLayerExtensionData(ref texture, ref m_SourceRect, ref m_DestRect);

            m_Shape = YVRRenderLayerType.Projection;
            switch (layerData)
            {
                case ProjectionLayerData:
                    SetupProjectionLayer(layerInfo);
                    break;
                case ProjectionLayerRigData:
                    SetupProjectionLayerRig(layerInfo);
                    break;
                default:
                    Debug.LogError("XRProjectionLayer: Unsupported LayerData type: " + layerData.GetType().Name);
                    return;
            }

            CreateYVRLayer();
        }

        private void SetupProjectionLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            Vector2 resolution = default;
            YVRPlugin.Instance.GetEyeResolution(ref resolution);
            width = (int)resolution.x;
            height = (int)resolution.y;
            separateSwapChain = true;
        }

        private void SetupProjectionLayerRig(CompositionLayerManager.LayerInfo layerInfo)
        {
            Vector2 resolution = default;
            YVRPlugin.Instance.GetEyeResolution(ref resolution);
            width = (int)resolution.x;
            height = (int)resolution.y;
            separateSwapChain = true;
            // Create render textures to be the target texture of the child cameras.
            Camera leftCamera = null;
            Camera rightCamera = null;
            if (compositionLayer.transform.childCount >= 2)
            {
                leftCamera = compositionLayer.transform.GetChild(0).GetComponent<Camera>();
                rightCamera = compositionLayer.transform.GetChild(1).GetComponent<Camera>();
            }

            String layerName = String.Format("projectLayer_{0}", layerInfo.Id);
            RenderTexture leftRT = new RenderTexture(width, height, 0, RenderTextureFormat.ARGB32) { name = layerName + "_left" };
            leftRT.Create();
            RenderTexture rightRT = new RenderTexture(width, height, 0, RenderTextureFormat.ARGB32) { name = layerName + "_right" };
            rightRT.Create();

            TexturesExtension texExt = compositionLayer.GetComponent<TexturesExtension>();
            texExt.LeftTexture = leftRT;
            texExt.RightTexture = rightRT;
            if (leftCamera)
            {
                YVRCameraRenderer.EyeFov leftEyeFov = default;
                YVRPlugin.Instance.GetEyeFov(eyeSide: 0, ref leftEyeFov);
                float upFovDegrees = leftEyeFov.UpFov * Mathf.Rad2Deg;
                float downFovDegrees = leftEyeFov.DownFov * Mathf.Rad2Deg;
                float maxFovY = Mathf.Max(upFovDegrees, downFovDegrees);
                float totalFovY = 2 * maxFovY;
                leftCamera.fieldOfView = totalFovY;
                leftCamera.targetTexture = leftRT;
                texture = leftRT;
            }
            if (rightCamera)
            {
                YVRCameraRenderer.EyeFov rightEyeFov = default;
                YVRPlugin.Instance.GetEyeFov(eyeSide: 1, ref rightEyeFov);
                float upFovDegrees = rightEyeFov.UpFov * Mathf.Rad2Deg;
                float downFovDegrees = rightEyeFov.DownFov * Mathf.Rad2Deg;
                float maxFovY = Mathf.Max(upFovDegrees, downFovDegrees);
                float totalFovY = 2 * maxFovY;
                rightCamera.fieldOfView = totalFovY;
                rightCamera.targetTexture = rightRT;
                rightEyeTexture = rightRT;
            }
        }

        protected override void HandleLayerPose() { }

        protected override void HandleLayerShape() { }
    }
}
#endif