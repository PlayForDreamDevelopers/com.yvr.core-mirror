#pragma once
#include "openxr/openxr.h"
#include "./PassthroughMode.h"

typedef struct PassthroughStyle_
{
    PassthroughMode textureColorMapType;
    XrPassthroughColorLutMETA lutSource;
    float lutWeight;

} PassthroughStyle;
