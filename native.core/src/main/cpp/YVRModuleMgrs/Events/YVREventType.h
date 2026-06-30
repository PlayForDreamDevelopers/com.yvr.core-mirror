#pragma once

/// Event type define
enum YVREventType
{
    kEventNone = 0,
    kVisibilityGained = 2,
    kVisibilityLost = 3,
    kFocusGained = 4,
    kFocusLost = 5,
    kRecenter = 20,
    kPreSubmit = 100,
    kPostSubmit = 101,
    KInputDeviceChange = 200
};
