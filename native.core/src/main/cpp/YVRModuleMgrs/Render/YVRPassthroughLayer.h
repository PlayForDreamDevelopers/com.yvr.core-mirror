#pragma once

#include "./YVRRenderLayer.h"

class YVRPassthroughLayer : public YVRRenderLayer
{
public:
    YVRPassthroughLayer(int id);
    void setPassthroughLayer(XrPassthroughLayerFB layer);
    void resume() override;
    void pause() override;
    void swapChain() override {}
    std::uintptr_t getColorHandle(xrEyeSide eyeSide) override { return 0; }
    std::uintptr_t getColorHandle(xrEyeSide eyeSide, int bufferIndex) override { return 0; }
    void tick() override {}
    void setPose(XrPosef pose) override {}
    void setSize(XrExtent2Df size) override {}
    void setImageRect(yvrRect rect) override {}
    void setLayerSettings(bool enableSuperSample, bool expensiveSuperSample,
                          bool enableSharpen, bool expensiveSharpen) override {}
    void setSurfaceSwapChainDimensions(int width, int height) override {}
    void allocSwapChain() override {}
    void destroySwapChain() override {}
    void writeLayerNext() override {}

    XrPassthroughLayerFB passthroughLayer;

    ~YVRPassthroughLayer() = default;
};
