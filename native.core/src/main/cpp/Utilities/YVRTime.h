#include "openxr/openxr.h"
#include <mutex>
#include <time.h>
#include "../YVRUnityPlugin.h"

inline void buildTime(int64_t timeNs, struct timespec& timeDuration)
{
    memset(&timeDuration, 0x00, sizeof(struct timespec));
    timeDuration.tv_nsec += timeNs;
    while (timeDuration.tv_nsec >= 1000 * 1000000l)
    {
        timeDuration.tv_sec += 1;
        timeDuration.tv_nsec -= 1000 * 1000000l;
    }
}


inline XrTime GetCurrentTime(clockid_t clk_id)
{
    struct timespec tp;

    const int status = clock_gettime(clk_id, &tp);
    if (status != 0)
    {
        YInfo("clock_gettime status=%i", status);
        return 0;
    }
    const uint64_t result = (uint64_t)tp.tv_sec * (uint64_t)(1000 * 1000 * 1000) + uint64_t(tp.tv_nsec);

    XrTime time{};
    timespec timeSpec{};
    buildTime(result, timeSpec);
    xrConvertTimespecTimeToTimeKHR(plugin.openxrMgr->program->instance, &timeSpec, &time);

    return time;
}
