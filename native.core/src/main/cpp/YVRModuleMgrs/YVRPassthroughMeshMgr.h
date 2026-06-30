#pragma once
#include "../OpenXR/pch.h"
#include "pfdm_openxr_preview/openxr_yvr_private.h"
#include "YVRSceneMgr.h"

typedef struct PersistentPassthroughMeshModeInfo
{
    bool followMode;
    XrControllerYVR controller;
} PersistentPassthroughMeshModeInfo;

typedef struct PersistentPassthroughMeshTransform
{
    XrPosef                     pose;
    XrVector3f                  scale;
} PersistentPassthroughMeshTransform;

class YVRPassthroughMeshMgr
{
  public:
    YVRPassthroughMeshMgr();
    ~YVRPassthroughMeshMgr();

    void SetPersistentPassthroughMeshInfo(PersistentPassthroughMeshModeInfo setInfo);
    void GetPersistentPassthroughMeshInfo(PersistentPassthroughMeshModeInfo* getInfo);
    void SetPersistentPassthroughMesh(SpaceTriangleMesh* mesh);
    void SetPersistentPassthroughTransform(PersistentPassthroughMeshTransform transform);
};