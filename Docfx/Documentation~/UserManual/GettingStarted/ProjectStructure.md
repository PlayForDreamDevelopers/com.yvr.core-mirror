# Docfx 工程结构

本部分将介绍 Docfx 工程的结构，以帮助更好的理解 Docfx 工程中每一部分的用处。

[默认生成的 Docfx 工程](./ConfigureEnvironment.md#初始化-docfx-工程文件夹)目录结构如下：
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

可以看出Docfx 工程主要通过 `docfx.json`，一系列`.md` 和 `.yml` 文件以及各文件夹构成：
- [docfx.json](./ProjectStructure/DocfxJson.md)：作为整个 Docfx 工程的总管理文件，工程中存在的各文件夹意义实际都由此文件定义
- `.md` 文件：Markdown 文件作为文档的自定义内容
- [.yml](./ProjectStructure/YAML.md)：包含用以管理文档的结构的 `toc.yml` 和表示 api metadata 的各 YAML 文件