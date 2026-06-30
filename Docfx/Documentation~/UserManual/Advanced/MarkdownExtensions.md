# Markdown 拓展

## Alerts

`Alerts` 是 Docfx 设计用来高亮文本的拓展

在 `.md` 文件中，如果要增加 Alerts，可使用如下格式：

```markdown
> [!NOTE]
> This is a note which needs your attention, but it's not super important.
```

渲染结果如下：

![](./assets/MarkdownExtensions/Untitled%203.png)

支持的类型有 `Note`,  `TIP`, `WARNING`, `IMPORTANT`, `CAUTION`

> [!Note]
> 更多 DocFx 对于 markdown 的拓展，可见[官方文档](https://dotnet.github.io/docfx/docs/markdown.html?tabs=linux%2Cdotnet)