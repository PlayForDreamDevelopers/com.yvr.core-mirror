#pragma once

#include "../../UnityHeader/IUnityGraphics.h"
#include "../pch.h"
#include <string>

class GraphicsPluginBase
{
public:
    virtual ~GraphicsPluginBase() = default;

    // OpenXR extensions required by this graphics API.
    static std::vector<std::string> GetInstanceExtensions();

    // Create an instance of this graphics api for the provided instance and systemId.
    virtual void InitializeDevice(XrInstance instance, XrSystemId systemId) = 0;

    // Get the graphics binding header for session creation.
    virtual const XrBaseInStructure* GetGraphicsBinding() const = 0;

    virtual int64_t getTextureFormat(bool isSRGB) =0;
    virtual int64_t getDepthFormat() =0;
    virtual int64_t getMotionVectorTextureFormat() = 0;
    virtual int64_t getMotionVectorDepthFormat() = 0;
    virtual XrSwapchainImageBaseHeader* allocateSwapchainImages(uint32_t capacity) =0;
    virtual void deallocateSwapchainImages(XrSwapchainImageBaseHeader* images) =0;
    virtual uintptr_t getImageHandle(XrSwapchainImageBaseHeader* swapChain, int index) =0;

    static void setUnityInterface(IUnityInterfaces* unityInterfaces);

protected:
    static IUnityInterfaces* unityInterfaces;
};
