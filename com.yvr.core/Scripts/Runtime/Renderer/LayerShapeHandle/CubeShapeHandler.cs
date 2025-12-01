namespace YVR.Core
{
    public class CubeShapeHandler : ILayerShapeHandler
    {
        public void HandleLayerPose(IYVRLayerHandle layerHandle, LayerShapeData data)
        {
            var layerTransform = data.layerTransform;
            XRPose pose = new XRPose();
            pose.position = layerTransform.position.FromFlippedZVector3f();
            pose.orientation = layerTransform.rotation.FromFlippedZQuatf();
            layerHandle.SetLayerPose(data.renderLayerId,pose);
        }

        public void HandleLayerShape(IYVRLayerHandle layerHandle, LayerShapeData data) { }
    }
}