/**
 * @file openxr_yvr_private.h
 * @author yvr
 * @brief
 * @version 0.1
 * @date 2023-11-20
 *
 * @copyright Copyright (c) 2023 yvr
 *
 * 所有私有扩展如果不是扩展公开扩展，都使用同一个扩展号523
 * struct type: 1000522000 + 当前最大值
 */
#ifndef OPENXR_YVR_PRIVATE_H_
#define OPENXR_YVR_PRIVATE_H_ 1

#include "openxr_yvr.h"

// clang-format off
#if defined(__cplusplus)
extern "C" {
#endif

// Flag bits for XrSpaceLocationFlags extension
static const XrSpaceLocationFlags XR_SPACE_LOCATION_POSITION_TRACKED_ALIGNED_BIT = 0x00000010;
static const XrSpaceLocationFlags XR_SPACE_LOCATION_POSITION_TRACKED_INFERRING_BIT = 0x00000020;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_RECENTER_OCCURED_YVR, 1000060001);
typedef struct XrEventDataRecenterOccuredYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
    XrTime                      changeTime;
    XrPosef                     recenterPose;
} XrEventDataRecenterOccuredYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_GROUND_DISTANCE_YVR, 1000060003);
typedef struct XrEventDataGroundDistanceYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
    float                       distance;
} XrEventDataGroundDistanceYVR;

// 扩展 XR_YVR_controller_tracking_functions(501)
#ifndef XR_YVR_controller_tracking_functions
#error "Please define XR_YVR_controller_tracking_functions first"
#endif

// Flag bits for XrInputDeviceFlagsYVR
typedef XrFlags64 XrInputDeviceFlagsYVR;
static const XrInputDeviceFlagsYVR XR_INPUT_DEVICES_YVR_CONTROLLER_BIT_YVR = 0x00000001;
static const XrInputDeviceFlagsYVR XR_INPUT_DEVICES_YVR_HAND_BIT_YVR = 0x00000002;

static const XrHandTrackingAimFlagsFB XR_HAND_TRACKING_AIM_FIST_GESTURE_BIT_FB = 0x80000000;
static const XrHandTrackingAimFlagsFB XR_HAND_TRACKING_AIM_FIVE_GESTURE_BIT_FB = 0x40000000;
static const XrHandTrackingAimFlagsFB XR_HAND_TRACKING_AIM_PALM_IN_FRONT_BIT_FB = 0x20000000;

typedef enum XrControllerPairingStateYVR {
    XR_PAIRING_SUCCESSFULLY_YVR = 100,
    XR_PAIRING_ONGOING_YVR = 200,
    XR_PAIRING_STATE_UNKOWN_YVR = 201,
    XR_PAIRING_ERROR_UNKWON_YVR = 202,
    XR_PAIRING_INVALID_MASK_YVR = 203
} XrControllerPairingStateYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_INPUT_DEVICE_CHANGED_YVR, 1000060004);
typedef struct XrEventDataInputDeviceChangedYVR {
    XrStructureType             type;
    const void *XR_MAY_ALIAS    next;
    XrInputDeviceFlagsYVR       oldDevice;
    XrInputDeviceFlagsYVR       newDevice;
} XrEventDataInputDeviceChangedYVR;

typedef enum XrControllerTypeYVR {
    XR_CONTROLLER_TYPE_RING_YVR = 0,
    XR_CONTROLLER_TYPE_NONE_RING_YVR = 1,
} XrControllerTypeYVR;

typedef XrResult(XRAPI_PTR *PFN_xrStartControllerPairingYVR)(XrInstance instance, XrControllerYVR device);
typedef XrResult(XRAPI_PTR *PFN_xrStopControllerPairingYVR)(XrInstance instance, XrControllerYVR device);
typedef XrResult(XRAPI_PTR *PFN_xrRemoveControllerPairingYVR)(XrInstance instance, XrControllerYVR device);
typedef XrResult(XRAPI_PTR *PFN_xrQueryControllerPairingStateYVR)(XrInstance instance, XrControllerYVR device, XrControllerPairingStateYVR *state);
typedef XrResult(XRAPI_PTR *PFN_xrGetControllerSerialNumberYVR)(XrInstance instance, XrControllerYVR device, char *serialNumber);
typedef XrResult(XRAPI_PTR *PFN_xrGetCurrentInputDeviceYVR)(XrInstance instance, XrInputDeviceFlagsYVR *inputDevice);
typedef XrResult(XRAPI_PTR *PFN_xrSetCurrentInputDeviceYVR)(XrInstance instance, XrInputDeviceFlagsYVR inputDevice);
typedef XrResult(XRAPI_PTR *PFN_xrQueryCurrentConnectedControllerTypeYVR)(XrInstance instance, XrControllerYVR device, XrControllerTypeYVR *type);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrStartControllerPairingYVR(XrInstance instance, XrControllerYVR device);
XRAPI_ATTR XrResult XRAPI_CALL xrStopControllerPairingYVR(XrInstance instance, XrControllerYVR device);
XRAPI_ATTR XrResult XRAPI_CALL xrRemoveControllerPairingYVR(XrInstance instance, XrControllerYVR device);
XRAPI_ATTR XrResult XRAPI_CALL xrQueryControllerPairingStateYVR(XrInstance instance, XrControllerYVR device, XrControllerPairingStateYVR *state);
XRAPI_ATTR XrResult XRAPI_CALL xrGetControllerSerialNumberYVR(XrInstance instance, XrControllerYVR device, char *serialNumber);
XRAPI_ATTR XrResult XRAPI_CALL xrGetCurrentInputDeviceYVR(XrInstance instance, XrInputDeviceFlagsYVR *inputDevice);
XRAPI_ATTR XrResult XRAPI_CALL xrSetCurrentInputDeviceYVR(XrInstance instance, XrInputDeviceFlagsYVR inputDevice);
XRAPI_ATTR XrResult XRAPI_CALL xrQueryCurrentConnectedControllerTypeYVR(XrInstance instance, XrControllerYVR device, XrControllerTypeYVR *type);
#endif
#endif


// 扩展 XR_YVR_boundary_functions(502)
#ifndef XR_YVR_boundary_functions
#error "Please define XR_YVR_boundary_functions first"
#endif

typedef enum XrBoundaryShowStyleYVR {
    BOUNDARY_SHOW_STYLE_STANDARD_GRID_YVR = 0,
    BOUNDARY_SHOW_STYLE_CUSTOM_RIPPLE_YVR,
} XrBoundaryShowStyleYVR;

typedef enum XrBoundaryTypeYVR {
    BOUNDARY_TYPE_LOCAL_YVR = 0,
    BOUNDARY_TYPE_CUSTOM_YVR
} XrBoundaryTypeYVR;

typedef enum XrLocalBoundaryRadiusTypeYVR {
    LOCAL_BOUNDARY_RADIUS_SMALL_YVR = 0,
    LOCAL_BOUNDARY_RADIUS_MEDIUM_YVR,
    LOCAL_BOUNDARY_RADIUS_LARGE_YVR,
} XrLocalBoundaryRadiusTypeYVR;

typedef enum XrBoundaryVisibleControlYVR {
    BOUNDARY_NON_VISIBLE_YVR = 0,         // boundary non visible, but also depends on boundary collision detect
    BOUNDARY_FORCE_VISIBLE_YVR,           // boundary force visible
    BOUNDARY_FORCE_NON_VISIBLE_YVR,       // boundary force non visible
    BOUNDARY_NON_VISIBLE_WHEN_INSIDE_YVR, // boundary non visible when inside the boundary, visible when outside
    BOUNDARY_ENABLE_DETECTION,            // boundary enable detection and visible
    BOUNDARY_DISABLE_DETECTION            // boundary disable detection and non visible
} XrBoundaryVisibleControlYVR;

typedef enum XrBoundaryOutEffectTypeYVR{
    BOUNDARY_OUT_EFFECT_NONE,
    BOUNDARY_OUT_EFFECT_GRID,
    BOUNDARY_OUT_EFFECT_VST,
    BOUNDARY_OUT_EFFECT_GRID_VST
}XrBoundaryOutEffectTypeYVR;

typedef enum XrBoundaryControlActionYVR {
    ACTION_SET_BOUNDARY_SHOW_STYLE = 0,  // see XrBoundaryShowStyleYVR    1 * sizeof(int32_t)
    ACTION_SET_BOUNDARY_PREVIEW,         // max 256 floats
    ACTION_SET_BOUNDARY_POINTS,          // max 256 floats
    ACTION_SET_BOUNDARY_CONFIRM,         // 2 byte
    ACTION_SET_GROUND_DISTANCE,          // 2 * sizeof(float)
    ACTION_SET_CLEAR_BOUNDARY_DATA,      // 0 byte
    ACTION_SET_BOUNDARY_TYPE,            // see XrBoundaryTypeYVR     1 * sizeof(int32_t)
    ACTION_SET_GUARDIAN_APP_ENTER,       // 0 byte
    ACTION_SET_GUARDIAN_APP_LEAVE,       // 0 byte
    ACTION_SET_BOUNDARY_VISIBLE,         // see XrBoundaryVisibleControlYVR   1 * sizeof(int32_t)
    ACTION_SET_LOCAL_BOUNDARY_RADIUS,    // 1 * sizeof(float)
    ACTION_SET_BOUNDARY_DETECTION_ENABLE,// 1 * sizeof(int32_t)
    ACTION_SET_ORIGINAL_BOUNDARY_RADIUS, // 1 * sizeof(float)
    ACTION_SET_AUTO_GROUND_DISTANCE_ENABLE, // 1 * sizeof(int32_t)
    ACTION_SET_EFFECT_DISTANCE,             // 1 * sizeof(float)
    ACTION_SET_OUT_BOUNDARY_EFFECT,
    ACTION_GET_GROUND_DISTANCE = 100,  // 1 * sizeof(float)
    ACTION_GET_LOCAL_BOUNDARY_RADIUS,  // see XrLocalBoundaryRadiusTypeYVR 1 * sizeof(int32_t)
    ACTION_GET_BOUNDARY_TYPE,           // see XrBoundaryTypeYVR     1 * sizeof(int32_t)
    ACTION_GET_ORIGINAL_BOUNDARY_RADIUS,// 1 * sizeof(float)
    ACTION_GET_AUTO_GROUND_DISTANCE_ENABLE,// 1 * sizeof(int32_t)
    ACTION_GET_EFFECT_DISTANCE,             // 1 * sizeof(float)
    ACTION_GET_OUT_BOUNDARY_EFFECT
} XrBoundaryControlActionYVR;

typedef struct XrBoundaryControlData {
    XrBoundaryControlActionYVR action;

    int dataLengthBytes;
    uint8_t *controlData;
} XrBoundaryControlData;

typedef struct XrBoundaryStatus {
    XrBoundaryControlActionYVR type;

    int *dataLengthBytes;
    uint8_t *statusData;
} XrBoundaryStatus;

typedef XrResult(XRAPI_PTR *PFN_xrSetBoundaryControlYVR)(XrSession session, XrBoundaryControlData *data);
typedef XrResult(XRAPI_PTR *PFN_xrGetBoundaryStatusYVR)(XrSession session, XrBoundaryStatus *status);
typedef XrResult(XRAPI_PTR *PFN_xrGetBoundaryDataYVR)(XrSession session, int* outBid, int* outLength,float* outPoints);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrSetBoundaryControlYVR(XrSession session, XrBoundaryControlData *data);
XRAPI_ATTR XrResult XRAPI_CALL xrGetBoundaryStatusYVR(XrSession session, XrBoundaryStatus *status);
XRAPI_ATTR XrResult XRAPI_CALL xrGetBoundaryDataYVR(XrSession session, int* outBid, int* outLength,float* outPoints);
#endif
#endif

#define XR_YVR_camera_functions 1
#define XR_YVR_camera_functions_SPEC_VERSION 1
#define XR_YVR_CAMERA_FUNCTIONS_EXTENSION_NAME "XR_YVR_camera_functions"
typedef enum CameraTypeYVR_ {
    YVR_CAMERA_TRACKING_LOWER_PAIR = 0,
    YVR_CAMERA_TRACKING_UPPER_PAIR = 1,
    YVR_CAMERA_VST_PAIR = 2,
    YVR_CAMERA_TYPE_MAX
} CameraTypeYVR;

typedef struct CameraFrameYVR_ {
    uint32_t fn;
    uint64_t exposure_ts;
    uint32_t shutter_speed_ns;
    volatile uint8_t* buffer;
    uint32_t len;
    uint32_t width;
    uint32_t height;
    uint32_t gain;
    uint32_t stride;
    uint32_t format;
} CameraFrameYVR;

typedef struct CameraFrameInfoYVR_ {
    CameraTypeYVR type;
    CameraFrameYVR frame;
} CameraFrameInfoYVR;

typedef void (*CameraFrameCallbackYVRFn)(CameraFrameInfoYVR* frame);

typedef XrResult(XRAPI_PTR *PFN_xrInitCameraYVR)(XrSession session);
typedef XrResult(XRAPI_PTR *PFN_xrReleaseCameraYVR)(XrSession session);
typedef XrResult(XRAPI_PTR *PFN_xrStartCameraYVR)(XrSession session);
typedef XrResult(XRAPI_PTR *PFN_xrStopCameraYVR)(XrSession session);
typedef XrResult(XRAPI_PTR *PFN_xrSetCameraFrameCallbackYVR)(XrSession session, CameraFrameCallbackYVRFn cb);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrInitCameraYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrReleaseCameraYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrStartCameraYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrStopCameraYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrSetCameraFrameCallbackYVR(XrSession session, CameraFrameCallbackYVRFn cb);
#endif
#endif


// 扩展 XR_YVR_config_functions(503)
#ifndef XR_YVR_config_functions
#error "Please define XR_YVR_config_functions first"
#endif

XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_HAND_ENABLED, 16);
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_HAND_AUTO_ACTIVATE_TIME, 17);
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_HAND_AIM_SENSITIVITY, 0xE8); // int, 手势射线灵敏度
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_TRAVEL_MODE_STATE, 0xE9); // int, 旅行模式状态，见XrSlamTravelModeStateYVR
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_TRAVEL_MODE, 0xEA); // int, 旅行模式类型，见XrSlamTravelModeYVR
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_COLOR_CHROMA_STATE, 0xEB); // int, 色散校正开关状态
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_IPD_CONFIGURATION, 0xEC); // float, meter
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_HEVFILTER_VALUE, 0xED); // float Array
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_WEARING_DETECTION_SETTING, 0xEE);  // int
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_EYE_TRACKING_SETTING, 0xEF);   //int
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_EYE_TRACKING_ENABLED, 0xEF);//int Enable:1 Disable:0
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_MOTOR_POSITION_LEFT, 0xF0);    // float, millimetre
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_MOTOR_POSITION_RIGHT, 0xF1);   // float, millimetre
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_ROM_VERSION, 0xF2); //int
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_PASSTHROUGH_STATUS, 0xF3); //int
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_ANTI_GLARE_LEVEL, 0xF4); // int
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_CONTROLLER_MODEL_TYPE, 0xF5); // int
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_HAND_MODEL_TYPE, 0xF6); // int
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_SPACE_DETECT_RANGE, 0xF7); // space detect range, float
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_SPACE_DETECT_ENABLE_STATE, 0xF8); // Refer to xrSpaceDetectType
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_CAM_TRACKING_FREQ, 0xF9); // Refer to xrFreqType
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_HEVFILTER_ENABLE, 0xFA); //
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_LEFT_CONTROLLER_MCU_VERSION, 0xFB); // char Array
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_RIGHT_CONTROLLER_MCU_VERSION, 0xFC); // char Array
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_HMD_MCU_VERSION, 0xFD); // char Array
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_RECENTER_POSE, 0xFE); // float Array
XR_ENUM(xrGetSystemPropertyYVR, YVR_SYS_PROP_GET_MAX_NUMBERS, 0xFF); // float Array

XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_HAND_AIM_SENSITIVITY, 0xE6); // int, 手势射线灵敏度
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_TRAVEL_MODE, 0xE7); // int, 旅行模式类型，见XrSlamTravelModeYVR
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_VIRTUAL_SCENE, 0xE8); // float, 场景虚拟度
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_COLOR_CHROMA_STATE, 0xE9); // int, 色散校正开关
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_IPD_CONFIGURATION, 0xEA); // float, meter
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_HEVFILTER_VALUE, 0xEB); // float Array
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_WEARING_DETECTION_SETTING, 0xEC);  // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_EYE_TRACKING_SETTING, 0xED);   //int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_EYE_TRACKING_ENABLED, 0xED);//int Enable:1 Disable:0
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_MOTOR_POSITION_LEFT, 0xEE);    // float, millimetre
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_MOTOR_POSITION_RIGHT, 0xEF);   // float, millimetre
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_ANTI_GLARE_LEVEL, 0xF0); // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_CONTROLLER_MODEL_TYPE, 0xF1); // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_HAND_MODEL_TYPE, 0xF2); // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_SPACE_DETECT_RANGE, 0xF3); // space detect range, float
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_SPACE_DETECT_ENABLED, 0xF4); // int Enable:1 Disable:0
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_HAND_ENABLED, 0xF5); // int Enable:1 Disable:0
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_HAND_AUTO_ACTIVATE_TIME, 0xF6); // int ms
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_ZORDER, 0xF7); // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_AUTO_GROUND_DISTANCE, 0xF8); // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_HEVFILTER_ENABLE, 0xF9); // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_CAM_FREQ, 0xFA); //Refer to xrFreqType
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_IPD, 0xFB); // float, meter
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_LOG_LEVEL_MASK, 0xFC); // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_DO_RECENTER, 0xFD); // int
XR_ENUM(xrSetSystemPropertyYVR, YVR_SYS_PROP_SET_LAYER_VISIBILITY, 0xFE); // int

typedef enum xrStateRecenterPositionYVR {
    XR_STATE_RECENTER_WITHOUT_POSITION = 0,
    XR_STATE_RECENTER_WITH_POSITION = 1
} xrStateRecenterPositionYVR;

typedef enum xrAutoGroundDistance_ {
    YVR_AUTO_GROUND_DISTANCE_DISABLE = 0,
    YVR_AUTO_GROUND_DISTANCE_ENABLE = 1
} xrAutoGroundDistance;

typedef enum xrFreqType_ {
    YVR_FREQ_60HZ = 0,
    YVR_FREQ_50HZ = 1
} xrFreqType;

typedef enum xrSpaceDetectType_ {
    YVR_SPACE_DETECT_DISABLE = 0,
    YVR_SPACE_DETECT_ENABLE = 1
} xrSpaceDetectType;

typedef XrResult(XRAPI_PTR *PFN_xrGetSystemPropertyCharArrayYVR)(XrSession session, const xrGetSystemPropertyYVR propType, char *propData,
                                                                 int *outPropDataLen);
typedef XrResult(XRAPI_PTR *PFN_xrSetForceHeadModeYVR)(XrSession session, bool enable);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrGetSystemPropertyCharArrayYVR(XrSession session, const xrGetSystemPropertyYVR propType, char *propData,
                                                                int *outPropDataLen);
XRAPI_ATTR XrResult XRAPI_CALL xrSetForceHeadModeYVR(XrSession session, bool enable);
#endif
#endif

// 扩展 XR_YVR_frame_end_info_ext(505)
#ifndef XR_YVR_frame_end_info_ext
#error "Please define XR_YVR_frame_end_info_ext first"
#endif

typedef enum xrLayerTypeYVR {
    YVR_LAYER_TYPE_START = 0,
    YVR_LAYER_TYPE_PASSTHROUGH = 0,
    YVR_LAYER_TYPE_APP = 1,
    YVR_LAYER_TYPE_BOUNDARY = 2,
    YVR_LAYER_TYPE_SYSTEM_WIDGET = 3,
    YVR_LAYER_TYPE_SYSTEM_UI = 4,
    YVR_LAYER_TYPE_GLOBAL_SYSTEM_MENU = 5,
    YVR_LAYER_TYPE_VRWIDGET = 6,
    YVR_LAYER_TYPE_LOADING = 7,
    YVR_LAYER_TYPE_VST = 8,
    YVR_LAYER_TYPE_MAX = 9,
} xrLayerTypeYVR;

XR_STRUCT_ENUM(XR_TYPE_FRAME_END_INFO_LAYERTYPE_EXT_YVR, 1000060002);
// XrFrameEndInfoLayerTypeExtYVR extends XrFrameEndInfoExtYVR
typedef struct XrFrameEndInfoLayerTypeExtYVR {
    XrStructureType             type;
    const void *XR_MAY_ALIAS    next;
    xrLayerTypeYVR              layerType;
    float                       blendRate;
} XrFrameEndInfoLayerTypeExtYVR;


#define XR_YVR_composition_layer_alpha_blend 1
#define XR_YVR_composition_layer_alpha_blend_SPEC_VERSION 2
#define XR_YVR_COMPOSITION_LAYER_ALPHA_BLEND_EXTENSION_NAME "XR_YVR_composition_layer_alpha_blend"

typedef enum XrBlendFactorYVR {
    XR_BLEND_FACTOR_ZERO_YVR = 0,
    XR_BLEND_FACTOR_ONE_YVR = 1,
    XR_BLEND_FACTOR_SRC_ALPHA_YVR = 2,
    XR_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA_YVR = 3,
    XR_BLEND_FACTOR_DST_ALPHA_YVR = 4,
    XR_BLEND_FACTOR_ONE_MINUS_DST_ALPHA_YVR = 5,
    XR_BLEND_FACTOR_MAX_ENUM_YVR = 0x7FFFFFFF
} XrBlendFactorYVR;

XR_STRUCT_ENUM(XR_TYPE_COMPOSITION_LAYER_ALPHA_BLEND_YVR, 1000050007);
// XrCompositionLayerAlphaBlendYVR extends XrCompositionLayerBaseHeader
typedef struct XrCompositionLayerAlphaBlendYVR {
    XrStructureType         type;
    void* XR_MAY_ALIAS      next;
    XrBlendFactorYVR        srcFactorColor;
    XrBlendFactorYVR        dstFactorColor;
    XrBlendFactorYVR        srcFactorAlpha;
    XrBlendFactorYVR        dstFactorAlpha;
} XrCompositionLayerAlphaBlendYVR;

// 扩展 XR_YVR_hand_tracking_aim(513)
#ifndef XR_YVR_hand_tracking_aim
#error "Please define XR_YVR_hand_tracking_aim first"
#endif

/**
 * @brief 通知事件结构体头部标识
 *
 */
typedef struct EventHeaderYVR_ {
    int32_t type;        //!< 事件类型，手势图像为105
    uint64_t timestamp;  //!< 事件发生时间
} EventHeaderYVR;

typedef struct HandImageDataYVR_ {
    int32_t num;
    int32_t width;
    int32_t height;
    int32_t channel;
    uint8_t* data;
} HandImageDataYVR;

typedef struct HandImageEventYVR_ {
    EventHeaderYVR header;
    HandImageDataYVR image;
} HandImageEventYVR;

typedef enum HandDataTypeYVR_ {
    XR_HAND_DATA_TYPE_NORMAL_YVR = 0,       // 包含预测的正常手势数据
    XR_HAND_DATA_TYPE_RAW_DATA_YVR = 1,     // 输出算法的IMU系下的手势数据,无预测
    XR_HAND_DATA_TYPE_NO_PREDICT_YVR = 2    // 世界系下无预测的手势数据
} HandDataTypeYVR;

typedef enum HandEventTypeYVR_ {
    XR_HAND_EVENT_TYPE_ALG_IMAGE = 105,                //!< 手势算法调试图像
    XR_HAND_EVENT_TYPE_CALIBRATION_SUCCESS = 200,      //!< 手势校准成功
} HandEventTypeYVR;

/**
 * @brief 事件通知函数指针定义
 *
 */
typedef void (*EventsNotifyFn)(EventHeaderYVR* evt);

/**
 * @brief 注册手势图像回调，需要打开扩展XR_EXT_HAND_TRACKING_EXTENSION_NAME
 *
 */
typedef XrResult(XRAPI_PTR *PFN_xrRegisterHandImageCallbackYVR)(XrSession session, EventsNotifyFn cb);
typedef XrResult(XRAPI_PTR *PFN_xrUnregisterHandImageCallbackYVR)(XrSession session);

/**
 * @brief 开始手势校准，校准成功后会收到事件XR_HAND_EVENT_TYPE_CALIBRATION_SUCCESS
 * 
 */
typedef XrResult(XRAPI_PTR *PFN_xrStartHandCalibrationYVR)(XrSession session);

/**
 * @brief 停止手势校准
 * 
 */
typedef XrResult(XRAPI_PTR *PFN_xrStopHandCalibrationYVR)(XrSession session);

/**
 * @brief 注册手势事件相关回调
 * 
 */
typedef XrResult(XRAPI_PTR *PFN_xrRegisterHandEventCallbackYVR)(XrSession session, EventsNotifyFn cb);
typedef XrResult(XRAPI_PTR *PFN_xrUnregisterHandEventCallbackYVR)(XrSession session);

/**
 * @brief 手势算法调试内部接口
 * @param [in] handDataType : 手势输出数据类型，详见HandDataTypeYVR
 * 
 */
typedef XrResult (XRAPI_PTR *PFN_xrLocateHandJointsYVR)(const int handDataType, XrHandTrackerEXT handTracker, const XrHandJointsLocateInfoEXT* locateInfo, XrHandJointLocationsEXT* locations);
#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrRegisterHandImageCallbackYVR(XrSession session, EventsNotifyFn cb);
XRAPI_ATTR XrResult XRAPI_CALL xrUnregisterHandImageCallbackYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrStartHandCalibrationYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrStopHandCalibrationYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrRegisterHandEventCallbackYVR(XrSession session, EventsNotifyFn cb);
XRAPI_ATTR XrResult XRAPI_CALL xrUnregisterHandEventCallbackYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrLocateHandJointsYVR(
    XrHandTrackerEXT                            handTracker,
    const XrHandJointsLocateInfoEXT*            locateInfo,
    XrHandJointLocationsEXT*                    locations,
    const int                                   handDataType);
#endif
#endif

#define XR_YVR_spatial_entity 1
#define XR_YVR_spatial_entity_SPEC_VERSION 1
#define XR_YVR_SPATIAL_ENTITY_EXTENSION_NAME "XR_YVR_spatial_entity"

XR_STRUCT_ENUM(XR_TYPE_SPATIAL_ANCHOR_CREATE_INFO_YVR, 1000522003);
// XrSpatialAnchorCreateInfoYVR extends XrSpatialAnchorCreateInfoFB
typedef struct XrSpatialAnchorCreateInfoYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
    XrBool32                    global;
} XrSpatialAnchorCreateInfoYVR;


#define XR_YVR_scene 1
#define XR_YVR_scene_SPEC_VERSION 2
#define XR_YVR_SCENE_EXTENSION_NAME "XR_YVR_scene"

typedef XrResult(XRAPI_PTR *PFN_xrSetSpaceBoundingBox2DYVR)(XrSession session, XrSpace space, XrRect2Df* boundingBox2D);
typedef XrResult(XRAPI_PTR *PFN_xrSetSpaceBoundingBox3DYVR)(XrSession session, XrSpace space, XrRect3DfFB* boundingBox3D);
typedef XrResult(XRAPI_PTR *PFN_xrSetSpaceSemanticLabelsYVR)(XrSession session, XrSpace space, XrSemanticLabelsFB* semanticLabels);
typedef XrResult(XRAPI_PTR *PFN_xrSetSpaceBoundary2DYVR)(XrSession session, XrSpace space, XrBoundary2DFB* boundary2D);
typedef XrResult(XRAPI_PTR *PFN_xrSetSpaceRoomLayoutYVR)(XrSession session, XrSpace space, XrRoomLayoutFB* roomLayout);
typedef XrResult(XRAPI_PTR *PFN_xrSetSpaceTriangleMeshYVR)(XrSession session, XrSpace space, XrSpaceTriangleMeshMETA* triangleMesh);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrSetSpaceBoundingBox2DYVR(
    XrSession                                   session,
    XrSpace                                     space,
    XrRect2Df*                                  boundingBox2D);

XRAPI_ATTR XrResult XRAPI_CALL xrSetSpaceBoundingBox3DYVR(
    XrSession                                   session,
    XrSpace                                     space,
    XrRect3DfFB*                                boundingBox3D);

XRAPI_ATTR XrResult XRAPI_CALL xrSetSpaceSemanticLabelsYVR(
    XrSession                                   session,
    XrSpace                                     space,
    XrSemanticLabelsFB*                         semanticLabels);

XRAPI_ATTR XrResult XRAPI_CALL xrSetSpaceBoundary2DYVR(
    XrSession                                   session,
    XrSpace                                     space,
    XrBoundary2DFB*                             boundary2D);

XRAPI_ATTR XrResult XRAPI_CALL xrSetSpaceRoomLayoutYVR(
    XrSession                                   session,
    XrSpace                                     space,
    XrRoomLayoutFB*                             roomLayout);

XRAPI_ATTR XrResult XRAPI_CALL xrSetSpaceTriangleMeshYVR(
    XrSession                                   session,
    XrSpace                                     space,
    XrSpaceTriangleMeshMETA*                    triangleMesh);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */

#define XR_YVR_spatial_entity_container 1
#define XR_YVR_spatial_entity_container_SPEC_VERSION 1
#define XR_YVR_SPATIAL_ENTITY_CONTAINER_EXTENSION_NAME "XR_YVR_spatial_entity_container"

typedef XrResult(XRAPI_PTR *PFN_xrSetSpaceContainerYVR)(XrSession session, XrSpace space, XrSpaceContainerFB* spaceContainer);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrSetSpaceContainerYVR(
    XrSession                                   session,
    XrSpace                                     space,
    XrSpaceContainerFB*                         spaceContainer);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */


#define XR_YVR_scene_capture 1
#define XR_YVR_scene_capture_SPEC_VERSION  1
#define XR_YVR_SCENE_CAPTURE_EXTENSION_NAME "XR_YVR_scene_capture"

XR_STRUCT_ENUM(XR_TYPE_RESET_SCENE_INFO_YVR, 1000522004);
typedef struct XrResetSceneInfoYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
} XrResetSceneInfoYVR;

XR_STRUCT_ENUM(XR_TYPE_SCENE_STATUS_YVR, 1000522005);
typedef struct XrSceneStatusYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
    uint32_t                    sceneCount;
} XrSceneStatusYVR;

typedef XrResult (XRAPI_PTR *PFN_xrResetSceneYVR)(XrSession session, const XrResetSceneInfoYVR* info);
typedef XrResult (XRAPI_PTR *PFN_xrGetSceneStatusYVR)(XrSession session, XrSceneStatusYVR *status);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrResetSceneYVR(
    XrSession                                   session,
    const XrResetSceneInfoYVR*                  info);
XRAPI_ATTR XrResult XRAPI_CALL xrGetSceneStatusYVR(
    XrSession                                   session,
    XrSceneStatusYVR*                           status);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */


// 扩展 XR_YVR_spatial_entity_mesh(515)
#ifndef XR_YVR_spatial_entity_mesh
#error "Please define XR_YVR_spatial_entity_mesh first"
#endif

typedef enum XrRoomSetupStateYVR {
    XR_ROOM_SETUP_STATE_RUNNING_YVR = 0,            // 检测中
    XR_ROOM_SETUP_STATE_READY_YVR = 1,              // 检测完成
    XR_ROOM_SETUP_STATE_OVERFLOW_YVR = 2,           // 检测超过最大范围
    XR_ROOM_SETUP_STATE_SCANWALL_YVR = 3,           // 提示用户扫描一面墙
    XR_ROOM_SETUP_STATE_FAILURE_YVR = 4,            // 检测失败
    XR_ROOM_SETUP_STATE_MAX_ENUM_YVR = 0x7FFFFFFF
} XrRoomSetupStateYVR;

XR_STRUCT_ENUM(XR_TYPE_ROOM_SETUP_BEGIN_INFO_YVR, 1000514002);
typedef struct XrRoomSetupBeginInfoYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
    float                       minRoomArea;
    float                       maxRoomArea;
} XrRoomSetupBeginInfoYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_BOUNDARY_UPDATE_YVR, 1000514003);
typedef struct XrEventDataBoundaryUpdateYVR {
    XrStructureType             type;
    void* XR_MAY_ALIAS          next;
    XrSpace                     boundarySpace; // XR_SPACE_COMPONENT_TYPE_BOUNDED_2D_FB
} XrEventDataBoundaryUpdateYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_ROOM_SETUP_UPDATE_YVR, 1000514004);
typedef struct XrEventDataRoomSetupUpdateYVR {
    XrStructureType             type;
    void* XR_MAY_ALIAS          next;
    float                       progress; // [0, 1]
    XrRoomSetupStateYVR         roomSetupState;
} XrEventDataRoomSetupUpdateYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_ROOM_SETUP_COMPLETE_YVR, 1000514005);
typedef struct XrEventDataRoomSetupCompleteYVR {
    XrStructureType             type;
    void* XR_MAY_ALIAS          next;
    XrResult                    result;
    XrSpace                     roomLayoutSpace; // XR_SPACE_COMPONENT_TYPE_ROOM_LAYOUT_FB
} XrEventDataRoomSetupCompleteYVR;

XR_STRUCT_ENUM(XR_TYPE_BOUNDARY_DETECT_BEGIN_INFO_YVR, 1000514006);
typedef struct XrBoundaryDetectBeginInfoYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
    float                       radius;
} XrBoundaryDetectBeginInfoYVR;

typedef XrResult (XRAPI_PTR *PFN_xrBeginRoomSetupYVR)(XrMeshDetectorYVR meshDetector, const XrRoomSetupBeginInfoYVR* beginInfo);
typedef XrResult (XRAPI_PTR *PFN_xrEndRoomSetupYVR)(XrMeshDetectorYVR meshDetector);
typedef XrResult (XRAPI_PTR *PFN_xrBeginBoundaryDetectYVR)(XrMeshDetectorYVR meshDetector, const XrBoundaryDetectBeginInfoYVR* beginInfo);
typedef XrResult (XRAPI_PTR *PFN_xrEndBoundaryDetectYVR)(XrMeshDetectorYVR meshDetector);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrBeginRoomSetupYVR(
    XrMeshDetectorYVR meshDetector, 
    const XrRoomSetupBeginInfoYVR* beginInfo);
XRAPI_ATTR XrResult XRAPI_CALL xrEndRoomSetupYVR(
    XrMeshDetectorYVR meshDetector);
XRAPI_ATTR XrResult XRAPI_CALL xrBeginBoundaryDetectYVR(
    XrMeshDetectorYVR meshDetector,
    const XrBoundaryDetectBeginInfoYVR* beginInfo);
XRAPI_ATTR XrResult XRAPI_CALL xrEndBoundaryDetectYVR(
    XrMeshDetectorYVR meshDetector);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */


#define XR_YVR_loader_init_android 1
#define XR_YVR_loader_init_android_SPEC_VERSION  1
#define XR_YVR_LOADER_INIT_ANDROID_EXTENSION_NAME "XR_YVR_loader_init_android"

// 由于历史原因，XrLoaderInitInfoAndroidYVR 的 StructType 必须为 XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR
XR_STRUCT_ENUM(XR_TYPE_LOADER_INIT_INFO_ANDROID_YVR, XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR);
// XrLoaderInitInfoAndroidYVR extends XrLoaderInitInfoAndroidKHR
typedef struct XrLoaderInitInfoAndroidYVR {
    XrStructureType type;
    const void* XR_MAY_ALIAS next;
    uint64_t loaderVersion;
} XrLoaderInitInfoAndroidYVR;

#define XR_YVR_passthrough_content 1
#define XR_YVR_passthrough_content_SPEC_VERSION  1
#define XR_YVR_PASSTHROUGH_CONTENT_EXTENSION_NAME "XR_YVR_passthrough_content"
XR_DEFINE_HANDLE(XrPassthroughProviderYVR)
XR_DEFINE_HANDLE(XrPassthroughSwapchainYVR)

XR_STRUCT_ENUM(XR_TYPE_PASSTHROUGH_ACQUIRE_INFO_YVR, 1000522006);
typedef struct XrPassthroughAcquireInfoYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
    XrSpace                     space;
} XrPassthroughAcquireInfoYVR;

XR_STRUCT_ENUM(XR_TYPE_PASSTHROUGH_IMAGE_VIEW_YVR, 1000522007);
typedef struct XrPassthroughImageViewYVR {
    XrStructureType             type;
    const void* XR_MAY_ALIAS    next;
    XrPosef                     pose;
} XrPassthroughImageViewYVR;

XR_STRUCT_ENUM(XR_TYPE_PASSTHROUGH_IMAGE_YVR, 1000522008);
typedef struct XrPassthroughImageYVR {
    XrStructureType              type;
    const void* XR_MAY_ALIAS     next;
    uint32_t                     swapchainIndex;
    XrTime                       imageTime;
    XrPassthroughImageViewYVR    views[2];
} XrPassthroughImageYVR;

typedef XrResult (XRAPI_PTR *PFN_xrCreatePassthroughProviderYVR)(XrSession session, XrPassthroughProviderYVR* passthroughProvider);
typedef XrResult (XRAPI_PTR *PFN_xrDestroyPassthroughProviderYVR)(XrPassthroughProviderYVR passthroughProvider);
typedef XrResult (XRAPI_PTR *PFN_xrStartPassthroughProviderYVR)(XrPassthroughProviderYVR passthroughProvider);
typedef XrResult (XRAPI_PTR *PFN_xrStopPassthroughProviderYVR)(XrPassthroughProviderYVR passthroughProvider);
typedef XrResult (XRAPI_PTR *PFN_xrCreatePassthroughSwapchainYVR)(XrPassthroughProviderYVR passthroughProvider, XrPassthroughSwapchainYVR* swapchain);
typedef XrResult (XRAPI_PTR *PFN_xrDestroyPassthroughSwapchainYVR)(XrPassthroughSwapchainYVR swapchain);
typedef XrResult (XRAPI_PTR *PFN_xrEnumeratePassthroughSwapchainImagesYVR)(XrPassthroughSwapchainYVR swapchain, uint32_t imageCapacityInput, uint32_t* imageCountOutput, XrSwapchainImageBaseHeader* images);
typedef XrResult (XRAPI_PTR *PFN_xrAcquirePassthroughImageYVR)(XrPassthroughProviderYVR passthroughProvider, const XrPassthroughAcquireInfoYVR* acquireInfo, XrPassthroughImageYVR* passthroughImage);
typedef XrResult (XRAPI_PTR *PFN_xrCreatePassthroughSwapchainYVR2)(XrPassthroughProviderYVR passthroughProvider, XrPassthroughSwapchainYVR* swapchain);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrCreatePassthroughProviderYVR(
    XrSession                             session,
    XrPassthroughProviderYVR*             passthroughProvider);

XRAPI_ATTR XrResult XRAPI_CALL xrDestroyPassthroughProviderYVR(
    XrPassthroughProviderYVR              passthroughProvider);

XRAPI_ATTR XrResult XRAPI_CALL xrStartPassthroughProviderYVR(
    XrPassthroughProviderYVR              passthroughProvider);

XRAPI_ATTR XrResult XRAPI_CALL xrStopPassthroughProviderYVR(
    XrPassthroughProviderYVR              passthroughProvider);

XRAPI_ATTR XrResult XRAPI_CALL xrCreatePassthroughSwapchainYVR(
    XrPassthroughProviderYVR              passthroughProvider,
    XrPassthroughSwapchainYVR*            swapchain);

XRAPI_ATTR XrResult XRAPI_CALL xrDestroyPassthroughSwapchainYVR(
    XrPassthroughSwapchainYVR             swapchain);

XRAPI_ATTR XrResult XRAPI_CALL xrEnumeratePassthroughSwapchainImagesYVR(
    XrPassthroughSwapchainYVR             swapchain,
    uint32_t                              imageCapacityInput,
    uint32_t*                             imageCountOutput,
    XrSwapchainImageBaseHeader*           images);

XRAPI_ATTR XrResult XRAPI_CALL xrAcquirePassthroughImageYVR(
    XrPassthroughProviderYVR              passthroughProvider,
    const XrPassthroughAcquireInfoYVR*    acquireInfo,
    XrPassthroughImageYVR*                passthroughImage);

XRAPI_ATTR XrResult XRAPI_CALL xrCreatePassthroughSwapchainYVR2(
    XrPassthroughProviderYVR              passthroughProvider,
    XrPassthroughSwapchainYVR*            swapchain);

#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */

// 私有
XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_PASSTHROUGH_RESUMED_YVR, 1000522010);
typedef struct XrEventDataPassthroughResumedYVR {
    XrStructureType             type;
    void* XR_MAY_ALIAS          next;
} XrEventDataPassthroughResumedYVR;

// 扩展 XR_YVR_large_space(1000522000 + 011)
#define XR_YVR_large_space 1
#define XR_YVR_large_space_SPEC_VERSION  1
#define XR_YVR_LARGE_SPACE_EXTENSION_NAME "XR_YVR_large_space"

typedef enum XrMarkerTypeYVR {
    XR_MARKER_TYPE_ARUCO_YVR = 0,
    XR_MARKER_TYPE_APRIL_TAG_YVR = 1,
    XR_MARKER_TYPE_QR_YVR = 2,
    XR_MARKER_TYPE_EAN_13_YVR = 3,
    XR_MARKER_TYPE_UPC_A_YVR = 4,
    XR_MARKER_TYPE_CODE_128_YVR = 5,
    XR_MARKER_TYPE_MAX_ENUM_YVR = 0x7FFFFFFF
} XrMarkerTypeYVR;

typedef enum XrLargeSpaceResultCodeYVR{
    XR_LARGE_SPACE_RESULT_CODE_EXPORT_MAP_SUCCEED_YVR = 0,
    XR_LARGE_SPACE_RESULT_CODE_EXPORT_MAP_FAILURE_YVR = 1,
    XR_LARGE_SPACE_RESULT_CODE_ASSEMBLING_MAP_SUCCEED_YVR = 2,
    XR_LARGE_SPACE_RESULT_CODE_ASSEMBLING_MAP_FAILURE_YVR = 3,
    XR_LARGE_SPACE_RESULT_CODE_MAX_ENUM_YVR = 0x7FFFFFFF
}XrLargeSpaceResultCodeYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_MARKER_TRACKING_UPDATE_YVR, 1000522011);

typedef struct XrEventDataMarkerTrackingUpdateYVR{
    XrStructureType type;
    const void* XR_MAY_ALIAS next;

    XrSpace space;  // marker space ?
    uint64_t markerId;
    XrMarkerTypeYVR markerType;
    XrVector3f region[4];
    XrPosef markerPose;   // marker origin pose
    float confidence;
} XrEventDataMarkerTrackingUpdateYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_LARGE_SPACE_RESULT_YVR, 1000522012);

typedef struct XrEventDataLargeSpaceResultYVR{
    XrStructureType type;
    const void* XR_MAY_ALIAS next;
    XrLargeSpaceResultCodeYVR resultCode;
} XrEventDataLargeSpaceResultYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_LARGE_SPACE_MAP_A_UPDATE_YVR, 1000522013);

typedef struct XrEventDataLargeSpaceMapAUpdateYVR{
    XrStructureType type;
    const void* XR_MAY_ALIAS next;

    float estimated_time_consuming;
    float completed_percentage;
} XrEventDataLargeSpaceMapAUpdateYVR;

typedef XrResult (XRAPI_PTR *PFN_xrSetLargeSpaceMapRecordEnableYVR)(XrSession session, XrBool32 enable);
typedef XrResult (XRAPI_PTR *PFN_xrSetMarkerDetectionEnableYVR)(XrSession session, XrBool32 enable);
typedef XrResult (XRAPI_PTR *PFN_xrSetLargeSpaceMapAssemblingEnableYVR)(XrSession session, XrBool32 enable);
typedef XrResult (XRAPI_PTR *PFN_xrClearAllMarkersYVR)(XrSession session);
typedef XrResult (XRAPI_PTR *PFN_xrSetLargeSpaceOriginPoseYVR)(XrSession session, XrPosef inPose);
typedef XrResult (XRAPI_PTR *PFN_xrExportLargeSpaceMapYVR)(XrSession session);
typedef XrResult (XRAPI_PTR *PFN_xrSetRecenterEnableYVR)(XrSession session, XrBool32 enable);
typedef XrResult (XRAPI_PTR *PFN_xrGetRecenterEnableYVR)(XrSession session, XrBool32* outEnable);
typedef XrResult (XRAPI_PTR *PFN_xrGetLargeSpacePointsCalibParamsYVR)(XrSession session, XrPosef* outTransform, float* outGroundDistance);


#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrSetLargeSpaceMapRecordEnableYVR(XrSession session,XrBool32 enable);
XRAPI_ATTR XrResult XRAPI_CALL xrSetMarkerDetectionEnableYVR(XrSession session, XrBool32 enable);
XRAPI_ATTR XrResult XRAPI_CALL xrSetLargeSpaceMapAssemblingEnableYVR(XrSession session, XrBool32 enable);
XRAPI_ATTR XrResult XRAPI_CALL xrClearAllMarkersYVR(XrSession  session);
XRAPI_ATTR XrResult XRAPI_CALL xrSetLargeSpaceOriginPoseYVR(XrSession session, XrPosef inPose);
XRAPI_ATTR XrResult XRAPI_CALL xrExportLargeSpaceMapYVR(XrSession session);
XRAPI_ATTR XrResult XRAPI_CALL xrSetRecenterEnableYVR(XrSession session,XrBool32 enable);
XRAPI_ATTR XrResult XRAPI_CALL xrGetRecenterEnableYVR(XrSession  session, XrBool32* outEnable);
XRAPI_ATTR XrResult XRAPI_CALL xrGetLargeSpacePointsCalibParamsYVR(XrSession session, XrPosef* outTransform, float* outGroundDistance);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */

// clang-format on
typedef enum XrSlamCalibrationResultCodeYVR_ {
    XR_SLAM_CALIBRATION_FAILED_YVR = -1,
    XR_SLAM_CALIBRATION_SUCCEED_YVR = 0,
    XR_SLAM_CALIBRATION_RECORDING_DATA_COMPLETED_YVR = 1,  // 自标定采集数据完成
    XR_SLAM_CALIBRATION_RESULT_CODE_MAX_ENUM_YVR = 0x7FFFFFFF
} XrSlamCalibrationResultCodeYVR;

typedef struct XrEventDataSlamCalibrationResultYVR_ {
    XrStructureType type;
    const void* XR_MAY_ALIAS next;
    int resultCode;  // 见XrSlamCalibrationResultCodeYVR
} XrEventDataSlamCalibrationResultYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_SLAM_CALIBRATION_YVR, 1000522014);

typedef enum XrSlamTravelModeYVR_ {
    XR_SLAM_NORMAL_MODE_YVR = 0,   // slam正常模式
    XR_SLAM_TRAVEL_MODE1_YVR = 1,  // slam旅行模式1,单头显IMU旅行模式
    XR_SLAM_TRAVEL_MODE2_YVR = 2,  // slam旅行模式2,头显和手柄双IMU旅行模式
} XrSlamTravelModeYVR;

typedef enum XrSlamTravelModeStateYVR_ {
    XR_SLAM_STATE_NOT_IN_TRAVEL_MODE_YVR = 0,      // slam处于非旅行模式
    XR_SLAM_STATE_IN_TRAVEL_MODE1_YVR = 1,         // slam处于旅行模式1
    XR_SLAM_STATE_IN_TRAVEL_MODE2_YVR = 2,         // slam处于旅行模式2
    XR_SLAM_STATE_TRAVEL_MODE2_INIT_FAIL_YVR = 3,  // 旅行模式2初始化失败
    XR_SLAM_STATE_TRAVEL_MODE2_VIO_FAIL_YVR = 4,   // 旅行模式2 VIO失败
    XR_SLAM_TRAVEL_STATE_CODE_MAX_ENUM_YVR = 0x7FFFFFFF
} XrSlamTravelModeStateYVR;

typedef struct XrEventSlamTravelModeChangeYVR_ {
    XrStructureType type;
    const void* XR_MAY_ALIAS next;
    int mode;  // 见XrSlamTravelModeYVR
} XrEventSlamTravelModeChangeYVR;

typedef struct XrEventSlamTravelModeStateChangeYVR_ {
    XrStructureType type;
    const void* XR_MAY_ALIAS next;
    int state;  // 见XrSlamTravelModeStateYVR
} XrEventSlamTravelModeStateChangeYVR;

XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_SLAM_TRAVEL_MODE_STATE_CHANGE_YVR, 1000522015);
XR_STRUCT_ENUM(XR_TYPE_EVENT_DATA_SLAM_TRAVEL_MODE_CHANGE_YVR, 1000522016);

#define XR_YVR_slam_tracking_calibration 1
#define XR_YVR_slam_tracking_calibration_SPEC_VERSION 1
#define XR_YVR_SLAM_TRACKING_CALIBRATION_EXTENSION_NAME "XR_YVR_slam_tracking_calibration"

typedef XrResult(XRAPI_PTR* PFN_xrSetSlamCalibrationEnableYVR)(XrSession session, XrBool32 enable);
#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrSetSlamCalibrationEnableYVR(XrSession session, XrBool32 enable);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */

// 扩展 XR_YVR_render_scale(516)
#define XR_YVR_render_scale 1
#define XR_YVR_render_scale_SPEC_VERSION 1
#define XR_YVR_RENDER_SCALE_EXTENSION_NAME "XR_YVR_render_scale"

typedef struct XrRenderScaleNormalYVR {
    float normalScale;
} XrRenderScaleNormalYVR;

typedef struct XrRenderScaleQuadViewsYVR {
    float innerScale;
    float outerScale;
} XrRenderScaleQuadViewsYVR;

typedef struct XrRenderScaleYVR {
    bool overrideResolution;  // 开关
    bool useQuadViews;        // 注视点渲染启用状态
    union {
        XrRenderScaleNormalYVR normal;        // 普通缩放倍率
        XrRenderScaleQuadViewsYVR quadViews;  // 注视点缩放倍率，当quadviews开启生效
    } renderScale;
    float minRenderScale;  // 倍率最小值
    float maxRenderScale;  // 倍率最大值
} XrRenderScaleYVR;

typedef enum XrRenderSharpenTypeYVR_ {
    XR_RENDER_SHARPEN_INVALID_YVR = 0,       // 无效值
    XR_RENDER_SHARPEN_TYPE_FAST_YVR = 1,     // 锐化Normal模式
    XR_RENDER_SHARPEN_TYPE_QUALITY_YVR = 2,  // 锐化Quality模式
} XrRenderSharpenTypeYVR;

typedef struct XrSharpenInfoYVR_ {
    XrRenderSharpenTypeYVR type;
    uint8_t fast;     // 用于应用设置显示，对应noiseSuppress
    uint8_t quality;  // 用于应用设置显示，对应edgeSharpness
    bool enable;
} XrSharpenInfoYVR;

typedef struct XrFovYVR_ {
    float angle_left;
    float angle_right;
    float angle_up;
    float angle_down;
} XrFovYVR;

typedef struct XrRenderFovYVR_ {
    bool enable;
    XrFovYVR fov[2];  // 0:leftEye, 1:rightEye
} XrRenderFovYVR;

XR_STRUCT_ENUM(XR_TYPE_RENDER_SCALE_BUFFERS_YVR, 1000516001);
typedef struct XrRenderScaleBuffersYVR {
    XrStructureType type;
    void* XR_MAY_ALIAS next;
    uint32_t renderScaleCapacityInput;
    uint32_t renderScaleCountOutput;
    XrRenderScaleYVR* renderScales;
} XrRenderScaleBuffersYVR;

typedef enum XrFovResetTypeYVR_ {
    XR_FOV_RESET_HORIZON_YVR = 0,   // 重置fov水平方向为默认值
    XR_FOV_RESET_VERTICAL_YVR = 1,  // 重置fov垂直方向为默认值
} XrFovResetTypeYVR;

typedef XrResult(XRAPI_PTR* PFN_xrGetRenderScaleYVR)(XrSession session, const char* packageName, XrRenderScaleBuffersYVR* renderScaleBuffers);
typedef XrResult(XRAPI_PTR* PFN_xrSetRenderScaleYVR)(XrSession session, const char* packageName, XrRenderScaleYVR renderScale);
typedef XrResult(XRAPI_PTR* PFN_xrSetRenderSharpenYVR)(XrSession session, const char* packageName, XrSharpenInfoYVR* renderSharpenInfo);
typedef XrResult(XRAPI_PTR* PFN_xrGetRenderSharpenYVR)(XrSession session, const char* packageName, XrSharpenInfoYVR* outRenderSharpenInfo);
typedef XrResult(XRAPI_PTR* PFN_xrSetRenderFovYVR)(XrSession session, const char* packageName, XrRenderFovYVR* rendorFov);
typedef XrResult(XRAPI_PTR* PFN_xrGetRenderFovYVR)(XrSession session, const char* packageName, XrRenderFovYVR* outRendorFov);
typedef XrResult(XRAPI_PTR* PFN_xrResetRenderFovYVR)(XrSession session, const char* packageName, XrFovResetTypeYVR resetType);
#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrGetRenderScaleYVR(XrSession session, const char* packageName, XrRenderScaleBuffersYVR* renderScaleBuffers);
XRAPI_ATTR XrResult XRAPI_CALL xrSetRenderScaleYVR(XrSession session, const char* packageName, XrRenderScaleYVR renderScale);
XRAPI_ATTR XrResult XRAPI_CALL xrSetRenderSharpenYVR(XrSession session, const char* packageName, XrSharpenInfoYVR* renderSharpenInfo);
XRAPI_ATTR XrResult XRAPI_CALL xrGetRenderSharpenYVR(XrSession session, const char* packageName, XrSharpenInfoYVR* outRenderSharpenInfo);
XRAPI_ATTR XrResult XRAPI_CALL xrSetRenderFovYVR(XrSession session, const char* packageName, XrRenderFovYVR* rendorFov);
XRAPI_ATTR XrResult XRAPI_CALL xrGetRenderFovYVR(XrSession session, const char* packageName, XrRenderFovYVR* outRendorFov);
XRAPI_ATTR XrResult XRAPI_CALL xrResetRenderFovYVR(XrSession session, const char* packageName, XrFovResetTypeYVR resetType);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */

typedef struct XrColorSpaceConfigYVR_ {
    bool        enable;
    XrColorSpaceFB       colorSpace;
} XrColorSpaceConfigYVR;

typedef XrResult (XRAPI_PTR *PFN_xrSetColorSpaceConfigYVR)(XrSession session, const char* packageName, const XrColorSpaceConfigYVR* colorSpaceConfig);
typedef XrResult (XRAPI_PTR *PFN_xrGetColorSpaceConfigYVR)(XrSession session, const char* packageName, XrColorSpaceConfigYVR* outColorSpaceConfig);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrSetColorSpaceConfigYVR(XrSession session, const char* packageName, const XrColorSpaceConfigYVR* colorSpaceConfig);
XRAPI_ATTR XrResult XRAPI_CALL xrGetColorSpaceConfigYVR(XrSession session, const char* packageName, XrColorSpaceConfigYVR* outColorSpaceConfig);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */

#define XR_YVR_locate_room 1
#define XR_YVR_locate_room_SPEC_VERSION 1
#define XR_YVR_LOCATE_ROOM_EXTENSION_NAME "XR_YVR_locate_room"

XR_STRUCT_ENUM(XR_TYPE_LOCATE_ROOM_INFO_YVR, 1000516002);
typedef struct XrLocateRoomInfoYVR {
    XrStructureType             type;
    void* XR_MAY_ALIAS          next;
    XrUuidEXT                   uuid;
    bool                        isLocated;
    uint64_t                    sequence;
} XrLocateRoomInfoYVR;

typedef XrResult(XRAPI_PTR* PFN_xrSetLocateRoomYVR)(XrSession session, XrLocateRoomInfoYVR* roomInfo);
typedef XrResult(XRAPI_PTR* PFN_xrGetLocateRoomYVR)(XrSession session, XrLocateRoomInfoYVR* roomInfo);

#ifndef XR_NO_PROTOTYPES
#ifdef XR_EXTENSION_PROTOTYPES
XRAPI_ATTR XrResult XRAPI_CALL xrSetLocateRoomYVR(XrSession session, XrLocateRoomInfoYVR* roomInfo);
XRAPI_ATTR XrResult XRAPI_CALL xrGetLocateRoomYVR(XrSession session, XrLocateRoomInfoYVR* roomInfo);
#endif /* XR_EXTENSION_PROTOTYPES */
#endif /* !XR_NO_PROTOTYPES */

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // OPENXR_YVR_PRIVATE_H_
