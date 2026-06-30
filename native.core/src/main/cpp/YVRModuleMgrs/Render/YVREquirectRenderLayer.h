#pragma once

#include "YVRRenderLayer.h"
#include "YVRExtRenderLayer.h"

class YVREquirectRenderLayer : public YVRExtRenderLayer
{
  public:
    YVREquirectRenderLayer(int id, LayerCreateInfo createInfo);
    void setPose(XrPosef pose) override;
    void allocSwapChain() override;
    void setImageRect(yvrRect rect) override;
    //void setEquirectParam(float radius);
    void setEquirectRadius(float radius);
    void setEquirectOffset(yvrRect offset);
};
