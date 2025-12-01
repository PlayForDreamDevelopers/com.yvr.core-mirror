# if XR_COMPOSITION_LAYERS
using Unity.XR.CompositionLayers.Layers;
using Unity.XR.CompositionLayers.Services;
using UnityEngine;
using UnityEngine.XR;

namespace YVR.Core.XRCompositionLayers
{
    public class XREquirectLayer : YVRCustomLayerHandler
    {
        public static InputDevice centerEyeDevice => InputDevices.GetDeviceAtXRNode(XRNode.Head);

        public XREquirectLayer(IYVRLayerHandle yvrLayerHandle) : base(yvrLayerHandle) { }

        protected override void InitCompositionLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            m_LayerInfo = layerInfo;
            var layerData = layerInfo.Layer.LayerData;
            m_LayerDataType = layerData.GetType();
            GetLayerExtensionData(ref texture, ref m_SourceRect, ref m_DestRect);
            width = texture ? (int)(texture.width * m_RenderXScale) : (int)resolution.x;
            height = texture ? (int)(texture.height * m_RenderYScale) : (int)resolution.y;

            if (layerData is EquirectMeshLayerData equirect)
            {
                m_Shape = YVRRenderLayerType.Equirect;
                radius = equirect.Radius;
                centralHorizontalAngle = equirect.CentralHorizontalAngle;
                upperVerticalAngle = equirect.UpperVerticalAngle;
                lowerVerticalAngle = -equirect.LowerVerticalAngle;
            }
            else
            {
                Debug.LogError("XREquirectLayer: Unsupported LayerData type: " + layerData.GetType().Name);
                return;
            }

            CreateYVRLayer();
        }

        protected override void HandleLayerPose()
        {
            XRPose pose = new XRPose();
            Transform camera = Camera.main.transform;
            XRPose xrHeadPose = XRPose.identity;
            centerEyeDevice.TryGetFeatureValue(CommonUsages.centerEyePosition, out xrHeadPose.position);
            centerEyeDevice.TryGetFeatureValue(CommonUsages.centerEyeRotation, out xrHeadPose.orientation);
            XRPose headSpacePose = camera.ToYVRPose().Inverse() * compositionLayer.transform.ToYVRPose();
            pose = xrHeadPose * headSpacePose;
            m_YVRLayerHandle.SetLayerPose(renderLayerId, pose);
        }

        protected override void HandleLayerShape()
        {
            m_YVRLayerHandle.SetLayerEquirect2Param(renderLayerId, radius, centralHorizontalAngle, upperVerticalAngle,
                lowerVerticalAngle);
        }
    }
}
#endif