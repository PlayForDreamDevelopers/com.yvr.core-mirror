# Api 过滤

本部分将介绍 Docfx 对于 API 过滤的设置。

## 关闭默认过滤规则

`Docfx` 包含一系列默认的过滤规则，如不显示 `private` 的对象，如果要关闭默认过滤规则，可在 [Docfx.json 的 metadata 部分](../GettingStarted/ProjectStructure/DocfxJson.md#metadata) 中加入
```yaml
{
  "metadata": [
    {
      "src": [
        {
          "files": [ "**.csproj" ],
          "exclude": [ "**/bin/**", "**/obj/**" ],
          "src": "../src"
        }
      ],
      "dest": "obj/temp/docfxapi",
      "disableDefaultFilter": true
    }
  ],
  "build": ...
}
```

具体见[讨论](https://github.com/dotnet/docfx/issues/2561)

## 设置过滤条件

在 `metadata` 中增加 `filter` 字段，并指向设置了过滤格式的文件

```json
"filter": "filterConfig.yml",
```

如过滤所有的 `System` 和 `UnityEngine` 为前缀的信息：

```yaml
# filterConfig.yml

apiRules:
- exclude:
    uidRegex: ^System
- exclude:
    uidRegex: ^UnityEngine
```

# Reference

<https://dotnet.github.io/docfx/docs/dotnet-api-docs.html#filter-apis>