#pragma once

#include "../UnityHeader/XR/IUnityXRMeshing.h"
#include "YVRProviderBase.h"
#include "../YVRUnityPlugin.h"
#include <map>

typedef struct Mesh_
{
    XrVector3f position;
    XrQuaternionf rotation;
    XrVector3f scale;
    uint32_t vertexCount;
    XrVector3f* vertices;
    uint32_t indexCount;
    uint32_t* indices32;
    bool dirty;
} Mesh;

class YVRMeshingProvider : YVRProviderBase
{
  public:
    YVRMeshingProvider(YVRUnityXRProviderMgr& context, UnitySubsystemHandle handle);
    ~YVRMeshingProvider();

    UnitySubsystemErrorCode initialize() override;
    UnitySubsystemErrorCode start() override;
    void stop() override;
    void shutDown() override;
    void meshBlockUpdate(XrSpace meshBlockSpace, XrMeshBlockChangeStateYVR changeState);
    void clearMeshCache();

  private:
    void initializeUnityMeshLifecycle(UnityXRMeshProvider& unityGfxThreadProvider);
    UnitySubsystemErrorCode acquireMesh(const UnityXRMeshId* meshId, UnityXRMeshDataAllocator* allocator);
    UnitySubsystemErrorCode getMeshInfos(UnityXRMeshInfoAllocator* allocator);
    UnitySubsystemErrorCode getUpdatedTransforms(UnityXRMeshTransformList* list);
    UnitySubsystemErrorCode releaseMesh(const UnityXRMeshId* meshId, const UnityXRMeshDescriptor* mesh,
                                        void* pluginData);
    UnitySubsystemErrorCode setBoundingVolume(const UnityXRBoundingVolume* boundingVolume);
    UnitySubsystemErrorCode setMeshDensity(float density);
    bool getBlockTransform(uint64_t xrSpace, UnityXRVector3* position, UnityXRVector4* rotation);
    Mesh getTriangleMesh(XrSpace meshBlockSpace);
    void releaseMeshData(Mesh& mesh);
    std::map<uint64_t, Mesh> meshes;
};
