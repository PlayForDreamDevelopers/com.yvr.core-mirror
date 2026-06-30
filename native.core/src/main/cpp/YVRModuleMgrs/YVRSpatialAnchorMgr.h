#pragma once
#include "openxr/openxr.h"
#include <vector>
#include <map>

#define SpaceFilterInfoIdsMaxSize                  1024
#define UUIDSize                                   32
#define SpaceFilterInfoComponentMaxSize            16
#define ShareUserMaxSize                           1024

typedef struct YVRSpatialAnchorUUID_
{
    char uuid[UUIDSize];
} YVRSpatialAnchorUUID;

typedef struct YVRSpaceQueryInfo_
{
    uint32_t maxResultCount;
    XrDuration timeOut;
    XrSpaceStorageLocationFB storageLocation;
    XrSpaceComponentTypeFB component;
    uint32_t numIds;
    YVRSpatialAnchorUUID uuids[SpaceFilterInfoIdsMaxSize];
} YVRSpaceQueryInfo;

typedef struct YVRSpaceAnchorResult_
{
    uint64_t requestId;
    XrSpace space;
    char uuid[UUIDSize];
} YVRSpaceAnchorResult;

typedef struct YVRQuerySpaceResult_
{
    uint32_t numResults;
    YVRSpaceAnchorResult spaces[SpaceFilterInfoIdsMaxSize];
} YVRQuerySpaceResult;

typedef struct YVRSpaceSaveCompleteInfo_
{
    uint64_t requestId;
    XrResult resultCode;
    XrSpace space;
    char uuid[UUIDSize];
    XrSpaceStorageLocationFB location;
} YVRSpaceSaveCompleteInfo;

typedef struct YVRSpaceSetStatusCompleteInfo_
{
    uint64_t requestId;
    XrResult resultCode;
    XrSpace space;
    char uuid[UUIDSize];
    XrSpaceComponentTypeFB componentType;
    XrBool32 enabled;
} YVRSpaceSetStatusCompleteInfo;


typedef struct YVRSpaceAnchorSaveInfo_
{
    XrSpace space;
    XrSpaceStorageLocationFB storageLocation;
} YVRSpaceAnchorSaveInfo;

typedef struct YVRSpaceListSaveInfo_
{
    uint32_t spaceCount;
    XrSpace* spaces;
    XrSpaceStorageLocationFB location;
} YVRSpaceListSaveInfo;


typedef struct YVRSpaceSupportedComponent_
{
    uint32_t numComponents;
    XrSpaceComponentTypeFB components[SpaceFilterInfoComponentMaxSize];
} YVRSpaceSupportedComponent;

typedef struct YVRSpaceComponentStatusSetInfo_
{
    XrSpaceComponentTypeFB component;
    bool enable;
    XrDuration timeout;
} YVRSpaceComponentStatusSetInfo;

typedef struct YVRSpaceComponentStatus_
{
    bool enable;
    bool changePending;
} YVRSpaceComponentStatus;

typedef struct YVRSpaceShareInfo
{
    uint32_t spaceCount;
    XrSpace spaces[SpaceFilterInfoIdsMaxSize];
    uint32_t userCount;
    XrSpaceUserFB users[ShareUserMaxSize];
} YVRSpaceUser;

class YVRSpatialAnchorMgr
{

public:
    YVRSpatialAnchorMgr();
    ~YVRSpatialAnchorMgr();

    void createSpatialAnchorFB(XrVector3f position, XrQuaternionf rotation, uint64_t* createRequest);
    bool getSpatialAnchorPose(uint64_t space, XrVector3f* position, XrQuaternionf* rotation,XrSpaceLocationFlags* spaceLocationFlags);
    void saveSpatialAnchor(YVRSpaceAnchorSaveInfo spatialAnchorSaveInfo, uint64_t* requestId);
    void getSpatialAnchor(YVRSpaceQueryInfo queryinfo, uint64_t* requestId);
    void retrieveSpaceQueryResults(uint64_t requestId);
    void destroyAnchor(uint64_t space, XrSpaceStorageLocationFB storageLocation, uint64_t* requestId);
    void enumerateSpaceSupported(uint64_t space,YVRSpaceSupportedComponent* supportedComponent);
    bool setSpaceComponentStatus(uint64_t space,YVRSpaceComponentStatusSetInfo componentStatusSetInfo, uint64_t* requestId);
    void getSpaceComponentStatus(uint64_t space,XrSpaceComponentTypeFB component, YVRSpaceComponentStatus* componentStatus);
    void createSpaceUser(uint64_t userId, XrSpaceUserFB* spaceUser);
    void shareSpace(YVRSpaceShareInfo shareInfo, uint64_t* requestId);
    void saveSpatialAnchorList(YVRSpaceListSaveInfo saveInfo,  uint64_t* requestId);
    void getSpaceUserId(XrSpaceUserFB spaceUser, uint64_t* userId);
    void getSpaceUuid(uint64_t spaceHandle, YVRSpatialAnchorUUID* yvrUUID);
    std::map<uint64_t, std::vector<YVRSpaceAnchorResult>> spaceMap{};
};