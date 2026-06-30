#pragma once

#include "../UnityHeader/XR/UnitySubsystemTypes.h"

class YVRUnityXRProviderMgr;

inline YVRUnityXRProviderMgr& GetYVRProviderMgr(void* data) { return *static_cast<YVRUnityXRProviderMgr*>(data); }

class YVRProviderBase
{
public:
    YVRProviderBase(YVRUnityXRProviderMgr& context, UnitySubsystemHandle handle);
    virtual ~YVRProviderBase();

    virtual UnitySubsystemErrorCode initialize() = 0;
    virtual UnitySubsystemErrorCode start() = 0;
    virtual void stop() = 0;
    virtual void shutDown() = 0;

protected:
    YVRUnityXRProviderMgr& m_Context;
    UnitySubsystemHandle m_SystemHandle;
};
