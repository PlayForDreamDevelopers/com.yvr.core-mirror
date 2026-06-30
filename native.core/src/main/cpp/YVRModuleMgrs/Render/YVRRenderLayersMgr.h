#pragma once

#include <map>
#include <vector>
#include "./YVRRenderLayer.h"
#include "./YVRQuadRenderLayer.h"
#include "./YVRCylinderRenderLayer.h"
#include "./YVREquirectRenderLayer.h"
#include "./YVREquirect2RenderLayer.h"
#include "./YVRProjectionRenderLayer.h"
#include "./YVRPassthroughLayer.h"
#include "./YVRCubeRenderLayer.h"
#include "openxr/openxr.h"

class YVRRenderLayer;

class YVRRenderLayersMgr
{
public:
    YVRRenderLayersMgr();
    ~YVRRenderLayersMgr();

    int getLayersCount();
    void getAllLayersColorHandle(int* layersColorHandle);

    int createLayer(LayerCreateInfo layerCreateInfo);

    int addPassthroughLayer(XrPassthroughLayerFB layerHandle);
    void deletePassthroughLayer(int passthroughLayerId);
    void deleteRenderLayer(int id, bool destroyImmediate);

    void addActiveLayer(int id);
    void addActiveLayer(YVRRenderLayer* layer);

    void removeActiveLayer(int id);
    void removeActiveLayer(YVRRenderLayer* layer);

    void setLayerVisible(int id, bool isVisible);

    bool SetLayerDepth(int id, int depth);
    bool SetLayerDepth(YVRRenderLayer* layer, int depth);

    void SetSupportDPComposition(int id, bool isSupport);

    YVRRenderLayer* getRenderLayer(int id);
    YVRProjectionRenderLayer* getEyeBufferLayer() { return eyeBufferLayer; }
    YVRProjectionRenderLayer* getEnvironmentLayer() { return environmentLayer; }
    void deleteEyeBufferLayer(bool destroyImmediate);
    void deleteEnvironmentBufferLayer(bool destroyImmediate);

    int getLayerColorHandle(int id, xrEyeSide eyeSide, int bufferIndex);

    void lockLayerFlags(int id, int layerFlags);
    void unLockLayerFlags(int id);
    void setLayerFlags(int id, int layerFlags);
    void unsetLayerFlags(int id, int layerFlags);

    void enableAllActiveLayers();
    void pauseAllActiveLayers();
    void resumeAllActiveLayers();
    void disableAllLayers(bool immediately);
    void tickAllLayers();
    void SetLayersVecRequiredReSort();
    void addRenderLayerIdToBeDeletedLayerVec(int layerId);

    XrCompositionLayerBaseHeader** GetSortedXRLayers(unsigned int& layerCount, bool transFirstLayer = false);

private:
    static int createdLayerCount;
    bool layersVecRequiredReSort;
    YVRProjectionRenderLayer* environmentLayer;
    YVRProjectionRenderLayer* eyeBufferLayer;

    std::vector<XrCompositionLayerBaseHeader*> sortedXrLayerHeaders{};
    std::vector<YVRRenderLayer*> activeLayersVec{};
    std::vector<int> toBeDeletedLayerVec;

    std::map<int, YVRRenderLayer*> id2RenderLayerMap{};
};
