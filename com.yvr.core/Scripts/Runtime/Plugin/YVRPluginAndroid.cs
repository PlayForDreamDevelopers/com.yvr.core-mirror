using UnityEngine;
using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine.Internal;
using Unity.Profiling;
using YVR.Core.ImageTracking;

namespace YVR.Core
{
    [ExcludeFromDocs]
    public class YVRPluginAndroid : YVRPlugin
    {
        [DllImport("yvrplugin")]
        private static extern void YVRSetVSyncCount(int vSyncCount);

        [DllImport("yvrplugin")]
        private static extern void YVRRecenterPose();

        [DllImport("yvrplugin")]
        private static extern int YVRSetTrackingSpace(int trackingSpace);

        [DllImport("yvrplugin")]
        private static extern void YVRSetControllerVibration(uint controllerMask, float frequency, float amplitude);

        [DllImport("yvrplugin")]
        private static extern void YVRSetControllerVibrationWithDuration(
            uint controllerMask, float frequency, float amplitude,
            float duration);

        [DllImport("yvrplugin")]
        private static extern float YVRGetBatteryLevel();

        [DllImport("yvrplugin")]
        private static extern float YVRGetBatteryTemperature();

        [DllImport("yvrplugin")]
        private static extern int YVRGetBatteryStatus();

        [DllImport("yvrplugin")]
        private static extern float YVRGetVolumeLevel();

        [DllImport("yvrplugin")]
        private static extern float YVRGetGpuUtilization();

        [DllImport("yvrplugin")]
        private static extern float YVRGetCpuUtilization();

        [DllImport("yvrplugin")]
        private static extern void YVRSetPerformanceLevel(int cpulevel, int gpuLevel);

        [DllImport("yvrplugin")]
        private static extern int YVRGetCpuLevel();

        [DllImport("yvrplugin")]
        private static extern int YVRGetGpuLevel();

        [DllImport("yvrplugin")]
        private static extern void YVRSetUsingIPDInPositionTracking(bool usingIPD);

        [DllImport("yvrplugin")]
        private static extern float YVRGetDisplayFrequency();

        [DllImport("yvrplugin")]
        private static extern void YVRSetDisplayFrequency(float freshRate);

        [DllImport("yvrplugin")]
        private static extern int YVRGetDisplayAvailableFrequenciesNum();

        [DllImport("yvrplugin")]
        private static extern void YVRGetDisplayAvailableFrequencies(float[] frequenciesArray);

        [DllImport("yvrplugin")]
        private static extern void YVRGetEyeResolution(ref Vector2 resolution);

        [DllImport("yvrplugin")]
        private static extern void YVRGetEyeFov(int eyeSide, ref YVRCameraRenderer.EyeFov eyeFov);

        [DllImport("yvrplugin")]
        private static extern bool YVRIsUserPresent();

        [DllImport("yvrplugin")]
        private static extern bool YVRIsFocusing();

        [DllImport("yvrplugin")]
        private static extern bool YVRIsVisible();

        [DllImport("yvrplugin")]
        private static extern void YVRSetEventCallback(Action<int> callback);

        [DllImport("yvrplugin")]
        private static extern bool YVRGetBoundaryConfigured();

        [DllImport("yvrplugin")]
        private static extern void YVRTestBoundaryNode(YVRBoundary.BoundaryNode targetNode,
                                                       ref YVRBoundary.BoundaryTestResult testResult);

        [DllImport("yvrplugin")]
        private static extern void YVRTestBoundaryPoint(Vector3 targetPoint,
                                                        ref YVRBoundary.BoundaryTestResult testResult);

        [DllImport("yvrplugin")]
        private static extern Vector3 YVRGetBoundaryDimensions();

        [DllImport("yvrplugin")]
        private static extern bool YVRGetBoundaryVisible();

        [DllImport("yvrplugin")]
        private static extern void YVRSetBoundaryVisible(bool visible);

        [DllImport("yvrplugin")]
        private static extern int YVRGetBoundaryGeometryPointsCount();

        [DllImport("yvrplugin")]
        private static extern void YVRGetBoundaryGeometry(Vector3[] geometry);

        [DllImport("yvrplugin")]
        private static extern void YVRSetFoveation(int ffrLevel, int ffrDynamic);

        [DllImport("yvrplugin")]
        private static extern void YVRSetAppSWEnable(bool enable);

        [DllImport("yvrplugin")]
        private static extern bool YVRGetAppSWEnable();

        [DllImport("yvrplugin")]
        private static extern void YVRGetPrimaryController(ref uint controllerMask);

        [DllImport("yvrplugin")]
        private static extern void YVRSetPrimaryController(uint controllerMask);

        [DllImport("yvrplugin")]
        private static extern void YVRSetBoundaryDetectionEnable(bool enable);

        [DllImport("yvrplugin")]
        private static extern void YVRGetHandHandJointLocations(HandType handType,
                                                                ref HandData handData, IntPtr handJointLocationsPtr,
                                                                IntPtr jointVelocitiesHandle);

        [DllImport("yvrplugin")]
        private static extern void YVRGetCurrentInputDevice(ref ActiveInputDevice inputDevice);

        [DllImport("yvrplugin")]
        private static extern bool YVRGetHandEnable();

        [DllImport("yvrplugin")]
        private static extern int YVRGetHandAutoActivateTime();

        [DllImport("yvrplugin")]
        private static extern void YVRSetAPPHandEnable(bool enable);

        [DllImport("yvrplugin")]
        private static extern void YVRSetAPPControllerEnable(bool enable);

        [DllImport("yvrplugin")]
        public static extern void YVRSetPassthroughVisible(bool visible);

        [DllImport("yvrplugin")]
        public static extern void YVRSetAppSpacePosition(float x, float y, float z);

        [DllImport("yvrplugin")]
        public static extern void YVRSetAppSpaceRotation(float x, float y, float z, float w);

        [DllImport("yvrplugin")]
        public static extern void YVRSetAppSWSwitch(bool isOn);

        [DllImport("yvrplugin")]
        public static extern bool YVRGetAppSWSwitch();

        [DllImport("yvrplugin")]
        public static extern void yvrSetEyeBufferSettings(bool enableSuperSample, bool expensiveSuperSample,
                                                          bool enableSharpen, bool expensiveSharpen);

        [DllImport("yvrplugin")]
        public static extern void YVRCreateSpatialAnchor(Vector3 position, Quaternion rotation, ref UInt64 requestId);

        [DllImport("yvrplugin")]
        public static extern bool YVRGetSpatialAnchorPose(UInt64 spatialId, ref Vector3 position,
                                                          ref Quaternion rotation,
                                                          ref YVRAnchorLocationFlags locationFlags);

        [DllImport("yvrplugin")]
        public static extern void YVRSetCreateSpatialAnchorCallback(Action<YVRSpatialAnchorResult, bool> callback);

        [DllImport("yvrplugin")]
        public static extern void YVRQuerySpaces(YVRSpatialAnchorQueryInfo queryInfo, ref UInt64 requestId);

        [DllImport("yvrplugin")]
        public static extern void YVRSaveSpatialAnchor(YVRSpatialAnchorSaveInfo saveInfo, ref UInt64 requestId);

        [DllImport("yvrplugin")]
        public static extern void
            YVRSetQuerySpatialAnchorCallback(Action<YVRQuerySpatialAnchorResult, UInt64> callback);

        [DllImport("yvrplugin")]
        public static extern void YVRDestroySpatialAnchor(UInt64 space, YVRSpatialAnchorStorageLocation location,
                                                          ref UInt64 requestId);

        [DllImport("yvrplugin")]
        public static extern void YVRSetEraseSpatialAnchorCallback(Action<YVRSpatialAnchorResult, bool> callback);

        [DllImport("yvrplugin")]
        public static extern void YVRGetEnumerateSpaceSupported(UInt64 space,
                                                                ref YVRSpatialAnchorSupportedComponent component);

        [DllImport("yvrplugin")]
        public static extern bool YVRSetSpaceComponentStatus(UInt64 space,
                                                             YVRSpatialAnchorComponentStatusSetInfo statusSetInfo,
                                                             ref UInt64 requestId);

        [DllImport("yvrplugin")]
        public static extern void YVRGetSpaceComponentStatus(UInt64 space, YVRSpatialAnchorComponentType component,
                                                             ref YVRSpatialAnchorComponentStatus status);

        [DllImport("yvrplugin")]
        public static extern void YVRCreateSpaceUser(UInt64 userId, ref UInt64 spaceUser);

        [DllImport("yvrplugin")]
        public static extern void YVRShareSpace(YVRSpatialAnchorShareInfo shareInfo, ref UInt64 requestId);

        [DllImport("yvrplugin")]
        public static extern void YVRGetSpaceUserId(UInt64 spaceUser, ref UInt64 userId);

        [DllImport("yvrplugin")]
        public static extern void YVRSetSpaceShareCompleteCallback(Action<bool, UInt64> callback);

        [DllImport("yvrplugin")]
        public static extern void YVRSetSpaceSaveCompleteCallback(
            Action<YVRSpatialAnchorSaveCompleteInfo, bool> callback);

        [DllImport("yvrplugin")]
        public static extern void YVRSetSpaceListSaveCompleteCallback(Action<bool, UInt64> callback);

        [DllImport("yvrplugin")]
        public static extern void YVRSetSpaceSetStatusCompleteCallback(
            Action<YVRSpatialAnchorSetStatusCompleteInfo, bool> callback);

        [DllImport("yvrplugin")]
        public static extern void YVRSaveSpaceList(YVRSpatialAnchorListSaveInfo listSaveInfo, ref UInt64 requestId);

        [DllImport("yvrplugin")]
        public static extern void YVRGetSpaceHandleUuid(ulong anchorHandle, ref YVRSpatialAnchorUUID uuid);

        [DllImport("yvrplugin")]
        public static extern void YVRCreatePlaneDetection();

        [DllImport("yvrplugin")]
        public static extern void YVRSetPlaneDetectionsCallback(Action<YVRPlaneDetectorLocationsInternal> action);

        [DllImport("yvrplugin")]
        public static extern IntPtr YVRGetPolygonBuffer(ulong planeId, uint count);

        [DllImport("yvrplugin")]
        public static extern void YVREndPlaneDetection();

        [DllImport("yvrplugin")]
        public static extern void YVRDeletePlanePtr();

        [DllImport("yvrplugin")]
        public static extern bool YVRGetEyeTrackingSupports();

        [DllImport("yvrplugin")]
        public static extern bool YVRGetEyeTrackingEnable();

        [DllImport("yvrplugin")]
        public static extern int YVRGetSpaceBoundingBox2D(ulong anchorHandle, ref YVRRect2D boundingBox2D);

        [DllImport("yvrplugin")]
        public static extern int YVRGetSpaceBoundingBox3D(ulong anchorHandle, ref YVRRect3D boundingBox3D);

        [DllImport("yvrplugin")]
        public static extern int YVRGetSpaceBoundary2D(ulong anchorHandle, ref YVRBoundary2D boundary2D);

        [DllImport("yvrplugin")]
        public static extern int YVRGetSpaceSemanticLabels(ulong anchorHandle,
                                                           ref YVRAnchorSemanticLabel anchorSemanticLabel);

        [DllImport("yvrplugin")]
        public static extern int YVRGetSpaceRoomLayout(ulong anchorHandle, ref YVRRoomLayout roomLayout);

        [DllImport("yvrplugin")]
        public static extern int YVRGetSpaceContainer(ulong anchorHandle,
                                                      ref YVRSceneAnchorContainer sceneAnchorContainer);

        [DllImport("yvrplugin")]
        public static extern int YVRRequestSceneCapture(ref YVRSceneCaptureRequest requestString, ref ulong requestId);

        [DllImport("yvrplugin")]
        public static extern void YVRSetRequestSceneCaptureCallback(Action<ulong, bool> callback);

        [DllImport("yvrplugin")]
        public static extern int YVRGetSpaceTriangleMesh(ulong anchorHandle,
                                                         ref YVRAnchorTriangleMeshInternal anchorTriangleMesh);

        [DllImport("yvrplugin")]
        public static extern bool YVRGetRecommendedResolution(ref YVRExtent2DInt outRecommendedResolution);

        [DllImport("yvrplugin")]
        public static extern void YVRSetAdapterResolutionPolicy(
            YVRQualityManager.AdapterResolutionPolicy adapterResolutionPolicy);

        [DllImport("yvrplugin")]
        public static extern void YVRCreateMeshDetector();

        [DllImport("yvrplugin")]
        public static extern void YVRDestroyMeshDetector();

        [DllImport("yvrplugin")]
        public static extern void YVRSetMeshBlockUpdateCallback(
            Action<ulong, YVRMeshBlockChangeState> meshBlockChangeState);

        [DllImport("yvrplugin")]
        public static extern int YVRGetPassthroughSwapchainImageIndex();

        [DllImport("yvrplugin")]
        public static extern bool YVRGetPassthroughSwapchainImageValid();

        [DllImport("yvrplugin")]
        public static extern Quaternion YVRGetPassthroughImageDiffRotation(int eyeIndex);


        [DllImport("yvrplugin")]
        public static extern StereoRenderingMode YVRGetActualStereoRenderingMode();

        [DllImport("yvrplugin")]
        private static extern void YVRSetBlockInteractionData(bool isblock);

        [DllImport("yvrplugin")]
        private static extern bool YVRGetBlockInteractionData();

        [DllImport("yvrplugin")]
        public static extern void YVRSetPassthroughProviderEnable(bool enable);

        [DllImport("yvrplugin")]
        public static extern void YVRSetSessionStateChangeCallback(Action<int> callback);

        [DllImport("yvrplugin")]
        public static extern void YVRPollEvent();

        [DllImport("yvrplugin")]
        public static extern void YVRSwitchImageTracking(bool enable);

        [DllImport("yvrplugin")]
        public static extern void YVRRegisterImageTemplate(ImageTemplateInfo imageTemplateInfo);

        [DllImport("yvrplugin", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        public static extern void YVRUnRegisterImageTemplate(string imageTemplateInfo);

        [DllImport("yvrplugin")]
        public static extern void YVRSetImageTrackingUpdateCallback(Action<TrackedImageInfo> imageInfo);

        [DllImport("yvrplugin")]
        public static extern bool YVRIsPassthroughInitialized();

        [DllImport("yvrplugin")]
        public static extern int YVRSetPassthroughStyle(ref PassthroughStyle passthroughStyle);

        [DllImport("yvrplugin")]
        public static extern int YVRCreatePassthroughColorLut(PassthroughColorLutChannels channels, UInt32 resolution,
                                                              PassthroughColorLutData data, out UInt64 colorLut);

        [DllImport("yvrplugin")]
        public static extern int YVRDestroyPassthroughColorLut(UInt64 colorLut);

        [DllImport("yvrplugin")]
        public static extern int YVRUpdatePassthroughColorLut(UInt64 colorLut, PassthroughColorLutData data);

        [DllImport("yvrplugin")]
        private static extern float YVRGetIPD();


        //---------------------------------------------------------------------------------------------
        public static bool IsSuccess(int result) => result >= 0;

        public static YVRPluginAndroid Create() { return new YVRPluginAndroid(); }

        public override void SetTrackingSpace(YVRTrackingStateManager.TrackingSpace trackingSpace)
        {
            YVRSetTrackingSpace((int) trackingSpace);
        }

        public override StereoRenderingMode GetStereoRenderingMode() { return YVRGetActualStereoRenderingMode(); }

        public override void SetVSyncCount(YVRQualityManager.VSyncCount vSyncCount)
        {
            YVRSetVSyncCount((int) vSyncCount);
        }

        public override void RecenterTracking() { YVRRecenterPose(); }

        public override void SetControllerVibration(uint controllerMask, float frequency, float amplitude)
        {
            YVRSetControllerVibration(controllerMask, frequency, amplitude);
        }

        public override void SetControllerVibration(uint controllerMask, float frequency, float amplitude,
                                                    float duration)
        {
            YVRSetControllerVibrationWithDuration(controllerMask, frequency, amplitude, duration);
        }


        public override float GetBatteryLevel() { return YVRGetBatteryLevel(); }

        public override float GetBatteryTemperature() { return YVRGetBatteryTemperature(); }

        public override int GetBatteryStatus() { return YVRGetBatteryStatus(); }

        public override float GetVolumeLevel() { return YVRGetVolumeLevel(); }

        public override float GetGPUUtilLevel() { return YVRGetGpuUtilization(); }

        public override float GetCPUUtilLevel() { return YVRGetCpuUtilization(); }

        public override int GetCPULevel() { return YVRGetCpuLevel(); }

        public override int GetGPULevel() { return YVRGetGpuLevel(); }

        public override void SetPerformanceLevel(int cpuLevel, int gpuLevel)
        {
            YVRSetPerformanceLevel(cpuLevel, gpuLevel);
        }

        public override void GetEyeResolution(ref Vector2 resolution) { YVRGetEyeResolution(ref resolution); }

        public override void GetEyeFov(int eyeSide, ref YVRCameraRenderer.EyeFov eyeFov)
        {
            YVRGetEyeFov(eyeSide, ref eyeFov);
        }

        private float[] m_CacheAvailableFrequencies = null;

        public override float[] GetDisplayFrequenciesAvailable()
        {
            if (m_CacheAvailableFrequencies == null)
            {
                int availableFrequenciesNum = YVRGetDisplayAvailableFrequenciesNum();
                m_CacheAvailableFrequencies = new float[availableFrequenciesNum];
                YVRGetDisplayAvailableFrequencies(m_CacheAvailableFrequencies);
            }

            return m_CacheAvailableFrequencies;
        }

        public override float GetDisplayFrequency() { return YVRGetDisplayFrequency(); }

        public override void SetDisplayFrequency(float displayFrequency) { YVRSetDisplayFrequency(displayFrequency); }

        public override bool IsUserPresent() { return YVRIsUserPresent(); }

        public override void SetPassthrough(bool enable) { YVRSetPassthroughVisible(enable); }

        public override void SetPassthroughProviderEnable(bool enable) { YVRSetPassthroughProviderEnable(enable); }

        public override bool IsFocusing() { return YVRIsFocusing(); }

        public override bool IsVisible() { return YVRIsVisible(); }

        public override void SetEventCallback(Action<int> eventCallback) { YVRSetEventCallback(eventCallback); }

        public override bool GetBoundaryConfigured() { return YVRGetBoundaryConfigured(); }

        public override void TestBoundaryNode(YVRBoundary.BoundaryNode targetNode,
                                              ref YVRBoundary.BoundaryTestResult testResult)
        {
            YVRTestBoundaryNode(targetNode, ref testResult);
        }

        public override void TestBoundaryPoint(Vector3 targetPoint, ref YVRBoundary.BoundaryTestResult testResult)
        {
            YVRTestBoundaryPoint(targetPoint, ref testResult);
        }

        public override Vector3 GetBoundaryDimensions() { return YVRGetBoundaryDimensions(); }

        public override bool GetBoundaryVisible() { return YVRGetBoundaryVisible(); }

        public override void SetBoundaryVisible(bool visible) { YVRSetBoundaryVisible(visible); }

        public override void SetBoundaryDetectionEnable(bool enable) { YVRSetBoundaryDetectionEnable(enable); }

        public override Vector3[] GetBoundaryGeometry()
        {
            int pointsCount = YVRGetBoundaryGeometryPointsCount();
            Vector3[] result = new Vector3[pointsCount];
            if (pointsCount > 0)
                YVRGetBoundaryGeometry(result);

            return result;
        }

        public override void SetFoveation(int ffrLevel, int ffrDynamic) { YVRSetFoveation(ffrLevel, ffrDynamic); }

        public override void SetAppSWEnable(bool enable) { YVRSetAppSWEnable(enable); }

        public override bool GetAppSWEnable() { return YVRGetAppSWEnable(); }

        public override void GetPrimaryController(ref uint controllerMask)
        {
            YVRGetPrimaryController(ref controllerMask);
        }

        public override void SetPrimaryController(uint controllerMask) { YVRSetPrimaryController(controllerMask); }


        private GCHandle m_LJointLocationsHandle;
        private GCHandle m_RJointLocationsHandle;
        private GCHandle m_LJointVelocitiesHandle;
        private GCHandle m_RJointVelocitiesHandle;
        private ProfilerMarker m_HandProfilerMarker = new ProfilerMarker("GetHandJointLocations");

        public override void GetHandJointLocations(HandType handType, ref HandJointLocations jointLocations)
        {
            using (m_HandProfilerMarker.Auto())
            {
                if (jointLocations.jointLocations == null || jointLocations.jointVelocities == null)
                {
                    jointLocations.jointLocations = new HandJointLocation[(int) HandJoint.JointMax];
                    jointLocations.jointVelocities = new HandJointVelocity[(int) HandJoint.JointMax];
                }

                if (handType == HandType.HandLeft)
                {
                    m_LJointLocationsHandle = GCHandle.Alloc(jointLocations.jointLocations, GCHandleType.Pinned);
                    m_LJointVelocitiesHandle = GCHandle.Alloc(jointLocations.jointVelocities, GCHandleType.Pinned);
                }
                else if (handType == HandType.HandRight)
                {
                    m_RJointLocationsHandle = GCHandle.Alloc(jointLocations.jointLocations, GCHandleType.Pinned);
                    m_RJointVelocitiesHandle = GCHandle.Alloc(jointLocations.jointVelocities, GCHandleType.Pinned);
                }

                HandData handData = new HandData();
                YVRGetHandHandJointLocations(handType, ref handData,
                                             (handType == HandType.HandLeft
                                                 ? m_LJointLocationsHandle
                                                 : m_RJointLocationsHandle)
                                            .AddrOfPinnedObject(),
                                             (handType == HandType.HandLeft
                                                 ? m_LJointVelocitiesHandle
                                                 : m_RJointVelocitiesHandle)
                                            .AddrOfPinnedObject());
                jointLocations.aimState = handData.aimState;
                jointLocations.jointCount = handData.jointCount;
                jointLocations.handScale = handData.handScale;
                jointLocations.isActive = handData.isActive;

                if (handType == HandType.HandLeft)
                {
                    m_LJointLocationsHandle.Free();
                    m_LJointVelocitiesHandle.Free();
                }
                else if (handType == HandType.HandRight)
                {
                    m_RJointLocationsHandle.Free();
                    m_RJointVelocitiesHandle.Free();
                }
            }
        }

        public override void GetCurrentInputDevice(ref ActiveInputDevice inputDevice)
        {
            YVRGetCurrentInputDevice(ref inputDevice);
        }

        public override bool GetHandEnable() { return YVRGetHandEnable(); }

        public override int GetHandAutoActivateTime() { return YVRGetHandAutoActivateTime(); }

        public override void SetAPPHandEnable(bool enable) { YVRSetAPPHandEnable(enable); }

        public override void SetAPPControllerEnable(bool enable) { YVRSetAPPControllerEnable(enable); }

        public override void SetAppSpacePosition(float x, float y, float z) { YVRSetAppSpacePosition(x, y, z); }

        public override void SetAppSpaceRotation(float x, float y, float z, float w)
        {
            YVRSetAppSpaceRotation(x, y, z, w);
        }

        public override void SetAppSWSwitch(bool isOn) { YVRSetAppSWSwitch(isOn); }

        public override bool GetAppSWSwitch() { return YVRGetAppSWSwitch(); }

        public override void SetEyeBufferLayerSettings(bool enableSuperSample, bool expensiveSuperSample,
                                                       bool enableSharpen, bool expensiveSharpen)
        {
            yvrSetEyeBufferSettings(enableSuperSample, expensiveSuperSample, enableSharpen, expensiveSharpen);
        }

        public override void CreateSpatialAnchor(Vector3 position, Quaternion rotation, ref UInt64 requestId)
        {
            YVRCreateSpatialAnchor(position, rotation, ref requestId);
        }

        public override bool GetSpatialAnchorPose(UInt64 spatialId, ref Vector3 position, ref Quaternion rotation,
                                                  ref YVRAnchorLocationFlags locationFlags)
        {
            return YVRGetSpatialAnchorPose(spatialId, ref position, ref rotation, ref locationFlags);
        }

        public override void SetCreateSpatialAnchorCallback(Action<YVRSpatialAnchorResult, bool> callback)
        {
            YVRSetCreateSpatialAnchorCallback(callback);
        }

        public override void SaveSpatialAnchor(YVRSpatialAnchorSaveInfo saveInfo, ref ulong requestId)
        {
            YVRSaveSpatialAnchor(saveInfo, ref requestId);
        }

        public override void QuerySpatialAnchor(YVRSpatialAnchorQueryInfo queryInfo, ref ulong requestId)
        {
            YVRQuerySpaces(queryInfo, ref requestId);
        }

        public override void SetQuerySpatialAnchorCallback(Action<YVRQuerySpatialAnchorResult, UInt64> callback)
        {
            YVRSetQuerySpatialAnchorCallback(callback);
        }

        public override void DestroySpatialAnchor(UInt64 space, YVRSpatialAnchorStorageLocation location,
                                                  ref UInt64 requestId)
        {
            YVRDestroySpatialAnchor(space, location, ref requestId);
        }

        public override void SetEraseSpatialAnchorCallback(Action<YVRSpatialAnchorResult, bool> callback)
        {
            YVRSetEraseSpatialAnchorCallback(callback);
        }

        public override void GetSpatialAnchorEnumerateSupported(UInt64 space,
                                                                ref YVRSpatialAnchorSupportedComponent components)
        {
            YVRGetEnumerateSpaceSupported(space, ref components);
            YVRSpatialAnchorComponentType[]
                componentTypes = new YVRSpatialAnchorComponentType[components.numComponents];
            for (int i = 0; i < components.numComponents; i++)
            {
                componentTypes[i] = components.components[i];
            }

            components.components = componentTypes;
        }

        public override bool SetSpatialAnchorComponentStatus(UInt64 space,
                                                             YVRSpatialAnchorComponentStatusSetInfo statusSetInfo,
                                                             ref UInt64 requestId)
        {
            return YVRSetSpaceComponentStatus(space, statusSetInfo, ref requestId);
        }

        public override void GetSpatialAnchorComponentStatus(UInt64 space, YVRSpatialAnchorComponentType componentType,
                                                             ref YVRSpatialAnchorComponentStatus status)
        {
            YVRGetSpaceComponentStatus(space, componentType, ref status);
        }

        public override void CreateSpatialAnchorUserHandle(UInt64 userId, ref UInt64 spaceUser)
        {
            YVRCreateSpaceUser(userId, ref spaceUser);
        }

        public override void ShareSpatialAnchor(YVRSpatialAnchorShareInfo shareInfo, ref UInt64 requestId)
        {
            YVRShareSpace(shareInfo, ref requestId);
        }

        public override void GetSpatialAnchorUserId(UInt64 spaceUser, ref UInt64 userId)
        {
            YVRGetSpaceUserId(spaceUser, ref userId);
        }

        public override void SetSpatialAnchorShareCompleteCallback(Action<bool, UInt64> callback)
        {
            YVRSetSpaceShareCompleteCallback(callback);
        }

        public override void SetSpatialAnchorSaveCompleteCallback(
            Action<YVRSpatialAnchorSaveCompleteInfo, bool> callback)
        {
            YVRSetSpaceSaveCompleteCallback(callback);
        }

        public override void SetSpatialAnchorSaveListCompleteCallback(Action<bool, ulong> callback)
        {
            YVRSetSpaceListSaveCompleteCallback(callback);
        }

        public override void SetSpatialAnchorStatusCompleteCallback(
            Action<YVRSpatialAnchorSetStatusCompleteInfo, bool> callback)
        {
            YVRSetSpaceSetStatusCompleteCallback(callback);
        }

        public override void SaveSpatialAnchorList(YVRSpatialAnchorListSaveInfo listSaveInfo, ref UInt64 requestId)
        {
            YVRSaveSpaceList(listSaveInfo, ref requestId);
        }

        public override void GetSpatialAnchorUUID(ulong anchorHandle, ref YVRSpatialAnchorUUID uuid)
        {
            YVRGetSpaceHandleUuid(anchorHandle, ref uuid);
        }

        public override void CreatePlaneDetection() { YVRCreatePlaneDetection(); }

        public override void SetPlaneDetectionsCallback(Action<YVRPlaneDetectorLocationsInternal> action)
        {
            YVRSetPlaneDetectionsCallback(action);
        }

        public override IntPtr GetPolygonBuffer(ulong planeId, uint count)
        {
            return YVRGetPolygonBuffer(planeId, count);
        }

        public override void EndPlaneDetection() { YVREndPlaneDetection(); }

        public override bool GetEyeTrackingSupportes() { return YVRGetEyeTrackingSupports(); }

        public override bool GetEyeTrackingEnable() { return YVRGetEyeTrackingEnable(); }

        public override int GetSpaceBoundingBox2D(ulong anchorHandle, ref YVRRect2D boundingBox2D)
        {
            return YVRGetSpaceBoundingBox2D(anchorHandle, ref boundingBox2D);
        }

        public override int GetSpaceBoundingBox3D(ulong anchorHandle, ref YVRRect3D boundingBox3D)
        {
            return YVRGetSpaceBoundingBox3D(anchorHandle, ref boundingBox3D);
        }

        public override int GetSpaceBoundary2D(ulong anchorHandle, ref YVRBoundary2D boundary2D)
        {
            return YVRGetSpaceBoundary2D(anchorHandle, ref boundary2D);
        }

        public override int GetSpaceSemanticLabels(ulong anchorHandle, ref YVRAnchorSemanticLabel anchorSemanticLabel)
        {
            return YVRGetSpaceSemanticLabels(anchorHandle, ref anchorSemanticLabel);
        }

        public override int GetSpaceRoomLayout(ulong anchorHandle, ref YVRRoomLayout roomLayout)
        {
            return YVRGetSpaceRoomLayout(anchorHandle, ref roomLayout);
        }

        public override int GetSpaceContainer(ulong anchorHandle, ref YVRSceneAnchorContainer sceneAnchorContainer)
        {
            return YVRGetSpaceContainer(anchorHandle, ref sceneAnchorContainer);
        }

        public override int RequestSceneCapture(ref YVRSceneCaptureRequest requestString, ref ulong requestId)
        {
            return YVRRequestSceneCapture(ref requestString, ref requestId);
        }

        public override void SetSceneCaptureCallback(Action<ulong, bool> callback)
        {
            YVRSetRequestSceneCaptureCallback(callback);
        }

        public override int GetSpaceTriangleMesh(ulong anchorHandle,
                                                 ref YVRAnchorTriangleMeshInternal anchorTriangleMesh)
        {
            return YVRGetSpaceTriangleMesh(anchorHandle, ref anchorTriangleMesh);
        }

        public override bool GetRecommendedResolution(ref YVRExtent2DInt outRecommendedResolution)
        {
            return YVRGetRecommendedResolution(ref outRecommendedResolution);
        }

        public override void SetAdapterResolutionPolicy(
            YVRQualityManager.AdapterResolutionPolicy adapterResolutionPolicy)
        {
            YVRSetAdapterResolutionPolicy(adapterResolutionPolicy);
        }

        public override void CreateMeshDetector() { YVRCreateMeshDetector(); }

        public override void DestroyMeshDetector() { YVRDestroyMeshDetector(); }

        public override void SetMeshBlockUpdateCallback(Action<ulong, YVRMeshBlockChangeState> meshBlockChangeState)
        {
            YVRSetMeshBlockUpdateCallback(meshBlockChangeState);
        }

        public override int GetPassthroughSwapchainImageIndex() { return YVRGetPassthroughSwapchainImageIndex(); }

        public override bool GetPassthroughSwapchainImageValid() { return YVRGetPassthroughSwapchainImageValid(); }

        public override Quaternion GetPassthroughImageDiffRotation(int eyeIndex)
        {
            return YVRGetPassthroughImageDiffRotation(eyeIndex);
        }

        public override void SetBlockInteractionData(bool isBlock) { YVRSetBlockInteractionData(isBlock); }

        public override bool GetBlockInteractionData() { return YVRGetBlockInteractionData(); }

        public override void SetSessionStateChangeCallback(Action<int> state) =>
            YVRSetSessionStateChangeCallback(state);

        public override void PollEvent() { YVRPollEvent(); }

        public override void SwitchImageTracking(bool enable) { YVRSwitchImageTracking(enable); }

        public override void RegisterImageTemplate(ImageTemplateInfo imageTemplateInfo)
        {
            YVRRegisterImageTemplate(imageTemplateInfo);
        }

        public override void UnRegisterImageTemplate(string imageTemplateInfo)
        {
            YVRUnRegisterImageTemplate(imageTemplateInfo);
        }

        public override void SetImageTrackingUpdateCallback(Action<TrackedImageInfo> callback)
        {
            YVRSetImageTrackingUpdateCallback(callback);
        }

        public override bool SetPassthroughStyle(PassthroughStyle style)
        {
            return IsSuccess(YVRSetPassthroughStyle(ref style));
        }

        public override bool CreatePassthroughColorLut(PassthroughColorLutChannels channels, UInt32 resolution,
                                                       PassthroughColorLutData data, out UInt64 colorLut)
        {
            var xrResult = YVRCreatePassthroughColorLut(channels, resolution, data, out colorLut);
            return IsSuccess(xrResult);
        }

        public override bool DestroyPassthroughColorLut(ulong colorLut)
        {
            return IsSuccess(YVRDestroyPassthroughColorLut(colorLut));
        }

        public override bool UpdatePassthroughColorLut(ulong colorLut, PassthroughColorLutData data)
        {
            return IsSuccess(YVRUpdatePassthroughColorLut(colorLut, data));
        }

        public override float GetIPD() { return YVRGetIPD(); }
    }
}