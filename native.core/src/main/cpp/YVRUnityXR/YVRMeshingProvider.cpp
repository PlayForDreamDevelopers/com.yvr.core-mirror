#include "YVRMeshingProvider.h"
#include "../Utilities/YVRCoordinateConverter.h"


YVRMeshingProvider::YVRMeshingProvider(YVRUnityXRProviderMgr& context, UnitySubsystemHandle handle)
    : YVRProviderBase(context, handle) {}

void YVRMeshingProvider::releaseMeshData(Mesh& mesh)
{
    delete[] mesh.vertices;
    delete[] mesh.indices32;
    mesh.vertices = nullptr;
    mesh.indices32 = nullptr;
    mesh.vertexCount = 0;
    mesh.indexCount = 0;
    mesh.dirty = false;
}

void YVRMeshingProvider::clearMeshCache()
{
    for (auto& entry : meshes)
    {
        releaseMeshData(entry.second);
    }

    meshes.clear();
}

UnitySubsystemErrorCode YVRMeshingProvider::initialize()
{
    AnnounceCallingFunc();
    UnityXRMeshProvider unityMeshProvider{};
    unityMeshProvider.userData = &m_Context;
    initializeUnityMeshLifecycle(unityMeshProvider);
    m_Context.meshInterface->RegisterMeshProvider(m_SystemHandle, &unityMeshProvider);
    return kUnitySubsystemErrorCodeSuccess;
}

void YVRMeshingProvider::initializeUnityMeshLifecycle(UnityXRMeshProvider& unityMeshProvider)
{
    AnnounceCallingFunc();
    unityMeshProvider.GetMeshInfos = [](UnitySubsystemHandle handle, void* userData, UnityXRMeshInfoAllocator* allocator) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        return yvrProviderContext.meshProvider->getMeshInfos(allocator);
    };

    unityMeshProvider.AcquireMesh = [](UnitySubsystemHandle handle, void* userData, const UnityXRMeshId* meshId,
                                       UnityXRMeshDataAllocator* allocator) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        return yvrProviderContext.meshProvider->acquireMesh(meshId, allocator);
    };
    unityMeshProvider.GetUpdatedTransforms = [](UnitySubsystemHandle handle, void* userData,
                                                UnityXRMeshTransformList* list) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        return yvrProviderContext.meshProvider->getUpdatedTransforms(list);
    };
    unityMeshProvider.ReleaseMesh = [](UnitySubsystemHandle handle, void* userData, const UnityXRMeshId* meshId, const UnityXRMeshDescriptor* mesh,
                                       void* pluginData) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        return yvrProviderContext.meshProvider->releaseMesh(meshId, mesh, pluginData);
    };
    unityMeshProvider.SetBoundingVolume = [](UnitySubsystemHandle handle, void* userData,
                                             const UnityXRBoundingVolume* boundingVolume) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        return yvrProviderContext.meshProvider->setBoundingVolume(boundingVolume);
    };
    unityMeshProvider.SetMeshDensity = [](UnitySubsystemHandle handle, void* userData, float density) -> UnitySubsystemErrorCode
    {
        const YVRUnityXRProviderMgr& yvrProviderContext = GetYVRProviderMgr(userData);
        return yvrProviderContext.meshProvider->setMeshDensity(density);
    };
}

UnitySubsystemErrorCode YVRMeshingProvider::acquireMesh(const UnityXRMeshId* meshId, UnityXRMeshDataAllocator* allocator)
{
    Mesh mesh = meshes[meshId->idPart[0]];
    auto* meshData =
        m_Context.meshInterface->MeshDataAllocator_AllocateMesh(allocator, mesh.vertexCount, mesh.indexCount, kUnityXRIndexFormat32Bit,
                                                                static_cast<UnityXRMeshVertexAttributeFlags>(0), kUnityXRMeshTopologyTriangles);
    memcpy(meshData->positions, mesh.vertices, mesh.vertexCount * sizeof(UnityXRVector3));
    meshData->tangents = nullptr;
    meshData->uvs = nullptr;
    meshData->colors = nullptr;
    meshData->transform.position = {mesh.position.x, mesh.position.y, mesh.position.z};
    meshData->transform.rotation = {mesh.rotation.x, mesh.rotation.y, mesh.rotation.z, mesh.rotation.w};
    meshData->transform.scale = {mesh.scale.x, mesh.scale.y, mesh.scale.z};
    memcpy(meshData->indices32, mesh.indices32, mesh.indexCount * sizeof(uint32_t));

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRMeshingProvider::getMeshInfos(UnityXRMeshInfoAllocator* allocator)
{
    UnityXRMeshInfo* meshInfos = m_Context.meshInterface->MeshInfoAllocator_Allocate(allocator, meshes.size());
    if (meshInfos == nullptr) return kUnitySubsystemErrorCodeFailure;
    int i = 0;
    for (auto p : meshes)
    {
        UnityXRMeshId id{};
        id.idPart[0] = p.first;
        meshInfos[i].meshId = id;
        meshInfos[i].updated = p.second.dirty;
        meshInfos[i].priorityHint = 0;
        i++;
        p.second.dirty = false;
    }

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRMeshingProvider::getUpdatedTransforms(UnityXRMeshTransformList* list)
{
    std::vector<UnityXRMeshTransform> transformList{};
    int i = 0;
    auto time = plugin.inputMgr->getTime(false);
    for (auto p : meshes)
    {
        UnityXRMeshTransform meshTransform{};
        UnityXRTrsTransform transform{};
        UnityXRMeshId id{};
        id.idPart[0] = p.first;
        if (getBlockTransform(p.first, &transform.position, &transform.rotation))
        {
            transform.scale = {1, 1, 1};
            meshTransform.transform = transform;
            meshTransform.meshId = id;
            meshTransform.timestamp = time;
            transformList.push_back(meshTransform);
        }

        i++;
    }

    const size_t validTransformCount = transformList.size();
    m_Context.meshInterface->MeshTransformList_Add(list, transformList.data(), validTransformCount);
    return kUnitySubsystemErrorCodeSuccess;
}

bool YVRMeshingProvider::getBlockTransform(uint64_t xrSpace, UnityXRVector3* position, UnityXRVector4* rotation)
{
    XrVector3f xrPosition;
    XrQuaternionf xrRotation;
    XrSpaceLocationFlags spaceLocationFlags;
    const bool result = plugin.spatialAnchorMgr->getSpatialAnchorPose(xrSpace, &xrPosition, &xrRotation, &spaceLocationFlags);
    if (result)
    {
        xrPosition = ConvertPoseBetweenOpenXRandUnity(xrPosition);
        xrRotation = ConvertPoseBetweenOpenXRandUnity(xrRotation);
        *position = {xrPosition.x, xrPosition.y, xrPosition.z};
        *rotation = {xrRotation.x, xrRotation.y, xrRotation.z, xrRotation.w};
    }

    return result;
}

UnitySubsystemErrorCode YVRMeshingProvider::releaseMesh(const UnityXRMeshId* meshId, const UnityXRMeshDescriptor* mesh, void* pluginData)
{
    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRMeshingProvider::setBoundingVolume(const UnityXRBoundingVolume* boundingVolume) { return kUnitySubsystemErrorCodeFailure; }

UnitySubsystemErrorCode YVRMeshingProvider::setMeshDensity(float density) { return kUnitySubsystemErrorCodeFailure; }

void YVRMeshingProvider::meshBlockUpdate(XrSpace meshBlockSpace, XrMeshBlockChangeStateYVR changeState)
{
    SpaceTriangleMesh spaceTriangleMesh{0, 0, nullptr, 0, 0, nullptr};
    uint64_t meshId = (uint64_t)meshBlockSpace;
    Mesh mesh;
    switch (changeState)
    {
        case XR_MESH_BLOCK_CHANGE_STATE_ADDED_YVR:
        {
            mesh = getTriangleMesh(meshBlockSpace);
            meshes[meshId] = mesh;
            break;
        }
        case XR_MESH_BLOCK_CHANGE_STATE_UPDATED_YVR:
        {
            mesh = meshes[meshId];
            releaseMeshData(mesh);
            mesh = getTriangleMesh(meshBlockSpace);
            meshes[meshId] = mesh;
            break;
        }
        case XR_MESH_BLOCK_CHANGE_STATE_REMOVED_YVR:
        {
            mesh = meshes[meshId];
            releaseMeshData(mesh);
            meshes.erase(meshId);
            break;
        }
        case XR_MESH_BLOCK_CHANGE_STATE_UNCHANGED_YVR:
            break;
        case XR_MESH_BLOCK_CHANGE_STATE_CLEAR_YVR:
            clearMeshCache();
            break;
        case XR_MESH_BLOCK_CHANGE_STATE_MAX_ENUM_YVR:
            break;
        default: ;
    }
}

Mesh YVRMeshingProvider::getTriangleMesh(XrSpace meshBlockSpace)
{
    Mesh mesh{};
    SpaceTriangleMesh spaceTriangleMesh{0, 0, nullptr, 0, 0, nullptr};
    std::vector<uint32_t> indices;
    std::vector<XrVector3f> vertices;
    XrResult result = plugin.sceneMgr->getSpaceTriangleMeshMETA(meshBlockSpace, &spaceTriangleMesh);
    if (result == XR_SUCCESS)
    {
        spaceTriangleMesh.indexCapacityInput = spaceTriangleMesh.indexCountOutput;
        spaceTriangleMesh.vertexCapacityInput = spaceTriangleMesh.vertexCountOutput;
        indices.resize(spaceTriangleMesh.indexCountOutput);
        vertices.resize(spaceTriangleMesh.vertexCountOutput);
        spaceTriangleMesh.indices = indices.data();
        spaceTriangleMesh.vertices = vertices.data();
    }

    result = plugin.sceneMgr->getSpaceTriangleMeshMETA(meshBlockSpace, &spaceTriangleMesh);
    if (result == XR_SUCCESS)
    {
        mesh.vertexCount = spaceTriangleMesh.vertexCountOutput;
        mesh.vertices = new XrVector3f[mesh.vertexCount];
        memcpy(mesh.vertices, spaceTriangleMesh.vertices, sizeof(XrVector3f) * mesh.vertexCount);
        mesh.indexCount = spaceTriangleMesh.indexCountOutput;
        mesh.indices32 = new uint32_t[mesh.indexCount];
        memcpy(mesh.indices32, spaceTriangleMesh.indices, sizeof(uint32_t) * mesh.indexCount);
        mesh.dirty = true;
        XrVector3f scale{1, 1, 1};
        mesh.scale = scale;
        XrSpaceLocationFlags spaceLocationFlags;
        plugin.spatialAnchorMgr->getSpatialAnchorPose((uint64_t)meshBlockSpace, &mesh.position, &mesh.rotation, &spaceLocationFlags);
        mesh.position = ConvertPoseBetweenOpenXRandUnity(mesh.position);
        mesh.rotation = ConvertPoseBetweenOpenXRandUnity(mesh.rotation);
    }

    return mesh;
}

UnitySubsystemErrorCode YVRMeshingProvider::start()
{
    AnnounceCallingFunc();
    return kUnitySubsystemErrorCodeSuccess;
}

void YVRMeshingProvider::shutDown()
{
    AnnounceCallingFunc();
    clearMeshCache();
}

void YVRMeshingProvider::stop()
{
    AnnounceCallingFunc();
}

YVRMeshingProvider::~YVRMeshingProvider()
{
    clearMeshCache();
}
