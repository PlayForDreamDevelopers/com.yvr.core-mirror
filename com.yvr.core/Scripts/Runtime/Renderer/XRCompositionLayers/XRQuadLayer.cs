# if XR_COMPOSITION_LAYERS
using Unity.XR.CompositionLayers.Extensions;
using Unity.XR.CompositionLayers.Layers;
using Unity.XR.CompositionLayers.Services;
using UnityEngine;

namespace YVR.Core.XRCompositionLayers
{
    public class XRQuadLayer : YVRCustomLayerHandler
    {
        private TexturesExtension m_TexturesExtension;
        private QuadLayerData m_QuadLayerData;
        public XRQuadLayer(IYVRLayerHandle yvrLayerHandle) : base(yvrLayerHandle) { }

        protected override void InitCompositionLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            m_LayerInfo = layerInfo;
            var layerData = layerInfo.Layer.LayerData;
            m_LayerDataType = layerData.GetType();
            m_TexturesExtension = layerInfo.Layer.GetComponent<TexturesExtension>();
            GetLayerExtensionData(ref texture, ref m_SourceRect, ref m_DestRect);
            width =  (int)(texture ? texture.width  : resolution.x);
            height = (int)(texture ? texture.height : resolution.y);
            if (layerData is QuadLayerData quad)
            {
                m_Shape = YVRRenderLayerType.Quad;
                m_QuadLayerData = quad;
                var size = quad.GetScaledSize(compositionLayer.transform.lossyScale);
                m_RenderXScale = size.x;
                m_RenderYScale = size.y;
            }
            else
            {
                Debug.LogError("XRQuadLayer: Unsupported LayerData type: " + layerData.GetType().Name);
                return;
            }

            CreateYVRLayer();
        }

        protected override void HandleLayerPose()
        {
            m_DestRect.width = Mathf.Clamp(m_DestRect.width, 0f, 1f - m_DestRect.x);
            m_DestRect.height = Mathf.Clamp(m_DestRect.height, 0f, 1f - m_DestRect.y);

            Vector3 lossyScale = compositionLayer.transform.lossyScale;
            float offsetX = -lossyScale.x / 2f + m_DestRect.x * lossyScale.x +
                            m_DestRect.width * lossyScale.x / 2f;
            float offsetY = -lossyScale.y / 2f + m_DestRect.y * lossyScale.y +
                            m_DestRect.height * lossyScale.y / 2f;
            var offsetPose = new XRPose
                { position = new Vector3(offsetX, offsetY), orientation = Quaternion.identity };
            XRPose pose = compositionLayer.transform.ToXRTrackingSpacePose(Camera.main, offsetPose);
            m_YVRLayerHandle.SetLayerPose(renderLayerId, pose);
        }


        protected override void HandleLayerShape()
        {
            var correctedSize = m_TexturesExtension.CropToAspect ?
                FixAspectRatio(m_QuadLayerData, width, height) :
                m_QuadLayerData.GetScaledSize(compositionLayer.transform.lossyScale);
            float sizeX = correctedSize.x * m_TexturesExtension.LeftEyeDestinationRect.width;
            float sizeY = correctedSize.y * m_TexturesExtension.LeftEyeDestinationRect.height;
            m_YVRLayerHandle.SetLayerSize(renderLayerId, new XRSize(sizeX, sizeY));
        }

        private Vector2 FixAspectRatio(QuadLayerData data, int texWidth, int texHeight)
        {
            var requestedSize = data.GetScaledSize(compositionLayer.transform.lossyScale);
            float reqSizeRatio = (float)requestedSize.x / (float)requestedSize.y;
            float texRatio = (float)texWidth / (float)texHeight;
            if (reqSizeRatio > texRatio)
            {
                // too wide
                requestedSize.x = requestedSize.y * texRatio;
            }
            else if (reqSizeRatio < texRatio)
            {
                // too narrow
                requestedSize.y = requestedSize.x / texRatio;
            }
            return requestedSize;
        }
    }
}
#endif