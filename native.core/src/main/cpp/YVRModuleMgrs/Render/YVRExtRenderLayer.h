#pragma once
#include "YVRRenderLayer.h"

class YVRExtRenderLayer : public YVRRenderLayer
{
public:
    YVRExtRenderLayer(int id, LayerCreateInfo createInfo);
protected:
    XrEyeVisibility eyeVisibility;
};
