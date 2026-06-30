#include "Headers/XR/IUnityXRDisplay.h"
#include "Headers/XR/IUnityXRTrace.h"
#include "ProviderContext.h"
#include "YVRTrackingMgr.h"

// We'll use DX11 to allocate textures if we're on windows.
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WINAPI_FAMILY)
#include "D3D11.h"

#include "Headers/XR/IUnityGraphicsD3D11.h"
#define XR_DX11 1
#else
#define XR_DX11 0
#endif
#include "WriteLog.h"
#include "YVRUnityPlugin.h"
#include <vector>
#include "timer.h"

#define SIDE_BY_SIDE 1
#define NUM_RENDER_PASSES 2
static const float s_PoseXPositionPerPass[] = { -1.0f, 1.0f };

// BEGIN WORKAROUND: skip first frame since we get invalid data.  Fix coming to trunk.
static bool s_SkipFrame = true;
#define WORKAROUND_SKIP_FIRST_FRAME()           \
    if (s_SkipFrame)                            \
    {                                           \
        s_SkipFrame = false;                    \
        return kUnitySubsystemErrorCodeSuccess; \
    }
#define WORKAROUND_RESET_SKIP_FIRST_FRAME() s_SkipFrame = true;
// END WORKAROUND

class YVRDisplayProvider : ProviderImpl
{
public:

    YVRDisplayProvider(ProviderContext& ctx, UnitySubsystemHandle handle)
        : ProviderImpl(ctx, handle)
    {
    }
    std::vector<UnityXRRenderTextureId> m_UnityTextures;

    UnitySubsystemErrorCode Initialize() override;
    UnitySubsystemErrorCode Start() override;

    UnitySubsystemErrorCode GfxThreadStart(UnityXRRenderingCapabilities& renderingCaps);

    UnitySubsystemErrorCode GfxThreadSubmitCurrentFrame();
    UnitySubsystemErrorCode GfxThreadPopulateNextFrameDesc(const UnityXRFrameSetupHints& frameHints, UnityXRNextFrameDesc& nextFrame);

    UnitySubsystemErrorCode GfxThreadStop();
    UnitySubsystemErrorCode GfxThreadFinalBlitToGameViewBackBuffer(const UnityXRMirrorViewBlitInfo* mirrorBlitInfo, ProviderContext& ctx);

    UnitySubsystemErrorCode QueryMirrorViewBlitDesc(const UnityXRMirrorViewBlitInfo* mirrorRtDesc, UnityXRMirrorViewBlitDesc* blitDescriptor, ProviderContext& ctx);
    void Stop() override;
    void Shutdown() override;

private:
    void CreateTextures(int numTextures, int textureArrayLength, float requestedTextureScale);
    void DestroyTextures();

    UnityXRPose GetPose(int pass);
    UnityXRProjection GetProjection(int pass);

private:

    yvrHMDState m_HMDState{};
    yvrHMDState m_PreHmdState{};

    int m_TextureIndex;
    int m_SwapChainCount = 3;

#if XR_DX11
    std::vector<ID3D11Texture2D*> m_NativeTextures;
    std::vector<HANDLE> m_TextureHandle;
#else
    std::vector<void*> m_NativeTextures;
#endif
};