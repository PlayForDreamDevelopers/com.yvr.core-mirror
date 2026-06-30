# 环境配置

## 安装 DocFx

通过 `Chocolatey` 安装 ，执行语句 `choco install docfx -y`。

> [!Note]
> 目前 Docfx 使用版本为 2.56.6，可以在安装时指定 `Docfx` 的版本，即使用如下命令：
> 
> `choco install --yes docfx --version 2.56.6 --force`

Docfx 依赖 `MSBuild` 15.0 及以上的版本。2017 版本或以上的 Visual Studio 安装时将自动包含满足需求的 MSBuild。也可以通过 Chocolatey 单独安装 `MSBuild` ，如下所示：

```
choco install visualstudio2019buildtools --yes
```

> [!Caution]
> 若 MSBuild 未正确安装，会导致 API 文档生成失败，具体见 [FAQ](../FAQ.md#为何无法生成-api-metadata)

## 初始化 DocFx 工程文件夹

可使用如下语句初始化 Docfx 工程文件夹，执行后会生成 `docfx_project` 文件夹，此文件夹即为 Docfx 工程：

```powershell
docfx init -q
```

## 编译 Docfx 工程

当 Docfx 工程生成后，可使用如下命令生成静态网页并自动部署：

```powershell
docfx docfx_project\docfx.json --serve
```

生成后的网页默认会在 `_site` 文件夹下，且默认部署在地址 [http://localhost:8080](http://localhost:8080/)

上述命令中：
- `docfx_project\docfx.json` 用来说明编译文档的配置文件。可以省略 json 文件地址，直接使用 `docfx --serve` ，此时将自动找寻当前目录下的 `docfx.json` 文件作为配置
- `—-serve` 表示编译后需要自动部署，默认部署到本地 `8080` 端口。可以通过 `-p <port>` 配置部署的端口

> [!Important]
> `DocFx` 并不能像如 `MKDocs`，`Hugo` 一样在源文件修改后自动修改生成的页面。
> 
> 因此每次改动后，需要重新编译文档。及关键文件的配置