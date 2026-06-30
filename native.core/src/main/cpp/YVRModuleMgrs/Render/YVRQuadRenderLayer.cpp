#include "YVRQuadRenderLayer.h"
#include <cmath>
#include "../../YVRUnityPlugin.h"
#include "../../Utilities/YVRCoordinateConverter.h"
#include "../../YVRUnityXR/YVRDisplayProvider.h"

YVRQuadRenderLayer::YVRQuadRenderLayer(int id, LayerCreateInfo createInfo) : YVRExtRenderLayer(id, createInfo)
{
    xrLayer = reinterpret_cast<XrCompositionLayerBaseHeader*>(new XrCompositionLayerQuad{XR_TYPE_COMPOSITION_LAYER_QUAD});
}

void YVRQuadRenderLayer::allocSwapChain()
{
    YVRRenderLayer::allocSwapChain();
    const auto quadLayer = reinterpret_cast<XrCompositionLayerQuad*>(xrLayer);
    quadLayer->space = plugin.inputMgr->GetAppSpace();
    quadLayer->eyeVisibility = eyeVisibility;
    memset(&quadLayer->subImage, 0, sizeof(XrSwapchainSubImage));
    quadLayer->subImage.swapchain = primarySwapchain.handle;
    quadLayer->subImage.imageArrayIndex = 0;
    quadLayer->pose = XrPosef{0, 0, 0, 1};
}

void YVRQuadRenderLayer::setPose(XrPosef pose)
{
    const auto quadLayer = reinterpret_cast<XrCompositionLayerQuad*>(xrLayer);
    quadLayer->pose = ConvertPoseBetweenOpenXRandUnity(pose);
}

void YVRQuadRenderLayer::setSize(XrExtent2Df size)
{
    const auto quadLayer = reinterpret_cast<XrCompositionLayerQuad*>(xrLayer);
    quadLayer->size = size;
}

void YVRQuadRenderLayer::setImageRect(yvrRect rect)
{
    const auto quadLayer = reinterpret_cast<XrCompositionLayerQuad*>(xrLayer);
    quadLayer->subImage.imageRect.offset.x = (int)(primarySwapchain.width * rect.x);
    quadLayer->subImage.imageRect.offset.y = (int)(primarySwapchain.height * rect.y);
    quadLayer->subImage.imageRect.extent.width = (int)(primarySwapchain.width * rect.width);
    quadLayer->subImage.imageRect.extent.height = (int)(primarySwapchain.height * rect.height);
}
