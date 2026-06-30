#pragma once

#include <cassert>
struct IUnityXRTrace;
struct IUnityXRDisplayInterface;
struct IUnityXRInputInterface;

class YVRDisplayProvider;
class YVRInputProvider;

struct ProviderContext
{
    IUnityInterfaces* interfaces;
    IUnityXRTrace* trace;

    IUnityXRDisplayInterface* display;
    YVRDisplayProvider* yvrDisplayProvider;

    IUnityXRInputInterface* input;
    YVRInputProvider* yvrInputProvider;

};

struct YVRXRUserDefinedSettings
{
    bool use16BitDepthBuffer;
    bool useMonoscopic;
    bool useLinearColorSpace;
    short stereoRenderingMode;
    float resolutionScale;
};


inline ProviderContext& GetProviderContext(void* data)
{
    assert(data != NULL);
    return *static_cast<ProviderContext*>(data);
}

class ProviderImpl
{
public:
    ProviderImpl(ProviderContext& ctx, UnitySubsystemHandle handle)
        : m_Context(ctx)
        , m_Handle(handle)
    {
    }
    virtual ~ProviderImpl() {}

    virtual UnitySubsystemErrorCode Initialize() = 0;
    virtual UnitySubsystemErrorCode Start() = 0;

    virtual void Stop() = 0;
    virtual void Shutdown() = 0;

protected:
    ProviderContext& m_Context;
    UnitySubsystemHandle m_Handle;
};
