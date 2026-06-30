#pragma once
#if !UNITY
#include "IUnityInterface.h"
#endif
#include "IUnityXRDisplay.h"

/// @file IUnityXRDisplayShadingRateExt.h
/// @brief XR extension interface for texture allocation with shading rate image
/// @see IUnityXRDisplayShadingRateExt

/// Format for color texture.
typedef enum UnityXRRenderTextureFormat_ShadingRateExt
{
    /// R8 G8 B8 A8
    kUnityXRRenderTextureFormatRGBA32_ShadingRateExt,

    /// B8 G8 R8 A8
    kUnityXRRenderTextureFormatBGRA32_ShadingRateExt,

    /// R5 G6 B5
    kUnityXRRenderTextureFormatRGB565_ShadingRateExt,

    /// Don't create a color texture, instead create a reference to another color texture that's already been created.  Must fill out UnityTextureData::referenceTextureId.
    kUnityXRRenderTextureFormatReference_ShadingRateExt,

    /// No color texture.
    kUnityXRRenderTextureFormatNone_ShadingRateExt
} UnityXRRenderTextureFormat_ShadingRateExt;

/// Precision of depth texture.
typedef enum UnityXRDepthTextureFormat_ShadingRateExt
{
    /// 24-bit or greater depth texture.  Unity will prefer 32 bit floating point Z buffer if available on the platform.
    kUnityXRDepthTextureFormat24bitOrGreater_ShadingRateExt,

    /// If possible, use a 16-bit texture format for bandwidth savings.
    kUnityXRDepthTextureFormat16bit_ShadingRateExt,

    /// Don't create a depth texture, instead create a reference to another depth texture that's already been created.  Must fill out UnityTextureData::referenceTextureId.
    /// This is useful for sharing a single depth texture between double/triple buffered color textures (of the same width/height).
    kUnityXRDepthTextureFormatReference_ShadingRateExt,

    /// No depth texture.
    kUnityXRDepthTextureFormatNone_ShadingRateExt
} UnityXRDepthTextureFormat_ShadingRateExt;

/// Container for different ways of representing texture data.
/// If the format (#UnityXRRenderTextureDesc::colorFormat or #UnityXRRenderTextureDesc::depthFormat) is a 'Reference' format, referenceTextureId must be set.
/// Otherwise, nativePtr will be used.
typedef union UnityXRTextureData_ShadingRateExt
{
    /// @brief Native texture ID if you've allocated it yourself.  The texture ID varies by graphics API.
    ///
    ///  - GL: texture name that comes from glGenTextures
    ///  - DX11: ID3D11Texture2D*
    ///  - etc.
    ///
    /// You can pass in #kUnityXRRenderTextureIdDontCare and have Unity allocate one for you.
    void* nativePtr;

    /// Texture id to share color / depth with in the case of reference color / depth format.
    UnityXRRenderTextureId referenceTextureId;
} UnityXRTextureData_ShadingRateExt; ///< Contains all supported ways of representing texture data.

/// Description of a texture that the plugin can request to be allocated via IUnityXRDisplayInterface::CreateTexture.  Encapsulates both color and depth surfaces.
typedef struct UnityXRRenderTextureDesc_ShadingRateExt
{
    /// Color format of the texture.  Format will be sRGB if kUnityXRRenderTextureFlagsSRGB is set and there is an equivalent sRGB native format.
    UnityXRRenderTextureFormat_ShadingRateExt colorFormat;

    /// Data for color texture.
    UnityXRTextureData_ShadingRateExt color;

    /// Depth format of the texture.
    UnityXRDepthTextureFormat_ShadingRateExt depthFormat;

    /// Data for depth texture.
    UnityXRTextureData_ShadingRateExt depth;

    /// Width of the texture in pixels.
    uint32_t width;

    /// Height of the texture in pixels.
    uint32_t height;

    /// If requesting a texture array, the length of the texture array.
    uint32_t textureArrayLength;

    /// Combination of #UnityXRRenderTextureFlags.
    uint32_t flags;
} UnityXRRenderTextureDesc_ShadingRateExt;

/// @brief Exposes entry points related to shading rate
UNITY_DECLARE_INTERFACE(IUnityXRDisplayShadingRateExt)
{
    /// Provides the same functionality as IUnityXRDisplayInterface::CreateTexture, but also allows for supplying a shading rate image.
    /// This only works with the Vulkan graphics API if VK_EXT_fragment_density_map is available.
    ///
    /// @param[in] handle Handle obtained from UnityLifecycleProvider callbacks.
    /// @param[in] colorDepthDesc Descriptor of the color/depth textures to be created.
    /// @param[in] shadingRateTexture Native texture pointer for a shading rate image.
    /// @param[out] outTexId Texture ID representing a unique instance of a texture.
    /// @return kUnitySubsystemErrorCodeSuccess Successfully requested creation of texture.
    /// @return kUnitySubsystemErrorCodeInvalidArguments Invalid / null parameters
    /// @return kUnitySubsystemErrorCodeFailure Error
    UnitySubsystemErrorCode(UNITY_INTERFACE_API * CreateTextureWithShadingRate)(UnitySubsystemHandle handle, const UnityXRRenderTextureDesc_ShadingRateExt* colorDepthDesc, const UnityXRTextureData_ShadingRateExt* shadingRateTexture, UnityXRRenderTextureId* outTexId);
};
UNITY_REGISTER_INTERFACE_GUID(0x1066FBD7A90E4A74ULL, 0xBCCBB730B26DE473ULL, IUnityXRDisplayShadingRateExt)
