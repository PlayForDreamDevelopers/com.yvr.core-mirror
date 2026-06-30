LOCAL_PATH := $(call my-dir)
THIS_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)
LOCAL_MODULE := libopenxr_loader
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../../OpenXRLoader/openxr_loader/Libs/$(TARGET_ARCH_ABI)/libopenxr_loader.so
include $(PREBUILT_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_PATH := $(THIS_PATH)
LOCAL_MODULE    := yvrplugin

LOCAL_SRC_FILES := ../cpp/YVRUnityExportAPI.cpp \
               ../cpp/YVRLayerApisExport.cpp \
               ../cpp/YVRUnityExportAPI_Internal.cpp \
               ../cpp/Utilities/YVRDebug.cpp \
               ../cpp/Utilities/YVRLog.cpp \
               ../cpp/YVRUnityXR/YVRProviderBase.cpp \
               ../cpp/YVRUnityXR/YVRDisplayProvider.cpp \
               ../cpp/YVRUnityXR/YVRInputProvider.cpp \
               ../cpp/YVRUnityXR/InputDevices/InputDeviceBase.cpp \
               ../cpp/YVRUnityXR/InputDevices/ControllerInputDevice.cpp \
               ../cpp/YVRUnityXR/InputDevices/HMDInputDevice.cpp \
               ../cpp/YVRUnityXR/InputDevices/EyeTrackingInputDevice.cpp \
               ../cpp/YVRUnityXR/YVRMeshingProvider.cpp \
                ../cpp/YVRModuleMgrs/MixedReality/ImageTracking/YVRImageTrackingMgr.cpp \
               ../cpp/YVRModuleMgrs/Inputs/YVRInputMgr.cpp \
               ../cpp/YVRModuleMgrs/Inputs/XRMotionData.cpp \
               ../cpp/YVRModuleMgrs/Inputs/Hands/YVRHandMgr.cpp \
               ../cpp/YVRModuleMgrs/Inputs/Hands/XRHandData.cpp \
               ../cpp/YVRModuleMgrs/Inputs/Hands/YVRHandCalibrationMgr.cpp \
               ../cpp/YVRModuleMgrs/Inputs/Controllers/YVRControllersMgr.cpp \
               ../cpp/YVRModuleMgrs/Inputs/HMD/YVRHMDMgr.cpp \
               ../cpp/YVRModuleMgrs/Inputs/EyeTracking/YVREyeTrackingMgr.cpp \
               ../cpp/YVRModuleMgrs/YVRSpatialAnchorMgr.cpp\
               ../cpp/YVRModuleMgrs/YVRPlaneDetectionMgr.cpp\
               ../cpp/YVRModuleMgrs/YVRSceneMgr.cpp\
               ../cpp/YVRModuleMgrs/YVRMeshMgr.cpp\
               ../cpp/YVRModuleMgrs/YVRPassthroughProviderMgr.cpp \
               ../cpp/YVRModuleMgrs/YVRPassthroughMeshMgr.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRRenderMgr.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRRenderLayer.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRExtRenderLayer.cpp \
               ../cpp/YVRModuleMgrs/Render/YVREquirectRenderLayer.cpp \
               ../cpp/YVRModuleMgrs/Render/YVREquirect2RenderLayer.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRProjectionRenderLayer.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRQuadRenderLayer.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRCylinderRenderLayer.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRPassthroughLayer.cpp \
                ../cpp/YVRModuleMgrs/Render/YVRCubeRenderLayer.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRRenderLayersMgr.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRRenderConfigsMgr.cpp \
               ../cpp/YVRModuleMgrs/Render/YVRRenderSwapchainMgr.cpp \
               ../cpp/YVRModuleMgrs/YVRLifecycleMgr.cpp \
               ../cpp/YVRModuleMgrs/Events/YVREventsMgr.cpp \
               ../cpp/YVRModuleMgrs/YVRUnityXRProviderMgr.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/GfxTasksMgr.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/SetFoveationDataEventTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/SwapLayerBufferTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/TaskBase.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/AddActiveLayerTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/RemoveActiveLayerTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/CreateRenderLayerTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/DestroyRenderLayerTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/PrepareHandleLayerTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/TriggerLayerPreSubmitCallbackTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/SetLayerDepthTask.cpp \
               ../cpp/YVRModuleMgrs/Render/RenderTasks/SetEyeBufferSettingsTask.cpp \
               ../cpp/YVRModuleMgrs/YVROpenXRMgr.cpp \
                ../cpp/YVRModuleMgrs/YVRLargeSpatialMgr.cpp \
               ../cpp/OpenXR/Common.cpp \
               ../cpp/OpenXR/OpenXRProgram.cpp \
               ../cpp/OpenXR/OpenXRExtMgr.cpp \
               ../cpp/OpenXR/OpenXRAndroidPlatformPlugin.cpp \
               ../cpp/OpenXR/GraphicsPlugin/GraphicsPluginBase.cpp\
               ../cpp/OpenXR/GraphicsPlugin/GraphicsPluginOpenGLES.cpp\
               ../cpp/OpenXR/GraphicsPlugin/GraphicsPluginVulkan.cpp\


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../OpenXRLoader\openxr_loader\Include\
                    $(LOCAL_PATH)/../../../OpenXRLoader\openxr_loader\Include\openxr\
                    $(LOCAL_PATH)/../../../OpenXRLoader\openxr_loader\Include\pfdm_openxr_preview\



LOCAL_SHARED_LIBRARIES := libopenxr_loader
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv3
LOCAL_CFLAGS := -DXR_USE_TIMESPEC -DXR_USE_PLATFORM_ANDROID -DXR_USE_GRAPHICS_API_OPENGL_ES -DXR_USE_GRAPHICS_API_VULKAN
LOCAL_CPPFLAGS :=
LOCAL_LDFLAGS := -Wl,-z,max-page-size=16384
COMMON_SRC_FILES := $(LOCAL_SRC_FILES)
include $(BUILD_SHARED_LIBRARY)

