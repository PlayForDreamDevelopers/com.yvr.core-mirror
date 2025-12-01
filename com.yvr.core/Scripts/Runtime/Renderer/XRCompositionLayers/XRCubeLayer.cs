# if XR_COMPOSITION_LAYERS
using System;
using Unity.XR.CompositionLayers.Layers;
using Unity.XR.CompositionLayers.Services;
using UnityEngine;

namespace YVR.Core.XRCompositionLayers
{
    public class XRCubeLayer : YVRCustomLayerHandler
    {
        public XRCubeLayer(IYVRLayerHandle yvrLayerHandle) : base(yvrLayerHandle) { }

        protected override void InitCompositionLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            m_LayerInfo = layerInfo;
            var layerData = layerInfo.Layer.LayerData;
            m_LayerDataType = layerData.GetType();

            // get ExtensionData
            GetLayerExtensionData(ref texture, ref m_SourceRect, ref m_DestRect);
            width = texture ? (int)(texture.width * m_RenderXScale) : (int)resolution.x;
            height = texture ? (int)(texture.height * m_RenderYScale) : (int)resolution.y;
            m_Shape = YVRRenderLayerType.CubeProjection;
            CreateYVRLayer();
        }

        protected override void CopyTextureToColorHandle()
        {
            for (int i = 0; i < swapChainBufferCount; i++)
            {
                if (textureHandle == 0) return;
                int destTextureId = m_YVRLayerHandle.GetLayerColorHandle(renderLayerId,
                    YVRRenderLayerEyeMask.kEyeMaskBoth, i % swapChainBufferCount);
                var destTexture =
                    Cubemap.CreateExternalTexture(width, TextureFormat.RGBA32, false, (IntPtr)destTextureId);
                Graphics.CopyTexture(texture, destTexture);
            }
        }

        protected override void HandleLayerPose()
        {
            XRPose pose = new XRPose();
            pose.position = compositionLayer.transform.position.FromFlippedZVector3f();
            pose.orientation = compositionLayer.transform.rotation.FromFlippedZQuatf();
            m_YVRLayerHandle.SetLayerPose(renderLayerId,pose);
        }

        protected override void HandleLayerShape() { }

    }

}
#endif