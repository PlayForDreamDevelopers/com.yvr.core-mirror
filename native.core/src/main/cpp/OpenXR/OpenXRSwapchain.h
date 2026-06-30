#pragma once
#include "./pch.h"

struct OpenXRSwapchain
{
    XrSwapchain handle;
    int32_t width;
    int32_t height;
    bool swapchainAllocated;
};
