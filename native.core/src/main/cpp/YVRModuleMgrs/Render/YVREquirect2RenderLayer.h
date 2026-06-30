#pragma once

#include "YVRRenderLayer.h"
#include "YVRExtRenderLayer.h"

class YVREquirect2RenderLayer : public YVRExtRenderLayer
{
  public:
    YVREquirect2RenderLayer(int id, LayerCreateInfo createInfo);
    void setPose(XrPosef pose) override;
    void allocSwapChain() override;
    void setImageRect(yvrRect rect) override;
    void setEquirect2Param(float radius, float centralHorizontalAngle, float upperVerticalAngle, float lowerVerticalAngle);
};