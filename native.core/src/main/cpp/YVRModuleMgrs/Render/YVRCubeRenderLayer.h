#pragma once

#include "./YVRRenderLayer.h"

class YVRCubeRenderLayer : public YVRRenderLayer
{
public:
    YVRCubeRenderLayer(int id, LayerCreateInfo createInfo);
    void setPose(XrPosef pose) override;
    void allocSwapChain() override;
    std::uintptr_t getColorHandle(xrEyeSide eyeSide, int bufferIndex) override;
};
