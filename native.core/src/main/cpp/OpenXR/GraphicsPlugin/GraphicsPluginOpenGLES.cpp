#include "GraphicsPluginOpenGLES.h"
#include "../OpenXRExtMgr.h"
#include "../Common.h"

GraphicsPluginOpenGLES::GraphicsPluginOpenGLES() = default;

GraphicsPluginOpenGLES::~GraphicsPluginOpenGLES() = default;

void GraphicsPluginOpenGLES::InitializeDevice(XrInstance instance, XrSystemId systemId)
{
    AnnounceCallingFunc();

    XrGraphicsRequirementsOpenGLESKHR graphicsRequirements{XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_ES_KHR};
    OpenXRAPI(xrGetOpenGLESGraphicsRequirementsKHR(instance, systemId, &graphicsRequirements));

    GLint major = 0;
    GLint minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    contextApiMajorVersion = major;

    const XrVersion desiredApiVersion = XR_MAKE_VERSION(major, minor, 0);
    YInfo("Designed OpenGLES version is %s", GetXrVersionString(desiredApiVersion).c_str());
    YInfo("Min required version %s", GetXrVersionString(graphicsRequirements.minApiVersionSupported).c_str());
    if (graphicsRequirements.minApiVersionSupported > desiredApiVersion)
    {
        YError("Runtime does not support desired Graphics API and/or version");
    }

    graphicsBinding.display = eglGetCurrentDisplay();
    graphicsBinding.config = (EGLConfig)0;
    graphicsBinding.context = eglGetCurrentContext();
    CheckEGLError(__FILE__,__LINE__);

    AnnounceCalledFunc();
}

const XrBaseInStructure* GraphicsPluginOpenGLES::GetGraphicsBinding() const { return reinterpret_cast<const XrBaseInStructure*>(&graphicsBinding); }

int64_t GraphicsPluginOpenGLES::getTextureFormat(bool isSRGB) { return isSRGB ? GL_SRGB8_ALPHA8 : GL_RGBA8; }
int64_t GraphicsPluginOpenGLES::getDepthFormat() { return GL_DEPTH24_STENCIL8; }

int64_t GraphicsPluginOpenGLES::getMotionVectorTextureFormat() { return GL_RGBA16F; }
int64_t GraphicsPluginOpenGLES::getMotionVectorDepthFormat() { return GL_DEPTH_COMPONENT24; }

XrSwapchainImageBaseHeader* GraphicsPluginOpenGLES::allocateSwapchainImages(uint32_t capacity)
{
    XrSwapchainImageOpenGLESKHR* swapchainImageBuffer = new XrSwapchainImageOpenGLESKHR[capacity];

    for (int i = 0; i != capacity; ++i)
    {
        swapchainImageBuffer[i].type = XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_ES_KHR;
        swapchainImageBuffer[i].next = NULL;
    }

    return reinterpret_cast<XrSwapchainImageBaseHeader*>(swapchainImageBuffer);
}

void GraphicsPluginOpenGLES::deallocateSwapchainImages(XrSwapchainImageBaseHeader* images) { delete[] images; }

uintptr_t GraphicsPluginOpenGLES::getImageHandle(XrSwapchainImageBaseHeader* swapChain, int index)
{
    return (std::uintptr_t)reinterpret_cast<const XrSwapchainImageOpenGLESKHR*>(swapChain)[index].image;
}
