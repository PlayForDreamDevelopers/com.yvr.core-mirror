# 交叉引用

## 工程内引用

### Manual 中引用 API References

使用 xref: <uid> 的格式，如：

```markdown
[YVRInputModule](xref:YVR.Core.YVRInputModule)
```

### Manual 中引用标题

使用 markdown 的标准语法，即如果要引用文章中的某个标题，可使用 `#` 符号，如要引用 `Add Product Details` 标题，该标题定义为：

```markdown
### Add Product Details
```

则要链接到该标题的格式为：

```markdown
[Product Detail](#add-product-details).
```

## 跨工程引用

// TODO

参考：
<https://github.com/NormandErwan/UnityXrefMaps>：生成 Unity 引擎跨工程引用 XRef 的工程
<https://github.com/NormandErwan/DocFxForUnity>：创建 Unity Docfx 的教程
<https://dotnet.github.io/docfx/tutorial/links_and_cross_references.html#cross-reference-between-projects>：Docfx 官方对于跨工程引用的说明

### 增加代码覆盖率报告入口

在 [YTestFramework 工程](http://192.168.9.237:8081/YTestFramework/UserManual/CodeCoverage.html)中加入了对于生成代码覆盖率报告的支持。

代码覆盖率报告最终会生成在指定的文件夹内，且该文件夹内会包含一个 `index.htm` 文件，该文件可以直接在浏览器中打开，作为代码覆盖率报告的入口。

可以将该文件夹生成在文档的目录下，即文档目录结构如下：
```text
D:.
├───Apis
├───CodeCoverage
│   ├───CodeCoverage-opencov
│   │   └───Automated
│   ├───Report
│   │   ├───....
│   │   ├───index.htm
│   │   ├───....
│   └───Report-history
├───UserManual
│   ├───assets
|   | .....
```

此时可以将 `CodeCoverage` 加入 `docfx.json` 的 Resource 部分：
```json
// ...
        "resource": [
            {
                "files": [
                    "UserManual/**.png",
                    "UserManual/**.jpg",
                    "CodeCoverage/**"

                ]
            }
        ],
// ...
```

并在 `toc.yml` 中即能指向 `Coverage\Report` 目录下的 `index.htm` 文件：
```yaml
- name: User Manual
  href: UserManual/
  homepage: UserManual/introduction.md

- name: Api Documentation
  href: Apis/
  homepage: Apis/index.md

- name: Code Coverage
  href: CodeCoverage/Report/index.htm
```
