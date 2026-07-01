# 显示与渲染配置

本页介绍与显示、分辨率、色彩空间和设备显示参数相关的常用运行时接口。若只需要修改屏幕刷新率，可参考[屏幕刷新率](./DisplayRefreshRate.md)。

## 获取显示刷新率

可以通过 `YVRManager.instance.cameraRenderer.displayFrequenciesAvailable` 获取当前设备支持的刷新率列表，并通过 `displayFrequency` 读取或设置当前刷新率。

```csharp
float[] supportedFrequencies = YVRManager.instance.cameraRenderer.displayFrequenciesAvailable;
float currentFrequency = YVRManager.instance.cameraRenderer.displayFrequency;

YVRManager.instance.cameraRenderer.displayFrequency = 90;
```

## 获取眼缓冲分辨率和视场角

`YVRManager.instance.cameraRenderer.GetEyeRenderDesc` 可获取单眼渲染描述，其中包含眼缓冲分辨率、兼容旧接口的对称 FOV，以及完整的上下左右 FOV。

```csharp
YVRCameraRenderer.EyeRenderDescription leftEye =
    YVRManager.instance.cameraRenderer.GetEyeRenderDesc(EyeSide.Left);

Vector2 resolution = leftEye.resolution;
YVRCameraRenderer.EyeFov fullFov = leftEye.fullFov;
```

## 动态分辨率

`YVRQualityManager` 支持根据系统推荐分辨率动态调整 `XRSettings.renderViewportScale`。启用后，SDK 会在 `minDynamicResolutionScale` 和 `maxDynamicResolutionScale` 范围内逐步调整渲染比例。

```csharp
var qualityManager = YVRManager.instance.qualityManager;
qualityManager.enableDynamicResolution = true;
qualityManager.minDynamicResolutionScale = 0.8f;
qualityManager.maxDynamicResolutionScale = 1.0f;
```

可通过 `powerSetting` 设置分辨率适配策略：

```csharp
YVRManager.instance.qualityManager.powerSetting =
    YVRQualityManager.AdapterResolutionPolicy.BALANCED;
```

## 获取 IPD

可以通过 `YVRPlugin.Instance.GetIPD()` 获取当前设备的 IPD 数据。

```csharp
float ipd = YVRPlugin.Instance.GetIPD();
```

## 设置色彩空间

可以通过 `YVRPlugin.Instance.SetColorSpace` 设置应用使用的色彩空间。常用值包括 `ColorSpaceType.Rec709`、`ColorSpaceType.Rec2020` 和 `ColorSpaceType.P3`。

```csharp
YVRPlugin.Instance.SetColorSpace(ColorSpaceType.P3);
```

> [!Note]
> 色彩空间、动态分辨率和刷新率都会影响画面质量、功耗和性能。建议在应用启动或场景切换等稳定时机设置，避免在同一帧内频繁切换。
