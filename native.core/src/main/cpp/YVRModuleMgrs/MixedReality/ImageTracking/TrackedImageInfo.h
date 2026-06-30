#pragma once
#include "openxr/openxr.h"

#define IMAGE_ID_MAX_LEN 64

struct TrackedImageInfo
{
    XrPosef pose;
    char imageId[IMAGE_ID_MAX_LEN];
    XrBool32 isActive;
    float confidence;
    XrTime time;
    XrExtent2Df size;
};
