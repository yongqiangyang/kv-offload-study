# KV Offload 源码研究包

围绕 vLLM Ascend DSA/MLA Decode Offload，分析 DDR 共享内存、KV 写回、Copy-SFA 稀疏读取、P/D 传输及迁移 Mooncake 的策略。

## 阅读入口

- [DSA Decode Offload 迁移 Mooncake 策略](docs/DSA-Decode-Offload迁移Mooncake策略.md)
- [Ascend–MemFabric 接口分析](docs/Ascend-MemFabric接口分析.md)
- [版本、来源与快照范围](manifest.json)
- [50 个源码符号索引](source-index.json)

## 代码快照

| 目录 | 内容 |
|---|---|
| [repos/vllm](repos/vllm) | vLLM |
| [repos/vllm-ascend](repos/vllm-ascend) | vLLM Ascend |
| [repos/Mooncake](repos/Mooncake) | Mooncake |
| [repos/memfabric_hybrid](repos/memfabric_hybrid) | MemFabric 官方 GitCode 代码 |
| [repos/memfabric_hybrid-github-mirror](repos/memfabric_hybrid-github-mirror) | 旧 GitHub 镜像，用于说明版本差异 |
| [repos/asc-devkit](repos/asc-devkit) | AscendC 已检出的稀疏源码子集，包含 DataCopyPad 接口与架构实现 |
| [releases/memfabric-1.3.0-python](releases/memfabric-1.3.0-python) | MemFabric 1.3.0 发布包提取的 Python/内核参考文件 |

快照整理于 2026-11-04。保留源码，不包含上游 Git 历史和未初始化子模块；asc-devkit 仅包含研究时检出的文件。构建和后续升级请使用 manifest 中的官方仓库及提交。各组件 HEAD 不是经过验证的部署组合。

本研究仅做静态源码核对，未在 NPU/CANN 环境构建或验证性能。原始 MemFabric wheel 未安装或执行；本公开仓库不重复上传 wheel 二进制，下载 URL 和 SHA256 保留在 manifest.json。

## 校验源码索引

```bash
python3 build_source_index.py
```

分析文档使用仓库相对链接和 GitHub 行号锚点。源码更新后应重新审阅结论及行号。

## 来源与许可证

上游源码与发布包提取文件的版权和许可证归原作者所有，保留在对应目录中。本仓库用于源码研究与接口分析，不代表任何上游项目的官方兼容性或性能承诺。详见 [THIRD_PARTY.md](THIRD_PARTY.md)。
