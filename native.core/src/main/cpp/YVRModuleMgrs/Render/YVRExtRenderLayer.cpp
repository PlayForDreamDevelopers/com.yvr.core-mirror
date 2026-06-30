#include "YVRExtRenderLayer.h"

YVRExtRenderLayer::YVRExtRenderLayer(int id, LayerCreateInfo createInfo) 
	: YVRRenderLayer(id, createInfo)
{ 
	eyeVisibility = createInfo.renderLayerEyeMask == xrEyeSide::kEyeMaskBoth   ? XrEyeVisibility::XR_EYE_VISIBILITY_BOTH
                    : createInfo.renderLayerEyeMask == xrEyeSide::kEyeMaskLeft ? XrEyeVisibility::XR_EYE_VISIBILITY_LEFT
                                                                                : XrEyeVisibility::XR_EYE_VISIBILITY_RIGHT;
}
