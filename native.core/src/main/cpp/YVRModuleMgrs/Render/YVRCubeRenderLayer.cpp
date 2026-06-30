#include "YVRCubeRenderLayer.h"
#include <openxr/openxr.h>
#include "../../Utilities/YVRMath.h"
#include "../../YVRUnityPlugin.h"

YVRCubeRenderLayer::YVRCubeRenderLayer(int id, LayerCreateInfo createInfo)
    : YVRRenderLayer(id, createInfo)
{
    eyeVisibility = createInfo.renderLayerEyeMask == xrEyeSide::kEyeMaskBoth   ? XrEyeVisibility::XR_EYE_VISIBILITY_BOTH
                    : createInfo.renderLayerEyeMask == xrEyeSide::kEyeMaskLeft ? XrEyeVisibility::XR_EYE_VISIBILITY_LEFT
                                                                               : XrEyeVisibility::XR_EYE_VISIBILITY_RIGHT;
    xrLayer = reinterpret_cast<XrCompositionLayerBaseHeader*>(new XrCompositionLayerCubeKHR{XR_TYPE_COMPOSITION_LAYER_CUBE_KHR});
    xrSwapchainCreateInfo.faceCount = 6;
}

void YVRCubeRenderLayer::setPose(XrPosef pose)
{
    auto cubeLayer = reinterpret_cast<XrCompositionLayerCubeKHR*>(xrLayer);
    cubeLayer->orientation = pose.orientation;
}

void YVRCubeRenderLayer::allocSwapChain()
{
    YVRRenderLayer::allocSwapChain();
    auto cubeLayer = reinterpret_cast<XrCompositionLayerCubeKHR*>(xrLayer);
    cubeLayer->space = plugin.inputMgr->GetAppSpace();
    cubeLayer->eyeVisibility = eyeVisibility;
    memset(&cubeLayer->swapchain, 0, sizeof(XrSwapchain));
    cubeLayer->swapchain = primarySwapchain.handle;
    cubeLayer->imageArrayIndex = 0;
}

std::uintptr_t YVRCubeRenderLayer::getColorHandle(xrEyeSide eyeSide, int bufferIndex)
{
    return plugin.openxrMgr->graphicsPlugin->getImageHandle(primarySwapchainImages, bufferIndex);
}