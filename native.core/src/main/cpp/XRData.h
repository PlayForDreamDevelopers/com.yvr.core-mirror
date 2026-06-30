#pragma once

#define EYE_NUM 2
#define SWAP_BUFFER_NUM 3  // How many buffer in single swap chain

typedef enum xrEyeSide_
{
    kEyeMaskLeft = 0x00000001,
    kEyeMaskRight = 0x00000002,
    kEyeMaskBoth = 0x00000003
} xrEyeSide;

typedef enum xrControllerSide_
{
    kControllerLeft = 0,
    kControllerRight = 1,
    kNumControllers = 2
} xrControllerSide;

typedef enum xrTextureType_
{
    kTypeTexture = 0,      //!< Standard texture
    kTypeTextureArray,     //!< Standard texture array (Left eye is first layer, right eye is second layer)
    kTypeImage,            //!< EGL Image texture
    kTypeEquiRectTexture,  //!< Equirectangular texture
    kTypeEquiRectImage,    //!< Equirectangular Image texture
    kTypeCubemapTexture,   //!< Cubemap texture (Not supporting cubemap image)
    kTypeVulkan,           //!< Vulkan texture
} xrTextureType;

typedef enum xrLayerFlags_
{
    kLayerFlagNone = 0x00000000,
    kLayerFlagHeadLocked = 0x00000001,
    kLayerFlagOpaque = 0x00000002,
    kLayerFlagSubsampled = 0x00000004,
    kLayerFlagPremultipliedAlpha = 0x00000008,
    kLayerFlagBlendTextureSourceAlpha = 0x00000010,
    kLayerFlagFlipY = 0x00000020,
    kLayerFlagDepthStencil = 0x00000040,
} xrLayerFlags;
