#pragma once

#include <array>
#include <pfdm_openxr_preview/openxr_yvr_private.h>

enum ControllerSide
{
    LEFT = 0,
    RIGHT = 1,
    COUNT = 2
};

struct ControllerXRActionContext
{
    XrActionSet actionSet{};
    std::array<XrPath, COUNT> controllerSubActionPath;
    std::array<XrBool32, COUNT> handActive;
    std::array<XrControllerChargeStatusYVR, COUNT> ctrlChargeStatus;

    XrAction poseAction{XR_NULL_HANDLE};
    XrAction vibrateAction{XR_NULL_HANDLE};

    XrAction primaryButtonAction{XR_NULL_HANDLE};
    XrAction primaryTouchAction{XR_NULL_HANDLE};
    XrAction secondaryButtonAction{XR_NULL_HANDLE};
    XrAction secondaryTouchAction{XR_NULL_HANDLE};

    XrAction thumbstickButtonAction{XR_NULL_HANDLE};
    XrAction thumbstickTouchAction{XR_NULL_HANDLE};
    XrAction thumbstickXValue{XR_NULL_HANDLE};
    XrAction thumbstickYValue{XR_NULL_HANDLE};

    XrAction gripButtonAction{XR_NULL_HANDLE};
    XrAction menuButtonAction{XR_NULL_HANDLE};

    XrAction triggerValue{XR_NULL_HANDLE};
    XrAction triggerTouchAction{XR_NULL_HANDLE};
};
