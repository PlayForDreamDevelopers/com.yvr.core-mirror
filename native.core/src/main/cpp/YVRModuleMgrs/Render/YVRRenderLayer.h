#pragma once

#include "..\..\XRData.h"
#include "../../OpenXR/OpenXRSwapchain.h"
#include "openxr/openxr.h"

typedef enum yvrRenderLayerType_
{
    Projection = 0,
    Quad,
    Cylinder,
    Equirect,
    CubeProjection
} yvrRenderLayerType;

struct LayerCreateInfo
{
    int depth;
    int width;
    int height;
    int swapChainBufferCount;
    int textureType;
    int layerFlags;
    int batchedLayerId;
    bool separateSwapChain;
    bool autoShowAfterCreated;
    bool isSurfaceSwapChain;
    yvrRenderLayerType renderLayerType;
    xrEyeSide renderLayerEyeMask;
    int protectedContent;
};

typedef struct yvrRect_
{
    float x;
    float y;
    float width;
    float height;
} yvrRect;

class YVRRenderLayer
{
public:
    int id;
    int batchedLayerId;//if not -1,this layer will use same swapchain with lefteye layer
    int depth;
    bool deleteLayerAfterReleaseSwapchain;

    YVRRenderLayer(int id);
    YVRRenderLayer(int id, LayerCreateInfo createInfo);

    XrCompositionLayerBaseHeader* getXrLayerHeader() const { return xrLayer; }
    void setFlags(int flags);
    void unsetFlags(int flags);
    void enable();
    void visible();
    virtual void resume();
    virtual void pause();
    void invisible();
    void disable(bool immediately = false);
    void setDRMSwapChain();

    void lockLayerFlag(int flags);
    void unLockLayerFlag();
    bool getFlagLocked();
    jobject GetSurface();

    int getBufferIndex() const;

    virtual void swapChain();
    virtual std::uintptr_t getColorHandle(xrEyeSide eyeSide);
    virtual std::uintptr_t getColorHandle(xrEyeSide eyeSide, int bufferIndex);
    virtual void tick();
    virtual void setPose(XrPosef pose);
    virtual void setSize(XrExtent2Df size);
    virtual void setImageRect(yvrRect rect);
    virtual void setLayerSettings(bool enableSuperSample, bool expensiveSuperSample, bool enableSharpen, bool expensiveSharpen);
    virtual void setSurfaceSwapChainDimensions(int width, int height);
    virtual void setSupportDPComposition(bool isSupport);

    virtual ~YVRRenderLayer();

protected:
    bool separateSwapChain;
    bool isSurfaceSwapChain;
    int bufferCount;
    bool isSupportDPComposition = false;
    XrEyeVisibility eyeVisibility;

    XrCompositionLayerBaseHeader* xrLayer{};
    XrCompositionLayerSettingsFB settings{XR_TYPE_COMPOSITION_LAYER_SETTINGS_FB};
    XrCompositionLayerImageLayoutFB compositionLayerImageLayoutFB{XR_TYPE_COMPOSITION_LAYER_IMAGE_LAYOUT_FB};
    XrDPCompositionLayerYVR dpCompositionLayerYVR{XR_TYPE_DP_COMPOSITION_LAYER_YVR};
    XrSwapchainCreateInfo xrSwapchainCreateInfo{};

    OpenXRSwapchain primarySwapchain{};
    bool swapchainImageAcquired = false;
    XrSwapchainImageBaseHeader* primarySwapchainImages;
    uint32_t primarySwapchainIndex;

    virtual void allocSwapChain();
    virtual void destroySwapChain();
    virtual void writeLayerNext();

private:
    const static int waitingFrames2Release;

    bool IsFlagLocked;
    int remainingFrame2Release;
    bool isStatic;
    bool acquireOnce;
    jobject surface;
};
