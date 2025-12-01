# if XR_COMPOSITION_LAYERS
using Unity.XR.CompositionLayers.Layers;
using Unity.XR.CompositionLayers.Services;
using UnityEngine;
using UnityEngine.XR;

namespace YVR.Core.XRCompositionLayers
{
    public class XRCylinderLayer : YVRCustomLayerHandler
    {
        public static InputDevice centerEyeDevice => InputDevices.GetDeviceAtXRNode(XRNode.Head);
        public XRCylinderLayer(IYVRLayerHandle yvrLayerHandle) : base(yvrLayerHandle) { }

        protected override void InitCompositionLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            m_LayerInfo = layerInfo;
            var layerData = layerInfo.Layer.LayerData;
            m_LayerDataType = layerData.GetType();
            GetLayerExtensionData(ref texture, ref m_SourceRect, ref m_DestRect);
            width = texture ? (int)(texture.width * m_RenderXScale) : (int)resolution.x;
            height = texture ? (int)(texture.height * m_RenderYScale) : (int)resolution.y;

            if (layerData is CylinderLayerData cylinder)
            {
                m_Shape = YVRRenderLayerType.Cylinder;
                var scaledSize = cylinder.GetScaledSize(compositionLayer.transform.lossyScale);
                radius = cylinder.ApplyTransformScale ? scaledSize.x : cylinder.Radius;
                centralAngle = cylinder.ApplyTransformScale ? scaledSize.y : cylinder.CentralAngle;
                ratio = cylinder.ApplyTransformScale ? scaledSize.z : cylinder.AspectRatio;
            }
            else
            {
                Debug.LogError("XRCylinderLayer: Unsupported LayerData type: " + layerData.GetType().Name);
                return;
            }

            CreateYVRLayer();
        }

        protected override void HandleLayerPose()
        {
            Transform camera = Camera.main.transform;
            Vector3 origin = compositionLayer.transform.position - (compositionLayer.transform.forward * radius);
            XRPose originPose = new XRPose() {orientation = compositionLayer.transform.rotation, position = origin};
            XRPose xrHeadPose = XRPose.identity;
            centerEyeDevice.TryGetFeatureValue(CommonUsages.centerEyePosition, out xrHeadPose.position);
            centerEyeDevice.TryGetFeatureValue(CommonUsages.centerEyeRotation, out xrHeadPose.orientation);
            XRPose pose = new XRPose();
            pose = xrHeadPose * camera.ToYVRPose().Inverse() * originPose;
            m_YVRLayerHandle.SetLayerPose(renderLayerId, pose);
        }

        protected override void HandleLayerShape()
        {
            m_YVRLayerHandle.SetLayerCylinderParam(renderLayerId, radius, centralAngle, ratio);
        }
    }
}
#endif