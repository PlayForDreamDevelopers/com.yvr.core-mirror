#pragma once

#include "YVRRenderLayer.h"
#include "YVRExtRenderLayer.h"

class YVRCylinderRenderLayer : public YVRExtRenderLayer
{
  public:
    YVRCylinderRenderLayer(int id, LayerCreateInfo createInfo);
    void setPose(XrPosef pose) override;
    void allocSwapChain() override;
    void setImageRect(yvrRect rect) override;
    void setCylinderParam(float radius, float centralAngle, float aspectRatio);
};
