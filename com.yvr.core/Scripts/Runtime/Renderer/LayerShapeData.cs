using UnityEngine;

namespace YVR.Core
{
    public struct LayerShapeData
    {
        public int renderLayerId;
        public Transform layerTransform;
        public YVRManager yvrManager;
        public float centralAngle;
        public float radius;
        public float ratio;
        public Rect destRect;
        public bool isXRCompositionLayers;
        public float centralHorizontalAngle;
        public float upperVerticalAngle;
        public float lowerVerticalAngle;
    }
}