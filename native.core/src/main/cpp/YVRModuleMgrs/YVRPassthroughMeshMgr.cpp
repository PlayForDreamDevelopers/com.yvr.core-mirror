#include "YVRPassthroughMeshMgr.h"
#include "YVRSceneMgr.h"
#include "../Utilities/YVRCoordinateConverter.h"
#include "../YVRUnityPlugin.h"

YVRPassthroughMeshMgr::YVRPassthroughMeshMgr() {}
YVRPassthroughMeshMgr::~YVRPassthroughMeshMgr() {}

void YVRPassthroughMeshMgr::SetPersistentPassthroughMeshInfo(const PersistentPassthroughMeshModeInfo setInfo)
{
    AnnounceCallingFunc();
    XrPersistentPassthroughMeshModeInfo xrSetInfo = {XR_TYPE_PERSISTENT_PASSTHROUGH_MESH_MODE_INFO_YVR};
    xrSetInfo.next = nullptr;
    xrSetInfo.followMode = setInfo.followMode;
    xrSetInfo.controller = setInfo.controller;
    OpenXRAPI(xrSetPersistentPassthroughMeshModeInfoYVR(plugin.openxrMgr->program->session,&xrSetInfo));
}

void YVRPassthroughMeshMgr::GetPersistentPassthroughMeshInfo(PersistentPassthroughMeshModeInfo* getInfo)
{
    AnnounceCallingFunc();
    if (getInfo == nullptr)
    {
        YError("GetPersistentPassthroughMeshInfo failed: getInfo is null.");
        return;
    }

    XrPersistentPassthroughMeshModeInfo xrGetInfo = {XR_TYPE_PERSISTENT_PASSTHROUGH_MESH_MODE_INFO_YVR};
    OpenXRAPI(xrGetPersistentPassthroughMeshModeInfoYVR(plugin.openxrMgr->program->session,&xrGetInfo));
    getInfo->followMode = xrGetInfo.followMode;
    getInfo->controller = xrGetInfo.controller;
}

void YVRPassthroughMeshMgr::SetPersistentPassthroughMesh(SpaceTriangleMesh* mesh)
{
    AnnounceCallingFunc();
    XrSpaceTriangleMeshMETA xrMesh = {XR_TYPE_SPACE_TRIANGLE_MESH_META};
    xrMesh.next = nullptr;
    xrMesh.vertexCapacityInput = mesh->vertexCapacityInput;
    xrMesh.indexCapacityInput = mesh->indexCapacityInput;
    xrMesh.indices = mesh->indices;
    xrMesh.vertices = mesh->vertices;
    OpenXRAPI(xrSetPersistentPassthroughMeshYVR(plugin.openxrMgr->program->session,&xrMesh));
}

void YVRPassthroughMeshMgr::SetPersistentPassthroughTransform(PersistentPassthroughMeshTransform transform)
{
    AnnounceCallingFunc();
    XrPersistentPassthroughMeshTransformYVR xrMesh = {XR_TYPE_PERSISTENT_PASSTHROUGH_MESH_TRANSFORM_YVR};
    xrMesh.next = nullptr;
    xrMesh.space = plugin.inputMgr->appSpace;
    xrMesh.time = plugin.inputMgr->getTime(false);
    xrMesh.pose = ConvertPoseBetweenOpenXRandUnity(transform.pose);
    xrMesh.scale = transform.scale;
    OpenXRAPI(xrSetPersistentPassthroughMeshTransformYVR(plugin.openxrMgr->program->session, &xrMesh));
}
