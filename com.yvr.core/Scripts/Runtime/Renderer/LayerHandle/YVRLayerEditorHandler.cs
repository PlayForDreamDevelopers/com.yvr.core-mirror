using System;
using UnityEngine;

namespace YVR.Core
{
    public class YVRLayerEditorHandle : IYVRLayerHandle
    {
        public int CreateLayer(YVRLayerCreateInfo layerCreateInfo) { return -1; }
        public void PrepareCreateLayerAsync(Action onCreateLayerPrepared = null) { }
        public void DestroyLayer(int layer, bool destroyImmediate) { }
        public void PrepareDestroyLayerAsync(Action onPrepareDestroyLayer = null) { }
        public void AddLayer(int layerId) { }
        public void PrepareAddLayer(Action onPrepareAddLayer = null) { }
        public void RemoveLayer(int layerId) { }
        public void PrepareRemoveLayer(Action onPrepareRemoveLayer = null) { }
        public void SetLayerVisible(int layerId, bool visible) { }
        public void PrepareSetLayerVisible(bool visible, Action onPrepareSetLayerVisible = null) { }
        public void CreateLayerAsync(YVRLayerCreateInfo layerCreateInfo, Action<int> onLayerCreated = null) { }
        public void DestroyLayerAsync(int layerId, bool destroyImmediate, Action onLayerDestroyed = null) { }
        public void AddLayerAsync(int layerId, Action onLayerAdded = null) { }
        public void RemoveLayerAsync(int layerId, Action onLayerAdded = null) { }

        public void SwapBufferLayer(int layerId) { }
        public int GetEyeBufferLayerId() { return 0; }

        public int GetLayerColorHandle(int layerId, int index) { return 0; }
        public int GetLayerColorHandle(int layerId) { return 0; }

        public void SetLayerPose(in int layerId, in XRPose pose) { }
        public void SetLayerSize(in int layerId, XRSize size) { }
        public void SetLayerCylinderParam(in int layerId, in float radius, in float centralAngle, in float aspectRatio) { }
        public void SetLayerEquirectRadius(in int layerId, in float radius) { }
        public void SetLayerEquirectOffset(in int layerId, in Rect rect) { }
        public void SetLayerEquirect2Param(in int layerId, in float radius, in float centralHorizontalAngle, in float upperVerticalAngle, in float lowerVerticalAngle) { }
        public void SetLayerSettings(int layerId, bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen) { }
        public void SetLayerImageRect(int layerId, Rect rect) { }
        public void SetSurfaceDimensions(int layerId, int width, int height) { }
        public int[] GetAlLayersColorHandle() { return null; }

        public int GetLayersCount() { return 0; }

        public void SetLayerPreSubmitCallback(Action onLayerPreSubmit) { }

        public void SetLayerDepthAsync(int layerId, int depth, Action<bool> onLayerDepthSet = null) { }

        public IntPtr GetLayerSurface(int layerId) { return IntPtr.Zero; }
    }
}