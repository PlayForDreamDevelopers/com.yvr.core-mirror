# 规范

之前部分介绍了 Docfx 本身的功能，本部分将介绍实际在 YVR 第一方项目中使用 Docfx 的推荐规范。

## 将 Docfx 工程作为 Submodule

Docfx 工程在 [Gerrit 仓库](http://192.168.9.249:8081/admin/repos/YVR/SDK/Docfx)中进行管理。

当一个其他工程需要导入模板时，需要先将 Docfx Gerrit 工程作为 Submodule，如：
```powershell
git submodule add ssh://wangxj@192.168.9.249:29418/YVR/SDK/Docfx Documentation
```

此时会自动生成 `.gitmodules` 文件，但其中的 url 路径为绝对路径。需要将其修改为相对路径。最终的 `.gitmodules` 应当如下：
```text
[submodule "Docfx"]
    path = Docfx
    url = ../Docfx
```

Docfx Submodule 将包含如下内容，即默认的 Template，memberpage 插件，最通用的过滤设置：
```text
├──DefaultTemplate
├──Plugins
├──.gitignore
├──filterConfig.yml
└──plantuml.jar
```

## 第一方仓库编写文档

第一方仓库的文档，都应当存储在 `Documentation~` 文件夹内，如下示例:
```text
D:\YTestFramework\com.yvr.test-framework\Documentation~\
├──Apis
│  ├──.manifest
│  ├──Index.md
│  └──toc.yml
├──UserManual
│  ├──assets
│  ├──GeneralTest
│  ├──GraphicsTest
│  ├──PerformanceTest
│  ├──GeneralTest.md
│  ├──GraphicsTest.md
│  ├──Introduction.md
│  ├──PerformanceTest.md
│  └──toc.yml
├──.gitignore
├──docfx.json
├──index.md
└──toc.yml
```

其中应当包含配置文件 [Docfx.json](./GettingStarted/ProjectStructure/DocfxJson.md) 和 [toc.yml](./GettingStarted/ProjectStructure/YAML.md#tocyaml) 用以控制 Docfx 的逻辑，以及各 `.md` 文档文件。

如果仓库是作为应用仓库，则 `Documentation~` 在仓库的根目录下，与工程的 `Assets` 文件夹同级。

如果仓库是作为 Package 仓库，则 `Documentation~` 在 Package 目录下，与 Package 的 `package.json` 同级。

> [!Note]
> 非特殊情况下，不需要为 Package 的 TestProjects 编写文档。因为 TestProjects 本身应当足够的简单。


### 提供编译脚本

第一方仓库需要提供编译文档的 batch 文件，以供 Jenkins 编译文档使用。

Batch 文件应当存在放在工程的根目录下，且命名为 `BuildDocument.bat`，Batch 文件示例如下：
```batch
set buildTarget= %1

echo %buildTarget%

if %buildTarget% ==build goto BuildDocfx
if %buildTarget% ==clean goto Clean
if %buildTarget% ==generate goto GenerateCSProj

:BuildDocfx
docfx .\com.yvr.test-framework\Documentation~\docfx.json --serve
goto End

:Clean
rmdir /s /q .\com.yvr.test-framework\Documentation~\obj
rmdir /s /q .\com.yvr.test-framework\Documentation~\_site
del .\com.yvr.test-framework\Documentation~\Apis\*.yml
goto End

:GenerateCSProj
"%UNITY%" -projectPath "./TestProjects/TestFramework/" -batchmode -quit -nographics -logFile -executeMethod "UnityEditor.SyncVS.SyncSolution"
goto End

:End
```

其中包含有三个编译选项，`build`，`clean` 和 `generate`，当选择这三个编译选项后，会分别跳转到 `BuildDocfx`，`Clean` 和 `GenerateCSProj` 分支：
- `build`：调用 docfx 编译文档
- `Clean`：删除 docfx 编译的缓存文件
- `generate`：生成 `csproj` 文件，实现为调用 Unity 的 `UnityEditor.SyncVS.SyncSolution` 函数触发脚本的编译。`%UNITY%` 为全局的环境变量，应当配置为指向 Unity.exe