#include "YVRCylinderRenderLayer.h"
#include <cmath>
#include "../../YVRUnityPlugin.h"
#include "../../Utilities/YVRCoordinateConverter.h"
#include "../../YVRUnityXR/YVRDisplayProvider.h"

YVRCylinderRenderLayer::YVRCylinderRenderLayer(int id, LayerCreateInfo createInfo) : YVRExtRenderLayer(id, createInfo)
{
    xrLayer = reinterpret_cast<XrCompositionLayerBaseHeader*>(new XrCompositionLayerCylinderKHR{XR_TYPE_COMPOSITION_LAYER_CYLINDER_KHR});
}

void YVRCylinderRenderLayer::allocSwapChain()
{
    YVRRenderLayer::allocSwapChain();
    const auto cylinderLayer = reinterpret_cast<XrCompositionLayerCylinderKHR*>(xrLayer);
    cylinderLayer->space = plugin.inputMgr->GetAppSpace();
    cylinderLayer->eyeVisibility = eyeVisibility;
    memset(&cylinderLayer->subImage, 0, sizeof(XrSwapchainSubImage));
    cylinderLayer->subImage.swapchain = primarySwapchain.handle;
    cylinderLayer->subImage.imageArrayIndex = 0;
    cylinderLayer->pose = XrPosef{0, 0, 0, 1};
}

void YVRCylinderRenderLayer::setImageRect(yvrRect rect)
{
    const auto cylinderLayer = reinterpret_cast<XrCompositionLayerCylinderKHR*>(xrLayer);
    cylinderLayer->subImage.imageRect.offset.x = (int)(primarySwapchain.width * rect.x);
    cylinderLayer->subImage.imageRect.offset.y = (int)(primarySwapchain.width * rect.y);
    cylinderLayer->subImage.imageRect.extent.width = (int)(primarySwapchain.width * rect.width);
    cylinderLayer->subImage.imageRect.extent.height = (int)(primarySwapchain.height * rect.height);
}

void YVRCylinderRenderLayer::setPose(const XrPosef pose)
{
    const auto cylinderLayer = reinterpret_cast<XrCompositionLayerCylinderKHR*>(xrLayer);
    cylinderLayer->pose = ConvertPoseBetweenOpenXRandUnity(pose);
}

void YVRCylinderRenderLayer::setCylinderParam(float radius, float centralAngle, float aspectRatio)
{
    const auto cylinderLayer = reinterpret_cast<XrCompositionLayerCylinderKHR*>(xrLayer);
    cylinderLayer->radius = radius;
    cylinderLayer->centralAngle = centralAngle;
    cylinderLayer->aspectRatio = aspectRatio;
}