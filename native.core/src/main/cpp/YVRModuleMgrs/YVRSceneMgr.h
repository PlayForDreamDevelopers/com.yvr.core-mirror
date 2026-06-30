#pragma once
#include "openxr/openxr.h"
#include "pfdm_openxr_preview/openxr_yvr_private.h"
#include <vector>
#include <map>
#include "YVRSpatialAnchorMgr.h"

typedef struct YVRRect2D_
{
    XrVector2f offset;
    XrVector2f extent;
} YVRRect2D;

typedef struct YVRRect3D_
{
    XrVector3f offset;
    XrVector3f extent;
} YVRRect3D;

typedef struct YVRRoomLayout_
{
    YVRSpatialAnchorUUID floorUuid;
    YVRSpatialAnchorUUID ceilingUuid;
    uint32_t wallUuidCapacityInput;
    uint32_t wallUuidCountOutput;
    YVRSpatialAnchorUUID* wallUuids;
} YVRRoomLayout;

typedef struct YVRBoundary2D_
{
    uint32_t vertexCapacityInput;
    uint32_t vertexCountOutput;
    XrVector2f* vertices;
} YVRBoundary2D;

typedef struct YVRAnchorSemanticLabel_
{
    uint32_t byteCapacityInput;
    uint32_t byteCountOutput;
    char* labels;
} YVRAnchorSemanticLabel;

typedef struct YVRSceneAnchorContainer_
{
    uint32_t anchorCapacityInput;
    uint32_t anchorCountOutput;
    YVRSpatialAnchorUUID* anchors;
} YVRSceneAnchorContainer;

typedef struct YVRSceneCaptureRequest_
{
    uint32_t requestByteCount;
    char* request;
} YVRSceneCaptureRequest;

typedef struct SpaceTriangleMesh_
{
    uint32_t              vertexCapacityInput;
    uint32_t              vertexCountOutput;
    XrVector3f*           vertices;
    uint32_t              indexCapacityInput;
    uint32_t              indexCountOutput;
    uint32_t*             indices;
} SpaceTriangleMesh;


class YVRSceneMgr
{
private:

public:
    YVRSceneMgr();
    ~YVRSceneMgr();
    XrResult getSpaceBoundingBox2D(XrSpace space, XrRect2Df* boundingBox2DOutput);
    XrResult getSpaceBoundingBox3D(XrSpace space, XrRect3DfFB* boundingBox3DOutput);
    XrResult getSpaceSemanticLabels(XrSpace space, YVRAnchorSemanticLabel* semanticLabelsOutput);
    XrResult getSpaceBoundary2D(XrSpace space,YVRBoundary2D* boundary2DOutput);
    XrResult getSpaceRoomLayout(XrSpace space,YVRRoomLayout* roomLayoutOutput);
    XrResult getSpaceContainer(XrSpace space,YVRSceneAnchorContainer* spaceContainer);
    XrResult requestSceneCapture(YVRSceneCaptureRequest* request, uint64_t* requestId);
    XrResult getSceneStatus(uint32_t* status);
    XrResult getLocateRoom(XrLocateRoomInfoYVR* roomInfo);
    XrResult resetScene();
    XrResult getSpaceTriangleMeshMETA(XrSpace space,SpaceTriangleMesh* spaceTriangleMesh);

    void createGlobalAnchor(XrVector3f position, XrQuaternionf rotation, uint64_t* createRequest);
    XrResult setSpaceBoundingBox2D(XrSpace space, XrRect2Df* boundingBox2DInput);
    XrResult setSpaceBoundingBox3D(XrSpace space, XrRect3DfFB* boundingBox3DInput);
    XrResult setSpaceSemanticLabels(XrSpace space, YVRAnchorSemanticLabel* semanticLabelsInput);
    XrResult setSpaceBoundary2D(XrSpace space, YVRBoundary2D* boundary2DInput);
    XrResult setSpaceRoomLayout(XrSpace space, YVRRoomLayout* roomLayoutInput);
    XrResult setSpaceContainer(XrSpace space, YVRSceneAnchorContainer* spaceContainerInput);
    XrResult setLocateRoom(XrLocateRoomInfoYVR* roomInfo);
    bool setSpaceTriangleMeshYVR(XrSpace space,SpaceTriangleMesh* spaceTriangleMesh);
    bool isValid(const XrUuidEXT& uuid);
    bool isComponentEnabled(XrSpace space, XrSpaceComponentTypeFB type);
};
