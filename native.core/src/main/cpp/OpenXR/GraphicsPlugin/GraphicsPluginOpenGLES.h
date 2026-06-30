#pragma once

#include "./GraphicsPluginBase.h"

class GraphicsPluginOpenGLES : public GraphicsPluginBase
{
public:
    GraphicsPluginOpenGLES();
    ~GraphicsPluginOpenGLES() override;

    void InitializeDevice(XrInstance instance, XrSystemId systemId) override;

    const XrBaseInStructure* GetGraphicsBinding() const override;
    int64_t getTextureFormat(bool isSRGB) override;
    int64_t getDepthFormat() override;
    int64_t getMotionVectorTextureFormat() override;
    int64_t getMotionVectorDepthFormat() override;
    XrSwapchainImageBaseHeader* allocateSwapchainImages(uint32_t capacity) override;
    void deallocateSwapchainImages(XrSwapchainImageBaseHeader* images) override;
    uintptr_t getImageHandle(XrSwapchainImageBaseHeader* swapChain, int index) override;

private:
    XrGraphicsBindingOpenGLESAndroidKHR graphicsBinding{XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR};
    GLint contextApiMajorVersion{0};
};
