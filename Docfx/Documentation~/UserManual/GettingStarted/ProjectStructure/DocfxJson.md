# Docfx.json

`docfx.json`文件是 Docfx 的核心，其指明了 Docfx 生成文档所需要的数据来源，模板，配置等等。

完整的 Docfx.json 配置说明，可见[官方文档](https://dotnet.github.io/docfx/tutorial/docfx.exe_user_manual.html)。

如下为几个比较重要的字段：

## metadata

docfx 可以为 .Net 代码生成代码文档页面，但其并不是直接对 .Net 代码进行操作，而是先将 .Net 代码转换为对应的 metadata（以 `.yml` 文件表示），再用 metadata 生成文档页面。

`metadata` 字段即用来配置生成 metadata 的过程。 `metadata` 的示例如下：

```json
  "metadata": [
    {
        "src": [
        {
            "src": "../UnityProject/",
            "files": [
                "Unity.com.yvr.core.csproj"
            ]
        }
        ],
        "dest": "Apis",
        "properties": {
            "DefineConstants": "UNITY_EDITOR"
        },
        "globalNamespaceId": "Global",
        "filter": "filterConfig.yml"
    }
  ],
```

其中：

- `src` 字段表示 .Net 代码的路径， 如上的实例中， docfx 从`.csproj` 文件生成 metadata。其中的子字段 `src` 表示 `.csproj` 文件工程存在的父目录，子字段 `files` 表明 `.csproj` 相对于父目录的路径。
    - 当 `docfx.json` 即为 `.csproj` 的父目录时，可以省略**子**字段 `src`，但 `metadata` 下的 `src` 不可省略。

- `dest` 字段表示 `metadata` 存储的目录

- `globalNamespaceID` 字段指明脚本的全局 Namespace。 docfx 默认会处理所有处在命名空间下的文件，如果某代码文件并不处于任一命名空间下，可能会被 docfx 无视。因此需要加上此条，让 Docfx 同样处理全局的代码文件。

- `filter` 字段表示过滤某些代码的规则文件，规则文件以 `.yml` 文件表示。具体可见 [Filter](../../Advanced/ApiFilter.md)

- `properties` 中包含对 MSBuild 的设置项，MSBuild PropertyGroup 中的内容都可以添加在此，如上例中的`DefineConstants` 来自于[MSBuild Conditional Constructs](https://learn.microsoft.com/en-us/visualstudio/msbuild/msbuild-conditional-constructs?view=vs-2022)

## build

`build` 字段指明工程生成代码文件时依赖的文件，编译后静态网页的输出路径，编译时使用的模板等数据。 `build` 字段示例如下：

```json
"build": {
    "xref": [
        "./ExternalXref/UnityUtilities.yml"
    ],
    "content": [
    {
        "files": [
            "Apis/**.yml",
            "Apis/**/index.md"
        ]
    },
    {
        "files": [
            "UserManual/**.md",
            "UserManual/**/**/toc.yml"
        ]
    },
    {
        "files": [
            "toc.yml",
            "index.md"
        ]
    }
    ],
    "resource": [
    {
        "files": [
            "images/**",
            "UserManual/**.png",
            "UserManual/**.jpg"
        ]
    }
    ],
    "overwrite": [
    {
        "files": [
            "ApisDocs/**.md"
        ],
      }
    ],
    "dest": "_site",
    "globalMetadata": {
        "_enableSearch": true
    },
    "template": [
        "default",
        "Plugins/memberpage.2.56.6/content",
        "Plugins/Custom/",
        "Plugins/DocFx.Plugins.PlantUml/template"
    ],
    "markdownEngineProperties": {
        "plantUml.localPlantUmlPath": "../../DocFx/plantuml.jar",
        "plantUml.outputFormat": "svg",
        "plantUml.remoteUrl": "http://www.plantuml.com/plantuml/",
        "plantUml.renderingMode": "remote"
    }
}
```

其中：

- `xref` 字段表示需要[跨工程引用](../../Advanced/CrossReference.md#跨工程引用) 的 Docfx 的 API 文档内容

- `content` 字段表示所有 Docfx 生成文档时所需要的源文件，主要由 `markdown` 文件（`.md`），和 [metadata](./YAML.md#apiyaml) 组成和 [toc.yml](./YAML.md#tocyaml) 组成，。

- `content` 字段表示编译文档时所依赖的所有文本资源，其中可以由多组 `files` 构成，如上实例使用了三组 `files` 分别选取了 APIs 和 UserManual 文件夹下的内容，以及根目录下的内容。

- `resource` 字段表示为文档依赖的其他资源，如文档中需要显示的图片等。

- `overwrite` 字段表示为 [API 文档重载](../../Advanced/ApiOverwrite.md)时需要用到的 `.md` 文件路径

- `dest` 字段表示编译后生成的静态网页文件存储路径

- `globalMetadata` 字段表示文档生成时一些全局的配置。

- `template` 字段表示文档生成所使用的[模板](../../Advanced/Plugins.md)

- `markdownEngineProperties` 字段包含对 markdown 解析引擎的配置