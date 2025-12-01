# if XR_COMPOSITION_LAYERS
using System;
using System.Collections.Generic;
using Unity.XR.CompositionLayers;
using Unity.XR.CompositionLayers.Extensions;
using Unity.XR.CompositionLayers.Layers;
using Unity.XR.CompositionLayers.Services;
using UnityEngine;
using YVR.Utilities;

namespace YVR.Core.XRCompositionLayers
{
    public abstract class YVRCustomLayerHandler
    {
        public int unityLayerId = -1;
        public int renderLayerId = -1;
        public int yvrrightEyeRenderLayerId { get; set; } = -1;
        protected CompositionLayer m_CompositionLayer;
        protected CompositionLayerManager.LayerInfo m_LayerInfo;
        private List<CompositionLayerExtension> m_LayerExtensions = new();

        public CompositionLayer compositionLayer
        {
            get => m_CompositionLayer;
            internal set => m_CompositionLayer = value;
        }

        protected IYVRLayerHandle m_YVRLayerHandle;

        internal List<CompositionLayerExtension> layerExtensions
        {
            get => m_LayerExtensions;
        }

        public int order => compositionLayer == null ? int.MinValue : compositionLayer.Order;
        internal bool Enabled => compositionLayer != null && compositionLayer.isActiveAndEnabled;

        public Texture texture = null;
        public Texture rightEyeTexture = null;
        public int width;
        public int height;

        protected Rect m_SourceRect = new Rect(0f, 0f, 1f, 1f);
        protected Rect m_DestRect = new Rect(0f, 0f, 1f, 1f);

        public YVRRenderLayerType m_Shape;
        public int swapChainBufferCount = 3;
        public bool isExternalTexture = false;
        protected bool m_IsRecreatingLayer = false;
        public float alpha = 1;
        private bool m_IsShowing = false;
        private int m_RightTextureHandle = -1;
        private int m_CachedTextureHandle = -1;
        protected Type m_LayerDataType;
        public int textureType = 0;
        public bool separateSwapChain = false;
        public Vector2 resolution = new Vector2(0, 0);

        //Shape
        protected float m_RenderXScale = 1.0f;
        protected float m_RenderYScale = 1.0f;
        public float radius = 0f;
        public float centralAngle = 1;
        public float ratio = 1.0f;
        public float centralHorizontalAngle = 0.0f;
        public float upperVerticalAngle = 0.0f;
        public float lowerVerticalAngle = 0.0f;

        protected int textureHandle
        {
            get
            {
                if (texture == null) return -1;
                return (int)texture.GetNativeTexturePtr();
            }
        }

        private int rightTextureHandle
        {
            get
            {
                if (rightEyeTexture == null) return -1;
                return (int)rightEyeTexture.GetNativeTexturePtr();
            }
        }

        public YVRCustomLayerHandler(IYVRLayerHandle yvrLayerHandle)
        {
            if (yvrLayerHandle == null)
            {
                Debug.LogError("YVRLayerHandle is null");
            }
            m_YVRLayerHandle = yvrLayerHandle;
        }

        public void CreateLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            compositionLayer = layerInfo.Layer;
            if (compositionLayer == null || compositionLayer.LayerData == null) return;

            compositionLayer.GetComponents(m_LayerExtensions);
            InitCompositionLayer(layerInfo);
            unityLayerId = layerInfo.Id;
            Application.focusChanged += OnFocusChanged;
        }

        public void OnFocusChanged(bool focus)
        {
            if (focus)
            {
                UpdateCompositeLayerContent();
            }
        }

        protected abstract void InitCompositionLayer(CompositionLayerManager.LayerInfo layerInfo);

        public void Show()
        {
            UpdateCompositeLayerContent();
            m_YVRLayerHandle.PrepareSetLayerVisible(true, () =>
            {
                if (renderLayerId != -1)
                {
                    m_YVRLayerHandle.SetLayerVisible(renderLayerId, true);
                    OnLayerAddedGfx();
                }
            });
        }

        public void Hide()
        {
            m_YVRLayerHandle.PrepareSetLayerVisible(false, () =>
            {
                if (renderLayerId != -1)
                {
                    m_YVRLayerHandle.SetLayerVisible(renderLayerId, false);
                    OnLayerRemovedGfx();
                }
            });
        }

        public void RecreateLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            m_IsRecreatingLayer = true;
            m_YVRLayerHandle.PrepareDestroyLayerAsync(() =>
            {
                m_IsRecreatingLayer = false;
                if (renderLayerId != -1)
                {
                    m_YVRLayerHandle.DestroyLayer(renderLayerId, false);
                }
            });

            InitCompositionLayer(m_LayerInfo);
        }

        public virtual void ModifyLayer(CompositionLayerManager.LayerInfo layerInfo)
        {
            compositionLayer.GetComponents(m_LayerExtensions);
            GetLayerExtensionData(ref texture, ref m_SourceRect, ref m_DestRect);
            if (m_LayerInfo.Layer.LayerData.GetType() != m_LayerDataType)
            {
                RecreateLayer(layerInfo);
            }
        }

        public void SetLayerDepth(int depth)
        {
            if (renderLayerId != -1)
            {
                m_YVRLayerHandle.SetLayerDepthAsync(renderLayerId, depth, null);
            }
        }

        protected virtual void OnLayerCreatedGfx(int layerId)
        {
            renderLayerId = layerId;
        }

        protected virtual void OnLayerAddedGfx()
        {
            m_IsShowing = true;
            AddUpdateLayerPoseAction();
        }

        protected virtual void OnLayerRemovedGfx()
        {
            m_IsShowing = false;
            RemoveUpdateLayerPoseAction();
        }

        private void AddUpdateLayerPoseAction()
        {
            Application.onBeforeRender -= UpdateLayerPose;
            Application.onBeforeRender += UpdateLayerPose;
        }

        private void RemoveUpdateLayerPoseAction()
        {
            Application.onBeforeRender -= UpdateLayerPose;
        }

        private void UpdateLayerPose()
        {
            if (compositionLayer.transform == null) return;
            if (renderLayerId == -1 || m_IsRecreatingLayer) return;

            HandleLayerPose();
            UpdateLayerShape();
        }

        protected abstract void HandleLayerPose();

        private void UpdateLayerShape()
        {
            if (compositionLayer.transform == null) return;
            if (renderLayerId == -1 || m_IsRecreatingLayer) return;

            m_YVRLayerHandle.SetLayerImageRect(renderLayerId, m_SourceRect);
            HandleLayerShape();
        }

        protected abstract void HandleLayerShape();

        public void UpdateCompositeLayerContent()
        {
            GfxHelper.instance.TriggerGfxThreadCallback(() =>
            {
                if (!Application.isEditor && (renderLayerId == -1 || textureHandle == -1 || !m_IsShowing ||
                                              m_IsRecreatingLayer || isExternalTexture)) return;
                SwapBufferLayer();
                CopyTextureToColorHandle();
            });
        }

        public void SwapBufferLayer()
        {
            if (!Application.isEditor && (renderLayerId == -1 || textureHandle == -1 || !m_IsShowing ||
                                          m_IsRecreatingLayer || isExternalTexture)) return;
            m_YVRLayerHandle.SwapBufferLayer(renderLayerId);
        }

        protected virtual void CopyTextureToColorHandle()
        {
            for (int i = 0; i < swapChainBufferCount; i++)
            {
                if (textureHandle == 0) return;
                int destTextureId = m_YVRLayerHandle.GetLayerColorHandle(renderLayerId, YVRRenderLayerEyeMask.kEyeMaskBoth,
                    i % swapChainBufferCount);
                CopyTexture(textureHandle, destTextureId, alpha);
                if (separateSwapChain)
                {
                    destTextureId = m_YVRLayerHandle.GetLayerColorHandle(renderLayerId,
                        YVRRenderLayerEyeMask.kEyeMaskRight, i % swapChainBufferCount);
                    CopyTexture(rightTextureHandle, destTextureId, alpha);
                }
            }
        }

        protected void CopyTexture(int sourceTextureId, int destTextureId, float alpha)
        {
            if (!isExternalTexture)
            {
                GfxHelper.instance.CopyTexture(sourceTextureId, destTextureId, alpha, new Rect(0, 0, 1, 1), new Rect(0, 0, 1, 1));
            }
        }

        public IntPtr GetExternalAndroidSurface()
        {
            if (!Application.isEditor && (!isExternalTexture || renderLayerId == -1)) return IntPtr.Zero;
            return m_YVRLayerHandle.GetLayerSurface(renderLayerId);
        }

        private void ApplyLayerSettings()
        {
            if (renderLayerId == -1) return;

            var sharpenType = YVRManager.instance.qualityManager.sharpenType;
            bool enableSuperSampling = sharpenType != YVRQualityManager.LayerSettingsType.None;
            bool expensiveSuperSampling = sharpenType == YVRQualityManager.LayerSettingsType.Quality;
            bool enableSharpen = sharpenType != YVRQualityManager.LayerSettingsType.None;
            bool expensiveSharpen = sharpenType == YVRQualityManager.LayerSettingsType.Quality;
            m_YVRLayerHandle.SetLayerSettings(renderLayerId, enableSuperSampling, expensiveSuperSampling, enableSharpen, expensiveSharpen);
        }

        protected void GetLayerExtensionData(ref Texture texture, ref Rect srcRect, ref Rect dstRect)
        {
            if (compositionLayer.LayerData == null)
                return;

            if (layerExtensions != null)
            {
                foreach (var extension in compositionLayer.Extensions)
                {
                    switch (extension)
                    {
                        case TexturesExtension texturesExtension:
                        {
                            int eyeIndex = 0;
                            isExternalTexture = texturesExtension.sourceTexture == TexturesExtension.SourceTextureEnum.AndroidSurface;
                            resolution = texturesExtension.Resolution;
                            if (texturesExtension.LeftTexture != null && texture != texturesExtension.LeftTexture)
                            {
                                eyeIndex = 0;
                                texture = texturesExtension.LeftTexture;
                            }

                            if (compositionLayer.LayerData.GetType() == typeof(ProjectionLayerData) ||
                                compositionLayer.LayerData.GetType() == typeof(ProjectionLayerRigData))
                            {
                                if (texturesExtension.RightTexture != null && rightEyeTexture != texturesExtension.RightTexture)
                                {
                                    eyeIndex = 1;
                                    rightEyeTexture = texturesExtension.RightTexture;
                                }
                            }
                            srcRect = eyeIndex == 0 ? texturesExtension.LeftEyeSourceRect : texturesExtension.RightEyeSourceRect;
                            dstRect = eyeIndex == 0 ? texturesExtension.LeftEyeDestinationRect : texturesExtension.RightEyeDestinationRect;
                            break;
                        }
                    }
                }
            }
        }

        protected void CreateYVRLayer()
        {
            var createInfo = new YVRLayerCreateInfo(
                order,
                width,
                height,
                swapChainBufferCount,
                textureType,
                2,
                separateSwapChain,
                Enabled,
                isExternalTexture,
                m_Shape,
                YVRRenderLayerEyeMask.kEyeMaskBoth,
                -1
            );

            if (width == 0 || height == 0) return;

            m_YVRLayerHandle.PrepareCreateLayerAsync(() =>
            {
                renderLayerId = m_YVRLayerHandle.CreateLayer(createInfo);
                OnLayerCreatedGfx(renderLayerId);
                OnLayerAddedGfx();
                ApplyLayerSettings();
            });
        }

        public void Destory()
        {
            m_YVRLayerHandle.PrepareDestroyLayerAsync(() =>
            {
                if (renderLayerId != -1)
                {
                    m_YVRLayerHandle.DestroyLayer(renderLayerId, true);
                }
            });

            Application.focusChanged -= OnFocusChanged;
        }
    }
}
#endif