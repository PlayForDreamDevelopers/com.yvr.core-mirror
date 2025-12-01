namespace YVR.Core
{
    public interface ILayerShapeHandler
    {
        public void HandleLayerPose(IYVRLayerHandle layerHandle, LayerShapeData data);

        public void HandleLayerShape(IYVRLayerHandle layerHandle, LayerShapeData data);
    }
}