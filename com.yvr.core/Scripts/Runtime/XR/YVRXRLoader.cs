using System;
using System.Collections.Generic;
using UnityEngine;
using System.Runtime.InteropServices;
using UnityEngine.Rendering;
using UnityEngine.XR;
using UnityEngine.XR.Management;
#if UNITY_INPUT_SYSTEM
using UnityEngine.InputSystem;
using UnityEngine.InputSystem.Layouts;
using UnityEngine.InputSystem.XR;
#endif

#if XR_HANDS
using UnityEngine.XR.Hands;
#endif

#if XR_ARFOUNDATION_5 || XR_ARFOUNDATION_6
using UnityEngine.XR.ARSubsystems;
using YVR.Core.ARFoundation.Anchor;
using YVR.Core.ARFoundation.Camera;
using YVR.Core.ARFoundation.Plane;
using YVR.Core.ARFoundation.Session;
using YVR.Core.ARFoundation.ImageTracking;
#endif

#if XR_COMPOSITION_LAYERS
using YVR.Core.XRCompositionLayers;
using Unity.XR.CompositionLayers.Services;
#endif

namespace YVR.Core.XR
{
    public class YVRXRLoader : XRLoaderHelper
    {
        [DllImport("yvrplugin")]
        private static extern void YVRSetXRUserDefinedSettings(ref YVRXRUserDefinedSettings userDefinedSettings);

        public static YVRXRUserDefinedSettings xrUserDefinedSettings = default;

        private static List<XRDisplaySubsystemDescriptor> displaySubsystemDescriptors
            = new List<XRDisplaySubsystemDescriptor>();

        private static List<XRInputSubsystemDescriptor> inputSubsystemDescriptors
            = new List<XRInputSubsystemDescriptor>();

        private static List<XRMeshSubsystemDescriptor> meshSubsystemDescriptors
            = new List<XRMeshSubsystemDescriptor>();
#if XR_HANDS
        private static List<XRHandSubsystemDescriptor> handSubsystemDescriptors
            = new List<XRHandSubsystemDescriptor>();
#endif

#if XR_ARFOUNDATION_5 || XR_ARFOUNDATION_6
        private static List<XRCameraSubsystemDescriptor> cameraSubsystemDescriptors = new();
        private static List<XRAnchorSubsystemDescriptor> anchorSubsystemDescriptors = new();
        private static List<XRSessionSubsystemDescriptor> sessionSubsystemDescriptors = new();
        private static List<XRPlaneSubsystemDescriptor> planeSubsystemDescriptors = new();
        private static List<XRImageTrackingSubsystemDescriptor> imageTrackingSubsystemDescriptors = new();
#endif

        public override bool Initialize()
        {
#if UNITY_INPUT_SYSTEM
            InputLayoutLoader.RegisterInputLayouts();
#endif

            var settings = YVRXRSettings.instance;

#if !UNITY_EDITOR && UNITY_ANDROID
            YVREventTracking.instance.UploadSDKInfo();
#endif
            xrUserDefinedSettings = new YVRXRUserDefinedSettings();
            xrUserDefinedSettings.stereoRenderingMode = settings.GetStereoRenderingMode();
            xrUserDefinedSettings.use16BitDepthBuffer = settings.use16BitDepthBuffer;
            xrUserDefinedSettings.useMonoscopic = settings.useMonoscopic;
            xrUserDefinedSettings.useLinearColorSpace = QualitySettings.activeColorSpace == ColorSpace.Linear;
            xrUserDefinedSettings.useAppSW = settings.useAppSW;
            xrUserDefinedSettings.enablePassthroughProvider = settings.passthroughProvider;
            xrUserDefinedSettings.autoResolve = settings.autoResolve;
            xrUserDefinedSettings.extraRenderPass = settings.extraRenderPass;
            xrUserDefinedSettings.extraRenderPassDepth = settings.extraRenderPassDepth;
            xrUserDefinedSettings.outerExtraPassRenderScale = settings.outerExtraPassRenderScale;
            xrUserDefinedSettings.innerExtraPassRenderScale = settings.innerExtraPassRenderScale;
            xrUserDefinedSettings.isP3 = settings.isP3;
            xrUserDefinedSettings.optimizeBufferDiscards = settings.optimizeBufferDiscards;
            xrUserDefinedSettings.outerPassRenderScale = settings.outerPassRenderScale;
            xrUserDefinedSettings.innerPassRenderScale = settings.innerPassRenderScale;
            YVRSetXRUserDefinedSettings(ref xrUserDefinedSettings);

            CreateSubsystem<XRDisplaySubsystemDescriptor, XRDisplaySubsystem>(displaySubsystemDescriptors, "Display");
            CreateSubsystem<XRInputSubsystemDescriptor, XRInputSubsystem>(inputSubsystemDescriptors, "Tracking");
            CreateSubsystem<XRMeshSubsystemDescriptor, XRMeshSubsystem>(meshSubsystemDescriptors, "Meshing");
#if XR_HANDS
            CreateSubsystem<XRHandSubsystemDescriptor, XRHandSubsystem>(handSubsystemDescriptors, "YVR Hands");
#endif

#if XR_ARFOUNDATION_5 || XR_ARFOUNDATION_6
            CreateSubsystem<XRCameraSubsystemDescriptor, XRCameraSubsystem>(cameraSubsystemDescriptors, YVRCameraSubsystem.k_SubsystemId);
            CreateSubsystem<XRAnchorSubsystemDescriptor, XRAnchorSubsystem>(anchorSubsystemDescriptors, YVRAnchorSubsystem.k_SubsystemId);
            CreateSubsystem<XRSessionSubsystemDescriptor, XRSessionSubsystem>(sessionSubsystemDescriptors, YVRSessionSubsystem.k_SubsystemId);
            CreateSubsystem<XRPlaneSubsystemDescriptor, XRPlaneSubsystem>(planeSubsystemDescriptors, YVRPlaneSubsystem.k_SubsystemId);
            CreateSubsystem<XRImageTrackingSubsystemDescriptor, XRImageTrackingSubsystem>(imageTrackingSubsystemDescriptors, YVRImageTrackingSubsystem.k_SubsystemId);
#endif
# if XR_COMPOSITION_LAYERS
            CompositionLayerManager.Instance.LayerProvider = new YVRCompositionLayerProvider();
#endif
            return true;
        }

        public override bool Start()
        {
            StartSubsystem<XRDisplaySubsystem>();
            StartSubsystem<XRInputSubsystem>();
            StartSubsystem<XRMeshSubsystem>();

#if XR_HANDS
            StartSubsystem<XRHandSubsystem>();
#endif
#if XR_ARFOUNDATION_5 || XR_ARFOUNDATION_6
            StartSubsystem<XRCameraSubsystem>();
            StartSubsystem<XRAnchorSubsystem>();
            StartSubsystem<XRSessionSubsystem>();
            StartSubsystem<XRPlaneSubsystem>();
            StartSubsystem<XRImageTrackingSubsystem>();
#endif

            return true;
        }

        public override bool Stop()
        {
            StopSubsystem<XRDisplaySubsystem>();
            StopSubsystem<XRInputSubsystem>();
            StopSubsystem<XRMeshSubsystem>();

#if XR_HANDS
            StopSubsystem<XRHandSubsystem>();
#endif
#if XR_ARFOUNDATION_5 || XR_ARFOUNDATION_6
            StopSubsystem<XRCameraSubsystem>();
            StopSubsystem<XRAnchorSubsystem>();
            StopSubsystem<XRSessionSubsystem>();
            StopSubsystem<XRPlaneSubsystem>();
            StopSubsystem<XRImageTrackingSubsystem>();
#endif
            return true;
        }

        public override bool Deinitialize()
        {
            DestroySubsystem<XRDisplaySubsystem>();
            DestroySubsystem<XRInputSubsystem>();
            DestroySubsystem<XRMeshSubsystem>();

#if XR_HANDS
            DestroySubsystem<XRHandSubsystem>();
#endif
#if XR_ARFOUNDATION_5 || XR_ARFOUNDATION_6
            DestroySubsystem<XRCameraSubsystem>();
            DestroySubsystem<XRAnchorSubsystem>();
            DestroySubsystem<XRSessionSubsystem>();
            DestroySubsystem<XRPlaneSubsystem>();
            DestroySubsystem<XRImageTrackingSubsystem>();
#endif
            return true;
        }
    }
}