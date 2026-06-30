#include "YVRPassthroughLayer.h"
#include "../../YVRUnityPlugin.h"


YVRPassthroughLayer::YVRPassthroughLayer(int id) : YVRRenderLayer(id)
{
    id = id;
}

void YVRPassthroughLayer::setPassthroughLayer(XrPassthroughLayerFB layer)
{
    passthroughLayer = layer;
    XrCompositionLayerPassthroughFB* passthrough_layer = new XrCompositionLayerPassthroughFB();
    passthrough_layer->type = XR_TYPE_COMPOSITION_LAYER_PASSTHROUGH_FB;
    passthrough_layer->flags = XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT;
    passthrough_layer->layerHandle = layer;
    passthrough_layer->space = XR_NULL_HANDLE;
    xrLayer = reinterpret_cast<XrCompositionLayerBaseHeader*>(passthrough_layer);
    depth = -100;
}

void YVRPassthroughLayer::resume()
{
    if (plugin.passThroughProviderMgr->passthroughLayer != XR_NULL_HANDLE)
    {
        OpenXRAPI(xrPassthroughLayerResumeFB(plugin.passThroughProviderMgr->passthroughLayer));
    }
}

void YVRPassthroughLayer::pause()
{
    if (plugin.passThroughProviderMgr->passthroughLayer != XR_NULL_HANDLE)
    {
        OpenXRAPI(xrPassthroughLayerPauseFB(plugin.passThroughProviderMgr->passthroughLayer));
    }
}