# 各文件夹含义

[默认生成的 Docfx 工程](../ConfigureEnvironment.md#初始化-docfx-工程文件夹)目录结构如下：
```text
D:\docfx_project
├──api
│  ├──.gitignore
│  ├──index.md
│  └──toc.yml
├──apidoc
├──articles
│  ├──intro.md
│  └──toc.yml
├──images
├──src
├──.gitignore
├──docfx.json
├──index.md
└──toc.yml
```

本部分将说明其中各文件夹的含义：

## api

`api` 文件夹包含代码文档页面的 metadata。

> [!Note]
> 该文件夹的具体路径和名称可由 [Docfx.json 中 metadata 字段](DocfxJson.md#metadata) 控制。

在含有 .net 源码的工程编译后，一系列表示 `.yml` 的 metadata 文件将保存在在该目录中。文件夹内的 `toc.yml` 同样由 Docfx 自动生成，用以管理各 api metadata 的结构。

> [!Note]
> 因为该文件夹内的所有 `.yml` 文件都由 docfx 自动生成，所以通常该文件夹内会包含 `.gitignore` 文件以过滤所有 `.yml` 文件。

## articles

包含一系列 `.md` 文档文件，具体名称和路径由 [Docfx.json 中 build 字段](DocfxJson.md#build) 控制。

其中的 `.md` 文件展示关系由文件夹内的 `toc.yml` 文件控制。