#include "YVRRenderLayersMgr.h"
#include "../../YVRUnityPlugin.h"

#include <algorithm>

int YVRRenderLayersMgr::createdLayerCount = 0;

YVRRenderLayersMgr::YVRRenderLayersMgr() : layersVecRequiredReSort(false), eyeBufferLayer(nullptr) {}

YVRRenderLayer* YVRRenderLayersMgr::getRenderLayer(int id)
{
    std::map<int, YVRRenderLayer*>::iterator iterator = id2RenderLayerMap.find(id);
    if (iterator == id2RenderLayerMap.end())
    {
        YError("ERROR: Ther is no renderlay with id %d .", id);
        return nullptr;
    }

    return iterator->second;
}

int YVRRenderLayersMgr::createLayer(LayerCreateInfo layerCreateInfo)
{
    AnnounceCalledFunc();
    YVRRenderLayer* layer = nullptr;
    switch (layerCreateInfo.renderLayerType)
    {
        case Projection:
            layer = static_cast<YVRRenderLayer*>(new YVRProjectionRenderLayer(createdLayerCount++, layerCreateInfo));
            break;
        case Cylinder:
            layer = static_cast<YVRRenderLayer*>(new YVRCylinderRenderLayer(createdLayerCount++, layerCreateInfo));
            break;
        case Equirect:
            layer = static_cast<YVRRenderLayer*>(new YVREquirect2RenderLayer(createdLayerCount++, layerCreateInfo));
            break;
        case CubeProjection:
            layer = static_cast<YVRRenderLayer*>(new YVRCubeRenderLayer(createdLayerCount++, layerCreateInfo));
            break;
        default:
            layer = static_cast<YVRRenderLayer*>(new YVRQuadRenderLayer(createdLayerCount++, layerCreateInfo));
            break;
    }

    /* YVRRenderLayer* layer = layerCreateInfo.depth == 0
                                ? static_cast<YVRRenderLayer*>(new YVRProjectionRenderLayer(createdLayerCount++, layerCreateInfo))
                                : static_cast<YVRRenderLayer*>(new YVRQuadRenderLayer(createdLayerCount++, layerCreateInfo));*/

    if (layerCreateInfo.depth == 0) eyeBufferLayer = static_cast<YVRProjectionRenderLayer*>(layer);
    if (layerCreateInfo.depth != 0 && layerCreateInfo.renderLayerType == Projection)
        environmentLayer = static_cast<YVRProjectionRenderLayer*>(layer);

    id2RenderLayerMap.insert({layer->id, layer});
    if (layerCreateInfo.autoShowAfterCreated) addActiveLayer(layer);

    return layer->id;
}

int YVRRenderLayersMgr::addPassthroughLayer(XrPassthroughLayerFB layerHandle)
{
    YVRPassthroughLayer* layer = new YVRPassthroughLayer(createdLayerCount++);
    layer->setPassthroughLayer(layerHandle);
    id2RenderLayerMap.insert({layer->id, layer});
    addActiveLayer(layer);
    return layer->id;
}

void YVRRenderLayersMgr::deletePassthroughLayer(int passthroughLayerId)
{
    AnnounceCallingFunc();
    YVRRenderLayer* passthroughLayer = getRenderLayer(passthroughLayerId);
    if (passthroughLayer == nullptr) return;

    removeActiveLayer(passthroughLayer);
    id2RenderLayerMap.erase(id2RenderLayerMap.find(passthroughLayerId));
}

void YVRRenderLayersMgr::deleteEyeBufferLayer(bool destroyImmediate)
{
    AnnounceCallingFunc();
    if (eyeBufferLayer == nullptr) return;

    removeActiveLayer(eyeBufferLayer);
    eyeBufferLayer->disable(destroyImmediate);
    if (destroyImmediate)
    {
        id2RenderLayerMap.erase(id2RenderLayerMap.find(eyeBufferLayer->id));
        delete (eyeBufferLayer);
        eyeBufferLayer = nullptr;
    }
    else
    {
        eyeBufferLayer->deleteLayerAfterReleaseSwapchain = true;
    }
}

void YVRRenderLayersMgr::deleteEnvironmentBufferLayer(bool destroyImmediate)
{
    AnnounceCallingFunc();
    if (environmentLayer == nullptr) return;

    removeActiveLayer(environmentLayer);
    environmentLayer->disable(destroyImmediate);
    if (destroyImmediate)
    {
        id2RenderLayerMap.erase(id2RenderLayerMap.find(environmentLayer->id));
        delete (environmentLayer);
        environmentLayer = nullptr;
    }
    else
    {
        environmentLayer->deleteLayerAfterReleaseSwapchain = true;
    }
}

void YVRRenderLayersMgr::deleteRenderLayer(int id, bool destroyImmediate)
{
    AnnounceCallingFunc();

    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer == nullptr) return;

    removeActiveLayer(renderLayer);
    renderLayer->disable(destroyImmediate);

    if (destroyImmediate)
    {
        id2RenderLayerMap.erase(id2RenderLayerMap.find(id));
        delete (renderLayer);
    }
    else
    {
        renderLayer->deleteLayerAfterReleaseSwapchain = true;
    }
}

void YVRRenderLayersMgr::addActiveLayer(int id)
{
    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer == nullptr) return;

    addActiveLayer(renderLayer);
}

void YVRRenderLayersMgr::removeActiveLayer(int id)
{
    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer == nullptr) return;

    removeActiveLayer(renderLayer);
}

void YVRRenderLayersMgr::addActiveLayer(YVRRenderLayer* layer)
{
    AnnounceCallingFunc();

    auto layerIter = std::find(activeLayersVec.cbegin(), activeLayersVec.cend(), layer);

    if (layerIter != activeLayersVec.cend())
    {
        YWarn("Layer %d already existed in active layer vectors ", layer->id);
        return;
    }

    layer->enable();
    activeLayersVec.push_back(layer);

    layersVecRequiredReSort = true;
}

void YVRRenderLayersMgr::removeActiveLayer(YVRRenderLayer* layer)
{
    AnnounceCallingFunc();

    auto layerIter = std::find(activeLayersVec.cbegin(), activeLayersVec.cend(), layer);
    if (layerIter == activeLayersVec.cend())
    {
        YWarn("There is no layer %d in active layer vectors", layer->id);
        return;
    }

    layer->disable();
    activeLayersVec.erase(layerIter);

    layersVecRequiredReSort = true;
}

void YVRRenderLayersMgr::setLayerVisible(int id, bool isVisible)
{
    AnnounceCallingFunc();
    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer == nullptr) return;

    if (isVisible)
    {
        auto layerIter = std::find(activeLayersVec.cbegin(), activeLayersVec.cend(), renderLayer);

        if (layerIter != activeLayersVec.cend())
        {
            YWarn("Layer %d already existed in active layer vectors ", renderLayer->id);
            return;
        }
        activeLayersVec.push_back(renderLayer);
        renderLayer->visible();
    }
    else
    {
        auto layerIter = std::find(activeLayersVec.cbegin(), activeLayersVec.cend(), renderLayer);
        if (layerIter == activeLayersVec.cend())
        {
            YWarn("There is no layer %d in active layer vectors", renderLayer->id);
            return;
        }
        activeLayersVec.erase(layerIter);
        renderLayer->invisible();
    }
    layersVecRequiredReSort = true;
}

bool YVRRenderLayersMgr::SetLayerDepth(int id, int depth)
{
    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer == nullptr) return false;
    return SetLayerDepth(renderLayer, depth);
}

bool YVRRenderLayersMgr::SetLayerDepth(YVRRenderLayer* layer, int depth)
{
    YInfo("Change layer %d depth to %d ", layer->id, depth);
    layer->depth = depth;
    layersVecRequiredReSort = true;
    return true;
}

int YVRRenderLayersMgr::getLayersCount()
{
    int result = activeLayersVec.size();
    return result;
}

void YVRRenderLayersMgr::lockLayerFlags(int id, int layerFlags)
{
    AnnounceCallingFunc();

    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer) renderLayer->lockLayerFlag(layerFlags);
}

void YVRRenderLayersMgr::unLockLayerFlags(int id)
{
    AnnounceCallingFunc();

    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer) renderLayer->unLockLayerFlag();
}

void YVRRenderLayersMgr::setLayerFlags(int id, int layerFlags)
{
    YInfo("setLayerFlags layerId(%d) layerFlags(%d)", id, layerFlags);
    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer) renderLayer->setFlags(layerFlags);
}

void YVRRenderLayersMgr::unsetLayerFlags(int id, int layerFlags)
{
    YInfo("unsetLayerFlags layerId(%d) layerFlags(%d)", id, layerFlags);
    YVRRenderLayer* renderLayer = getRenderLayer(id);
    if (renderLayer) renderLayer->unsetFlags(layerFlags);
}

int YVRRenderLayersMgr::getLayerColorHandle(int id, xrEyeSide eyeSide, int bufferIndex)
{
    AnnounceCallingFunc();

    YVRRenderLayer* renderLayer = getRenderLayer(id);
    return renderLayer ? renderLayer->getColorHandle(eyeSide, bufferIndex) : -1;
}

XrCompositionLayerBaseHeader** YVRRenderLayersMgr::GetSortedXRLayers(unsigned int& layerCount, bool transFirstLayer)
{
    if (layersVecRequiredReSort)
    {
        sortedXrLayerHeaders.clear();

        AnnounceCallingFunc();

        // Sort Render LayersVec with renderLayer's ID
        std::sort(activeLayersVec.begin(), activeLayersVec.end(),
                  [](const YVRRenderLayer* first, const YVRRenderLayer* sec) -> bool { return first->depth < sec->depth; });

        for (int i = 0; i != activeLayersVec.size(); ++i)
        {
            if (!activeLayersVec[i]->getFlagLocked())
            {
                if (i == 0 && !transFirstLayer)
                    activeLayersVec[i]->unsetFlags(XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT);
                else
                    activeLayersVec[i]->setFlags(XR_COMPOSITION_LAYER_BLEND_TEXTURE_SOURCE_ALPHA_BIT);
            }
            sortedXrLayerHeaders.push_back(activeLayersVec[i]->getXrLayerHeader());
        }

        layersVecRequiredReSort = false;
    }

    layerCount = sortedXrLayerHeaders.size();

    return sortedXrLayerHeaders.data();
}

void YVRRenderLayersMgr::addRenderLayerIdToBeDeletedLayerVec(int layerId) { toBeDeletedLayerVec.push_back(layerId); }

void YVRRenderLayersMgr::getAllLayersColorHandle(int* layersColorHandle)
{
    for (int i = 0; i != activeLayersVec.size(); ++i)
    {
        layersColorHandle[i] = activeLayersVec[i]->getColorHandle(kEyeMaskBoth);
    }
}

void YVRRenderLayersMgr::enableAllActiveLayers()
{
    for (int i = 0; i != activeLayersVec.size(); ++i)
    {
        activeLayersVec[i]->enable();
    }
}

void YVRRenderLayersMgr::pauseAllActiveLayers()
{
    for (int i = 0; i != activeLayersVec.size(); ++i)
    {
        activeLayersVec[i]->pause();
    }
}

void YVRRenderLayersMgr::resumeAllActiveLayers()
{
    for (int i = 0; i != activeLayersVec.size(); ++i)
    {
        activeLayersVec[i]->resume();
    }
}

void YVRRenderLayersMgr::disableAllLayers(bool immediately)
{
    for (auto it = id2RenderLayerMap.begin(); it != id2RenderLayerMap.end(); ++it) it->second->disable(immediately);
}

void YVRRenderLayersMgr::tickAllLayers()
{
    for (auto it = id2RenderLayerMap.begin(); it != id2RenderLayerMap.end(); ++it) it->second->tick();

    for (int i = 0; i != toBeDeletedLayerVec.size(); ++i)
    {
        id2RenderLayerMap.erase(id2RenderLayerMap.find(toBeDeletedLayerVec[i]));
    }
    toBeDeletedLayerVec.clear();
}

void YVRRenderLayersMgr::SetLayersVecRequiredReSort() { layersVecRequiredReSort = true; }

YVRRenderLayersMgr::~YVRRenderLayersMgr() = default;
