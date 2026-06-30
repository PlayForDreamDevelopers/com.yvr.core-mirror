#include "YVRDisplayProvider.h"

UnitySubsystemErrorCode YVRDisplayProvider::Initialize()
{
    plugin.StartPreviewToolService();
    plugin.previewToolService->GetDirect3DDeviceID(m_Context.interfaces->Get<IUnityGraphicsD3D11>()->GetDevice());
    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::Start()
{
    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::GfxThreadStart(UnityXRRenderingCapabilities& renderingCaps)
{
    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::GfxThreadSubmitCurrentFrame()
{
    if (m_NativeTextures.size() > 0 && yvrTrackingMgr.hmdState.targetTimestampNs!= 0)
    {
        plugin.previewToolService->Present((uint64_t)m_TextureHandle[m_TextureIndex % m_SwapChainCount]);
    }

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::GfxThreadPopulateNextFrameDesc(const UnityXRFrameSetupHints& frameHints, UnityXRNextFrameDesc& nextFrame)
{
    WORKAROUND_SKIP_FIRST_FRAME();

	bool reallocateTextures = (m_UnityTextures.size() == 0);
    if (plugin.previewToolService->serviceConnection->isConnected)
    {
        reallocateTextures = plugin.renderConfigsMgr->isReallocateTexture();
    }

    if ((kUnityXRFrameSetupHintsChangedSinglePassRendering & frameHints.changedFlags) != 0)
    {
        reallocateTextures = true;
    }
    if ((kUnityXRFrameSetupHintsChangedTextureResolutionScale & frameHints.changedFlags) != 0)
    {
        reallocateTextures = true;
    }

    if (reallocateTextures)
    {
        DestroyTextures();

#if SIDE_BY_SIDE
        int numTextures = m_SwapChainCount;
        int textureArrayLength = 0;
#else
        int numTextures = frameHints.appSetup.singlePassRendering ? NUM_RENDER_PASSES - 1 : NUM_RENDER_PASSES;
        int textureArrayLength = frameHints.appSetup.singlePassRendering ? 2 : 0;
#endif
        CreateTextures(numTextures, textureArrayLength, frameHints.appSetup.textureResolutionScale);
        plugin.renderConfigsMgr->setReallocateTexture(false);
    }

    // Frame hints tells us if we should setup our renderpasses with a single pass
	if (!plugin.renderConfigsMgr->isUsingSinglePass())
    {
        // Use multi-pass rendering to render
        // Can increase render pass count to do wide FOV or to have a separate view into scene.

        nextFrame.renderPassesCount = NUM_RENDER_PASSES;

        for (int pass = 0; pass < nextFrame.renderPassesCount; ++pass)
        {
            auto& renderPass = nextFrame.renderPasses[pass];

            // Texture that unity will render to next frame.  We created it above.
            // You might want to change this dynamically to double / triple buffer.
#if !SIDE_BY_SIDE
            renderPass.textureId = m_UnityTextures[pass];
#else
			renderPass.textureId = m_UnityTextures[m_TextureIndex % m_SwapChainCount];
#endif

            // One set of render params per pass.
            renderPass.renderParamsCount = 1;

            // Note that you can share culling between multiple passes by setting this to the same index.
            renderPass.cullingPassIndex = pass;

            // Fill out render params. View, projection, viewport for pass.
            auto& cullingPass = nextFrame.cullingPasses[pass];
            cullingPass.separation = fabs(s_PoseXPositionPerPass[1]) + fabs(s_PoseXPositionPerPass[0]);

            auto& renderParams = renderPass.renderParams[0];    
            renderParams.deviceAnchorToEyePose = cullingPass.deviceAnchorToCullingPose = GetPose(pass);
            renderParams.projection = cullingPass.projection = GetProjection(pass);

#if !SIDE_BY_SIDE
            // App has hinted that it would like to render to a smaller viewport.  Tell unity to render to that viewport.
            renderParams.viewportRect = frameHints.appSetup.renderViewport;

            // Tell the compositor what pixels were rendered to for display.
            // Compositor_SetRenderSubRect(pass, renderParams.viewportRect);
#else
            // TODO: frameHints.appSetup.renderViewport
            renderParams.viewportRect = {
                pass == 0 ? 0.0f : 0.5f, // x
                0.0f,                    // y
                0.5f,                    // width
                1.0f                     // height
            };
#endif
        }
    }
    else
    {
        // Example of using single-pass stereo to combine the first two render passes.
        nextFrame.renderPassesCount = NUM_RENDER_PASSES - 1;

        UnityXRNextFrameDesc::UnityXRRenderPass& renderPass = nextFrame.renderPasses[0];

        // Texture that unity will render to next frame.  We created it above.
        // You might want to change this dynamically to double / triple buffer.
        m_TextureIndex++;
		renderPass.textureId = m_UnityTextures[m_TextureIndex % m_SwapChainCount];

        // Two sets of render params for first pass, view / projection for each eye.  Fill them out next.
        renderPass.renderParamsCount = 2;

        for (int eye = 0; eye < 2; ++eye)
        {
            UnityXRNextFrameDesc::UnityXRRenderPass::UnityXRRenderParams& renderParams = renderPass.renderParams[eye];
            renderParams.deviceAnchorToEyePose = GetPose(eye);
            renderParams.projection = GetProjection(eye);

#if SIDE_BY_SIDE
            // TODO: frameHints.appSetup.renderViewport
            renderParams.viewportRect = {
                eye == 0 ? 0.0f : 0.5f, // x
                0.0f,                   // y
                0.5f,                   // width
                1.0f                    // height
            };
#else
            // Each eye goes to different texture array slices.
            renderParams.textureArraySlice = eye;
#endif
        }

        renderPass.cullingPassIndex = 0;

        auto& cullingPass = nextFrame.cullingPasses[0];
        cullingPass.deviceAnchorToCullingPose = GetPose(0);
        cullingPass.projection = GetProjection(0);
        cullingPass.separation = 0.625f;
    }

    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::GfxThreadStop()
{
    WORKAROUND_RESET_SKIP_FIRST_FRAME();
    return kUnitySubsystemErrorCodeSuccess;
}

UnitySubsystemErrorCode YVRDisplayProvider::GfxThreadFinalBlitToGameViewBackBuffer(const UnityXRMirrorViewBlitInfo* mirrorBlitInfo, ProviderContext& ctx)
{
#if XR_DX11
    ID3D11Device* dxDevice = ctx.interfaces->Get<IUnityGraphicsD3D11>()->GetDevice();
    ID3D11RenderTargetView* rtv = ctx.interfaces->Get<IUnityGraphicsD3D11>()->RTVFromRenderBuffer(mirrorBlitInfo->mirrorRtDesc->rtNative);
    ID3D11DeviceContext* immContext;
    dxDevice->GetImmediateContext(&immContext);

    immContext->OMSetRenderTargets(1, &rtv, NULL);
    // clear to blue
    const FLOAT clrColor[4] = { 0, 0, 1, 1 };
    immContext->ClearRenderTargetView(rtv, clrColor);
#endif

    return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeSuccess;
}

void YVRDisplayProvider::Stop()
{
}

void YVRDisplayProvider::Shutdown()
{
    plugin.StopPreviewToolService();
}

void YVRDisplayProvider::CreateTextures(int numTextures, int textureArrayLength, float requestedTextureScale)
{
	int texWidth = (int)(1600 * requestedTextureScale * (SIDE_BY_SIDE ? 2.0f : 1.0f));
	int texHeight = (int)(1600 * requestedTextureScale);
	if (plugin.previewToolService->serviceConnection->isConnected)
	{
		plugin.previewToolService->GetDisplayResolution(texWidth, texHeight);
	}

	m_NativeTextures.resize(numTextures);
	m_UnityTextures.resize(numTextures);
	m_TextureHandle.resize(numTextures);
	// Tell unity about the native textures, getting back UnityXRRenderTextureIds.
	ID3D11Device* dxDevice = m_Context.interfaces->Get<IUnityGraphicsD3D11>()->GetDevice();
	ID3D11DeviceContext* immContext;
	dxDevice->GetImmediateContext(&immContext);
	for (int i = 0; i < numTextures; ++i)
	{
		UnityXRRenderTextureDesc uDesc{};
#if XR_DX11
		//creating textures for unity to render in to.
		DXGI_FORMAT format = plugin.renderConfigsMgr->getColorSpace() == yvrColorSpace::kColorSpaceSRGB ?
			DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM_SRGB : DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		D3D11_TEXTURE2D_DESC dxDesc = CD3D11_TEXTURE2D_DESC(format, texWidth, texHeight);
		dxDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		dxDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
		dxDesc.Usage = D3D11_USAGE_DEFAULT;
		dxDesc.ArraySize = (textureArrayLength == 0 ? 1 : textureArrayLength);
		dxDesc.MipLevels = 1;
		ID3D11Texture2D* nativeTex;
		dxDevice->CreateTexture2D(&dxDesc, NULL, &nativeTex);
		m_NativeTextures[i] = nativeTex;
		uDesc.color.nativePtr = nativeTex;
#else
		// Telling Unity to create the texture.  You can later obtain the native texture resource with
		// QueryTextureDesc
		uDesc.color.nativePtr = (void*)kUnityXRRenderTextureIdDontCare;
#endif
		uDesc.colorFormat = kUnityXRRenderTextureFormatRGBA32;
		uDesc.width = texWidth;
		uDesc.height = texHeight;
		uDesc.textureArrayLength = textureArrayLength;
		if (plugin.renderConfigsMgr->getColorSpace() == yvrColorSpace::kColorSpaceSRGB)
			uDesc.flags |= kUnityXRRenderTextureFlagsSRGB;

		// Create an UnityXRRenderTextureId for the native texture so we can tell unity to render to it later.
		UnityXRRenderTextureId uTexId;
		m_Context.display->CreateTexture(m_Handle, &uDesc, &uTexId);
		IDXGIResource1* pDXGIResource(NULL);
		m_NativeTextures[i]->QueryInterface(__uuidof(IDXGIResource), (void**)&pDXGIResource);

		HANDLE handle;
		pDXGIResource->GetSharedHandle(&handle);
		m_TextureHandle[i] = handle;
		m_UnityTextures[i] = uTexId;
		pDXGIResource->Release();
	}
}

void YVRDisplayProvider::DestroyTextures()
{
#if XR_DX11
    assert(m_NativeTextures.size() == m_UnityTextures.size());
#endif

    for (int i = 0; i < m_UnityTextures.size(); ++i)
    {
        if (m_UnityTextures[i] != 0)
        {
            m_Context.display->DestroyTexture(m_Handle, m_UnityTextures[i]);
#if XR_DX11
            m_NativeTextures[i]->Release();
#endif
        }
    }

    m_UnityTextures.clear();
    m_NativeTextures.clear();
    m_TextureHandle.clear();
}

UnityXRPose YVRDisplayProvider::GetPose(int pass)
{
    UnityXRPose pose{};
	float* eyeBias = plugin.renderConfigsMgr->getEyeBias(pass);
    if (eyeBias[4] == 0)
    {
        pose.position.x = pass == 0 ? -0.032f : 0.032f;
        pose.rotation.w = 1.0f;
    }
    else
    {
        memcpy(&pose.rotation, &eyeBias[0], sizeof(UnityXRVector4));
        memcpy(&pose.position, &eyeBias[4], sizeof(UnityXRVector3));

        pose.position.z *= -1;
        pose.rotation.x *= -1;
        pose.rotation.y *= -1;
    }

    return pose;
}

UnityXRProjection YVRDisplayProvider::GetProjection(int pass)
{
    UnityXRProjection ret;
    ret.type = kUnityXRProjectionTypeHalfAngles;
    ret.data.halfAngles.left = -1.09f;
    ret.data.halfAngles.right = 1.09f;
    ret.data.halfAngles.top = 1.09f;
    ret.data.halfAngles.bottom = -1.09f;
    return ret;
}

UnitySubsystemErrorCode YVRDisplayProvider::QueryMirrorViewBlitDesc(const UnityXRMirrorViewBlitInfo* mirrorBlitInfo, UnityXRMirrorViewBlitDesc* blitDescriptor, ProviderContext& ctx)
{
    if (ctx.yvrDisplayProvider->m_UnityTextures.size() == 0)
    {
        return UnitySubsystemErrorCode::kUnitySubsystemErrorCodeFailure;
    }
	int srcTexId = ctx.yvrDisplayProvider->m_UnityTextures[m_TextureIndex % m_SwapChainCount];
    const UnityXRVector2 sourceTextureSize = { static_cast<float>(1600*2), static_cast<float>(1600) };
    const UnityXRRectf sourceUVRect = { 0.5f, 0.0f, 0.5f, 1.0f };
    const UnityXRVector2 destTextureSize = { static_cast<float>(mirrorBlitInfo->mirrorRtDesc->rtScaledWidth), static_cast<float>(mirrorBlitInfo->mirrorRtDesc->rtScaledHeight) };
    const UnityXRRectf destUVRect = { 0.0f, 0.0f, 1.0f, 1.0f };

    UnityXRVector2 sourceUV0, sourceUV1, destUV0, destUV1;

    float sourceAspect = (sourceTextureSize.x * sourceUVRect.width) / (sourceTextureSize.y * sourceUVRect.height);
    float destAspect = (destTextureSize.x * destUVRect.width) / (destTextureSize.y * destUVRect.height);
    float ratio = sourceAspect / destAspect;
    UnityXRVector2 sourceUVCenter = { sourceUVRect.x + sourceUVRect.width * 0.5f, sourceUVRect.y + sourceUVRect.height * 0.5f };
    UnityXRVector2 sourceUVSize = { sourceUVRect.width, sourceUVRect.height };
    UnityXRVector2 destUVCenter = { destUVRect.x + destUVRect.width * 0.5f, destUVRect.y + destUVRect.height * 0.5f };
    UnityXRVector2 destUVSize = { destUVRect.width, destUVRect.height };

    if (ratio > 1.0f)
    {
        sourceUVSize.x /= ratio;
    }
    else
    {
        sourceUVSize.y *= ratio;
    }

    sourceUV0 = { sourceUVCenter.x - (sourceUVSize.x * 0.5f), sourceUVCenter.y - (sourceUVSize.y * 0.5f) };
    sourceUV1 = { sourceUV0.x + sourceUVSize.x, sourceUV0.y + sourceUVSize.y };
    destUV0 = { destUVCenter.x - destUVSize.x * 0.5f, destUVCenter.y - destUVSize.y * 0.5f };
    destUV1 = { destUV0.x + destUVSize.x, destUV0.y + destUVSize.y };

    (*blitDescriptor).blitParamsCount = 1;
    (*blitDescriptor).blitParams[0].srcTexId = srcTexId;
    (*blitDescriptor).blitParams[0].srcTexArraySlice = 0;
    (*blitDescriptor).blitParams[0].srcRect = { sourceUV0.x, sourceUV0.y, sourceUV1.x - sourceUV0.x, sourceUV1.y - sourceUV0.y };
    (*blitDescriptor).blitParams[0].destRect = { destUV0.x, destUV0.y, destUV1.x - destUV0.x, destUV1.y - destUV0.y };
    return kUnitySubsystemErrorCodeSuccess;
}

// Binding to C-API below here
static UnitySubsystemErrorCode UNITY_INTERFACE_API Display_Initialize(UnitySubsystemHandle handle, void* userData)
{
    auto& ctx = GetProviderContext(userData);

    ctx.yvrDisplayProvider = new YVRDisplayProvider(ctx, handle);

    // Register for callbacks on the graphics thread.
    UnityXRDisplayGraphicsThreadProvider gfxThreadProvider{};
    gfxThreadProvider.userData = &ctx;

    gfxThreadProvider.Start = [](UnitySubsystemHandle handle, void* userData, UnityXRRenderingCapabilities* renderingCaps) -> UnitySubsystemErrorCode {
        auto& ctx = GetProviderContext(userData);
        return ctx.yvrDisplayProvider->GfxThreadStart(*renderingCaps);
    };

    gfxThreadProvider.SubmitCurrentFrame = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode {
        auto& ctx = GetProviderContext(userData);
        return ctx.yvrDisplayProvider->GfxThreadSubmitCurrentFrame();
    };

    gfxThreadProvider.PopulateNextFrameDesc = [](UnitySubsystemHandle handle, void* userData, const UnityXRFrameSetupHints* frameHints, UnityXRNextFrameDesc* nextFrame) -> UnitySubsystemErrorCode {
        auto& ctx = GetProviderContext(userData);
        return ctx.yvrDisplayProvider->GfxThreadPopulateNextFrameDesc(*frameHints, *nextFrame);
    };

    gfxThreadProvider.Stop = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode {
        auto& ctx = GetProviderContext(userData);
        return ctx.yvrDisplayProvider->GfxThreadStop();
    };

    gfxThreadProvider.BlitToMirrorViewRenderTarget = [](UnitySubsystemHandle handle, void* userData, const UnityXRMirrorViewBlitInfo mirrorBlitInfo) -> UnitySubsystemErrorCode {
        auto& ctx = GetProviderContext(userData);
        return ctx.yvrDisplayProvider->GfxThreadFinalBlitToGameViewBackBuffer(&mirrorBlitInfo, ctx);
    };

    ctx.display->RegisterProviderForGraphicsThread(handle, &gfxThreadProvider);

    UnityXRDisplayProvider provider{ &ctx, NULL, NULL };
    provider.QueryMirrorViewBlitDesc = [](UnitySubsystemHandle handle, void* userData, const UnityXRMirrorViewBlitInfo mirrorBlitInfo, UnityXRMirrorViewBlitDesc* blitDescriptor) -> UnitySubsystemErrorCode {
        auto& ctx = GetProviderContext(userData);
        return ctx.yvrDisplayProvider->QueryMirrorViewBlitDesc(&mirrorBlitInfo, blitDescriptor, ctx);
    };

    ctx.display->RegisterProvider(handle, &provider);

    return ctx.yvrDisplayProvider->Initialize();
}

UnitySubsystemErrorCode LoadDisplay(ProviderContext& ctx)
{
    ctx.display = ctx.interfaces->Get<IUnityXRDisplayInterface>();
    if (ctx.display == NULL)
        return kUnitySubsystemErrorCodeFailure;

    UnityLifecycleProvider displayLifecycleHandler{};
    displayLifecycleHandler.userData = &ctx;
    displayLifecycleHandler.Initialize = &Display_Initialize;

    displayLifecycleHandler.Start = [](UnitySubsystemHandle handle, void* userData) -> UnitySubsystemErrorCode {
        auto& ctx = GetProviderContext(userData);
        return ctx.yvrDisplayProvider->Start();
    };

    displayLifecycleHandler.Stop = [](UnitySubsystemHandle handle, void* userData) -> void {
        auto& ctx = GetProviderContext(userData);
        ctx.yvrDisplayProvider->Stop();
    };

    displayLifecycleHandler.Shutdown = [](UnitySubsystemHandle handle, void* userData) -> void {
        auto& ctx = GetProviderContext(userData);
        ctx.yvrDisplayProvider->Shutdown();
        delete ctx.yvrDisplayProvider;
    };

    return ctx.display->RegisterLifecycleProvider("YVR XR SDK", "Display", &displayLifecycleHandler);
}
