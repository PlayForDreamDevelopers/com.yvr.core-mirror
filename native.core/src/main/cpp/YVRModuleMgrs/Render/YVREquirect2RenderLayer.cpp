#include "YVREquirect2RenderLayer.h"
#include "../../YVRUnityPlugin.h"
#include "../../Utilities/YVRCoordinateConverter.h"

YVREquirect2RenderLayer::YVREquirect2RenderLayer(int id, LayerCreateInfo createInfo) : YVRExtRenderLayer(id, createInfo)
{
    xrLayer = reinterpret_cast<XrCompositionLayerBaseHeader*>(new XrCompositionLayerEquirect2KHR{XR_TYPE_COMPOSITION_LAYER_EQUIRECT2_KHR});
}

void YVREquirect2RenderLayer::allocSwapChain()
{
    YVRRenderLayer::allocSwapChain();
    const auto equirect2Layer = reinterpret_cast<XrCompositionLayerEquirect2KHR*>(xrLayer);
    equirect2Layer->space = plugin.inputMgr->GetAppSpace();
    equirect2Layer->eyeVisibility = eyeVisibility;
    memset(&equirect2Layer->subImage, 0, sizeof(XrSwapchainSubImage));
    equirect2Layer->subImage.swapchain = primarySwapchain.handle;
    equirect2Layer->subImage.imageArrayIndex = 0;
    equirect2Layer->pose = XrPosef{0, 0, 0, 1};
}

void YVREquirect2RenderLayer::setImageRect(yvrRect rect)
{
    const auto equirect2Layer = reinterpret_cast<XrCompositionLayerEquirect2KHR*>(xrLayer);
    equirect2Layer->subImage.imageRect.offset.x = (int)(primarySwapchain.width * rect.x);
    equirect2Layer->subImage.imageRect.offset.y = (int)(primarySwapchain.width * rect.y);
    equirect2Layer->subImage.imageRect.extent.width = (int)(primarySwapchain.width * rect.width);
    equirect2Layer->subImage.imageRect.extent.height = (int)(primarySwapchain.height * rect.height);
}

void YVREquirect2RenderLayer::setPose(const XrPosef pose)
{
    const auto equirect2Layer = reinterpret_cast<XrCompositionLayerEquirect2KHR*>(xrLayer);
    equirect2Layer->pose = ConvertPoseBetweenOpenXRandUnity(pose);
}

void YVREquirect2RenderLayer::setEquirect2Param(float radius, float centralHorizontalAngle, float upperVerticalAngle, float lowerVerticalAngle)
{
    const auto equirect2Layer = reinterpret_cast<XrCompositionLayerEquirect2KHR*>(xrLayer);
    equirect2Layer->radius = radius;
    equirect2Layer->centralHorizontalAngle = centralHorizontalAngle;
    equirect2Layer->upperVerticalAngle = upperVerticalAngle;
    equirect2Layer->lowerVerticalAngle = lowerVerticalAngle;
}