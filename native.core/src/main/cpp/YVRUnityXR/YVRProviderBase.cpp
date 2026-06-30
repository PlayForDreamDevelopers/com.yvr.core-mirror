#include "YVRProviderBase.h"

YVRProviderBase::YVRProviderBase(YVRUnityXRProviderMgr& context, UnitySubsystemHandle handle)
    : m_Context(context), m_SystemHandle(handle) {}

YVRProviderBase::~YVRProviderBase() {}
