# Screen Refresh Rate

The screen refresh rate represents the number of times the screen refreshes per second. Typically, the screen refresh rate is the same as the maximum frame rate supported by the application. For example, when the screen refresh rate is 72 Hz, the maximum frame rate of the application is 72 FPS. The higher the screen refresh rate, the faster the screen refreshes, and the better the smoothness and stability of the picture. However, a high screen refresh rate can also affect device performance. When the application cannot reach the maximum refresh rate of the device, it may cause frame drops, stuttering, tearing, and latency issues.

By default, we encourage applications to set the screen refresh rate to 90 Hz to provide a better user experience. Only set the screen refresh rate to 72 Hz under special circumstances, such as:
- Insufficient performance or excessive power consumption: You can use the [real-time monitoring tool](https://developer.pfdm.cn/yvrdoc/unity_CN/UserManual_CN/MetricsTool.html) to monitor application performance and ensure that the application can handle a high screen refresh rate. For more details, refer to performance monitoring and analysis.
- When playing videos: If the video frame rate does not match the screen refresh rate, it may cause visual discontinuity and stuttering. For example, if the video is 24 Hz and the screen refresh rate is set to 90 Hz. Because the 24 Hz video frame rate cannot be evenly distributed at the 90 Hz refresh rate, each frame's display time on the screen will be different, resulting in visual stuttering. Therefore, it is recommended to switch to an appropriate screen refresh rate when playing videos.

## Setting the Screen Refresh Rate

Set the screen refresh rate through `YVRManager.instance.cameraRenderer.displayFrequency` as shown below:

```csharp
YVRManager.instance.cameraRenderer.displayFrequency = 90;
```

You can set the screen refresh rate at the application startup or dynamically adjust the screen refresh rate as needed during the application runtime. If you do not set the screen refresh rate, the default screen refresh rate is 90 Hz.

For more display and rendering settings, see [Display and Rendering Configuration](./DisplayRenderingConfig.md).
