#include "XRMotionData.h"

XRMotionData::XRMotionData()
    : acceleration{XR_TYPE_SPACE_ACCELERATION_EXT_YVR},
      velocity{XR_TYPE_SPACE_VELOCITY, &acceleration},
      location{XR_TYPE_SPACE_LOCATION, &velocity}
{}
