#pragma once

#include "YVRRenderLayer.h"
#include "YVRExtRenderLayer.h"

class YVRQuadRenderLayer : public YVRExtRenderLayer
{
public:
    YVRQuadRenderLayer(int id, LayerCreateInfo createInfo);
    void setPose(XrPosef pose) override;
    void setSize(XrExtent2Df size) override;
    void setImageRect(yvrRect rect) override;
    void allocSwapChain() override;
};
