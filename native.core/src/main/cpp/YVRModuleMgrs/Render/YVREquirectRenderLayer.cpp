#include "YVREquirectRenderLayer.h"
#include "../../YVRUnityPlugin.h"
#include "../../Utilities/YVRCoordinateConverter.h"

YVREquirectRenderLayer::YVREquirectRenderLayer(int id, LayerCreateInfo createInfo)
    : YVRExtRenderLayer(id, createInfo)
{
    xrLayer = reinterpret_cast<XrCompositionLayerBaseHeader*>(new XrCompositionLayerEquirectKHR{XR_TYPE_COMPOSITION_LAYER_EQUIRECT_KHR});
}

void YVREquirectRenderLayer::allocSwapChain()
{
    YVRRenderLayer::allocSwapChain();

    const auto equirectLayer = reinterpret_cast<XrCompositionLayerEquirectKHR*>(xrLayer);
    equirectLayer->space = plugin.inputMgr->GetAppSpace();
    equirectLayer->eyeVisibility = eyeVisibility;
    memset(&equirectLayer->subImage, 0, sizeof(XrSwapchainSubImage));
    equirectLayer->subImage.swapchain = primarySwapchain.handle;
    equirectLayer->subImage.imageArrayIndex = 0;
    equirectLayer->pose = XrPosef{0, 0, 0, 1};
}

void YVREquirectRenderLayer::setImageRect(yvrRect rect)
{
    const auto equirectLayer = reinterpret_cast<XrCompositionLayerEquirectKHR*>(xrLayer);
    equirectLayer->subImage.imageRect.offset.x = (int)(primarySwapchain.width * rect.x);
    equirectLayer->subImage.imageRect.offset.y = (int)(primarySwapchain.width * rect.y);
    equirectLayer->subImage.imageRect.extent.width = (int)(primarySwapchain.width * rect.width);
    equirectLayer->subImage.imageRect.extent.height = (int)(primarySwapchain.height * rect.height);
}

void YVREquirectRenderLayer::setPose(XrPosef pose)
{
    const auto equirectLayer = reinterpret_cast<XrCompositionLayerEquirectKHR*>(xrLayer);
    equirectLayer->pose = ConvertPoseBetweenOpenXRandUnity(pose);
}

void YVREquirectRenderLayer::setEquirectRadius(float radius)
{
    const auto equirectLayer = reinterpret_cast<XrCompositionLayerEquirectKHR*>(xrLayer);
    equirectLayer->radius = radius;
}

void YVREquirectRenderLayer::setEquirectOffset(yvrRect offset)
{
    const auto equirectLayer = reinterpret_cast<XrCompositionLayerEquirectKHR*>(xrLayer);
    equirectLayer->scale = XrVector2f{offset.width, offset.height};
    equirectLayer->bias = XrVector2f{offset.x, offset.y};
}
