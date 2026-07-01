# Display and Rendering Configuration

This page describes common runtime APIs for display frequency, eye rendering data, dynamic resolution, IPD, and color space. If you only need to change the screen refresh rate, see [Screen Refresh Rate](./DisplayRefreshRate.md).

## Get Display Frequencies

Use `YVRManager.instance.cameraRenderer.displayFrequenciesAvailable` to get the display frequencies supported by the current device. Use `displayFrequency` to read or set the active frequency.

```csharp
float[] supportedFrequencies = YVRManager.instance.cameraRenderer.displayFrequenciesAvailable;
float currentFrequency = YVRManager.instance.cameraRenderer.displayFrequency;

YVRManager.instance.cameraRenderer.displayFrequency = 90;
```

## Get Eye Buffer Resolution and FOV

`YVRManager.instance.cameraRenderer.GetEyeRenderDesc` returns the render description for one eye, including eye buffer resolution, compatibility FOV, and the full left/right/up/down FOV.

```csharp
YVRCameraRenderer.EyeRenderDescription leftEye =
    YVRManager.instance.cameraRenderer.GetEyeRenderDesc(EyeSide.Left);

Vector2 resolution = leftEye.resolution;
YVRCameraRenderer.EyeFov fullFov = leftEye.fullFov;
```

## Dynamic Resolution

`YVRQualityManager` can adjust `XRSettings.renderViewportScale` based on the system recommended resolution. When enabled, the SDK changes the render scale within `minDynamicResolutionScale` and `maxDynamicResolutionScale`.

```csharp
var qualityManager = YVRManager.instance.qualityManager;
qualityManager.enableDynamicResolution = true;
qualityManager.minDynamicResolutionScale = 0.8f;
qualityManager.maxDynamicResolutionScale = 1.0f;
```

You can set the resolution adaptation policy through `powerSetting`:

```csharp
YVRManager.instance.qualityManager.powerSetting =
    YVRQualityManager.AdapterResolutionPolicy.BALANCED;
```

## Get IPD

Use `YVRPlugin.Instance.GetIPD()` to get the current device IPD.

```csharp
float ipd = YVRPlugin.Instance.GetIPD();
```

## Set Color Space

Use `YVRPlugin.Instance.SetColorSpace` to set the color space used by the application. Common values include `ColorSpaceType.Rec709`, `ColorSpaceType.Rec2020`, and `ColorSpaceType.P3`.

```csharp
YVRPlugin.Instance.SetColorSpace(ColorSpaceType.P3);
```

> [!Note]
> Color space, dynamic resolution, and display frequency affect visual quality, power, and performance. Prefer setting them during application startup or scene transitions instead of switching them frequently in the same frame.
