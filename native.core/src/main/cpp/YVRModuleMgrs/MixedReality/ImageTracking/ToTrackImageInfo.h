#pragma once
#include "openxr/openxr.h"

#define IMAGE_ID_MAX_LEN 64
#define IMAGE_PATH_MAX_LEN 256

struct ToTrackImageInfo
{
    float physicalWidth;
    float physicalHeight;
    char imageId[IMAGE_ID_MAX_LEN];
    char templatePath[IMAGE_PATH_MAX_LEN];
};
