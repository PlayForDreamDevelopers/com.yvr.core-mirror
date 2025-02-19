# 固定注视点渲染

静态注视点渲染 (FFR) 以低于中心的分辨率渲染眼睛纹理的边缘，由于较低的碎片着色器要求，可以减少 GPU 负载。因此，具有高碎片着色器计算量的内容将从使用 FFR 中看到最大的性能优势。

FFR 的级别可以通过 [YVR管理程序](xref:YVR.Core.YVRManager) 进行调整，如下图所示，较高的级别将导致较低的 GPU 负载，但在眼睛渲染纹理的边缘出现模糊的结果。

![固定注视点渲染](FixedFoveatedRendering/2021-04-29-16-06-46.png)

FFR 级别可以按帧修改，这意味着你可以根据运行时间中的内容切换 FFR 级别。然而，不同 FFR 级别之间的跳跃可能是明显的，因此强烈建议在切换场景时改变 FFR 级别。

请记住FFR 带来的 GPU 性能提升是以减少眼睛渲染纹理的边缘效果为代价的。因此，在使用 FFR 时，你应该注意视觉质量和 GPU 性能之间的平衡。

> [!WARNING]
> FFR 不是解决 GPU 性能问题的长久之计。


## 启用动态注视点渲染

在 YVR 管理器设置下，勾选 Fixed Foveated Rendering Dynamic。 
    ![DynamicFFR](./FixedFoveatedRendering/DynamicFFR.png)