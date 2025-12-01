# if XR_COMPOSITION_LAYERS
using System;
using System.Collections.Generic;
using System.Linq;
using Unity.XR.CompositionLayers.Provider;
using Unity.XR.CompositionLayers.Services;
using Unity.XR.CompositionLayers.Layers;
using UnityEngine;

namespace YVR.Core.XRCompositionLayers
{
    public class YVRCompositionLayerProvider : ILayerProvider
    {
        private static readonly Dictionary<int, YVRCustomLayerHandler> m_LayerMap = new();

        private IYVRLayerHandle yvrLayerHandle = new YVRLayerAndroidHandler();

        private List<CompositionLayerManager.LayerInfo> m_CacheActiveLayers = new();

        public static IntPtr GetLayerAndroidSurfaceObject(int layerObjectInstanceID)
        {
            IntPtr surfacePtr = default;
            foreach (var layerHandlerPair in m_LayerMap)
            {
                if (layerHandlerPair.Value?.compositionLayer?.GetInstanceID() == layerObjectInstanceID)
                {
                    surfacePtr = layerHandlerPair.Value.GetExternalAndroidSurface();
                }
            }

            return surfacePtr;
        }

        private YVRCustomLayerHandler GenerateLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            var layer = layerInfo.Layer;
            if (layer == null || layer.LayerData == null)
                return null;

            var layerHandler = CreateLayerHandler(layer.LayerData);
            if (layerHandler != null)
            {
                layerHandler.CreateLayer(layerInfo);
            }
            return layerHandler;
        }

        private YVRCustomLayerHandler CreateLayerHandler(LayerData layerData)
        {
            switch (layerData)
            {
                case QuadLayerData:
                    return new XRQuadLayer(yvrLayerHandle);
                case CylinderLayerData:
                    return new XRCylinderLayer(yvrLayerHandle);
                case EquirectMeshLayerData:
                    return new XREquirectLayer(yvrLayerHandle);
                case ProjectionLayerData:
                case ProjectionLayerRigData:
                    return new XRProjectionLayer(yvrLayerHandle);
                case CubeProjectionLayerData:
                    return new XRCubeLayer(yvrLayerHandle);
                default:
                    Debug.LogError($"Unsupported LayerData type: {layerData.GetType().Name}");
                    return null;
            }
        }

        public void SetInitialState(List<CompositionLayerManager.LayerInfo> layers)
        {

        }

        public void CleanupState()
        {
            foreach (var yvrLayer in m_LayerMap.Values)
            {
                yvrLayerHandle.DestroyLayer(yvrLayer.renderLayerId, true);
            }

            m_LayerMap.Clear();
        }

        public void UpdateLayers(
            List<CompositionLayerManager.LayerInfo> createdLayers,
            List<int> removedLayers,
            List<CompositionLayerManager.LayerInfo> modifiedLayers,
            List<CompositionLayerManager.LayerInfo> activeLayers)
        {
            foreach (var info in createdLayers)
            {
                m_LayerMap[info.Id] = GenerateLayer(info);
            }

            foreach (var id in removedLayers)
            {
                if (m_LayerMap.TryGetValue(id, out var yvrLayer))
                {
                    yvrLayer?.Destory();
                    m_LayerMap.Remove(id);
                }
            }

            foreach (var info in modifiedLayers)
            {
                if (m_LayerMap.TryGetValue(info.Id, out var yvrLayer))
                {
                    yvrLayer?.ModifyLayer(info);
                }
            }

            foreach (var info in activeLayers)
            {
                if (m_LayerMap.TryGetValue(info.Id, out var yvrLayer))
                {
                    yvrLayer?.UpdateCompositeLayerContent();
                }
            }

            UpdateLayerVisible(activeLayers);
            m_CacheActiveLayers = activeLayers;
        }

        private void UpdateLayerVisible(List<CompositionLayerManager.LayerInfo> layerInfos)
        {
            var added = layerInfos.Except(m_CacheActiveLayers).ToList();
            var removed = m_CacheActiveLayers.Except(layerInfos).ToList();
            foreach (var info in added)
            {
                if (m_LayerMap.TryGetValue(info.Id, out var yvrLayer))
                {
                    yvrLayer?.Show();
                }
            }

            foreach (var info in removed)
            {
                if (m_LayerMap.TryGetValue(info.Id, out var yvrLayer))
                {
                    yvrLayer?.Hide();
                }
            }
        }

        public void LateUpdate() { }

    }
}
#endif