# dm_hook 项目记忆

## 项目概况
- 大漠插件 (dm.dll) 开源重新实现，支持 COM 接口调用
- 核心文件: dm.cpp (3366行), dm_com.cpp (1339行), dllmain.cpp, dm.h, dm_com.h
- 编译: CMake + MSVC, 需 GDI+, 可选 Tesseract/ONNX Runtime

## 代码审查标准
- 文档: CODE_REVIEW_STANDARDS.md (v3.0, 2026-08-13)
- 7维度评分: 内存安全(22%) + 类型安全(13%) + 安全性(18%) + 性能(13%) + 可维护性(14%) + 现代C++(8%) + COM安全(12%)
- P0阻塞合并, P1超3个阻塞合并, P2/P3不阻塞
- 已知问题: 25项 (P0×5, P1×11, P2×9)

## 已知技术债务
- dm.cpp: sprintf无长度限制(19处), malloc/free管理, 3366行需拆分
- dm_com.cpp: 已修复 (std::vector替代裸new VARIANT[]、VariantCopy替代浅拷贝、reinterpret_cast替换C风格转换、unordered_map替代O(n)查找、匿名命名空间包裹、snprintf、ScopedHKey等)；仅剩 backlog: P1-6 (DispatchByPattern 42-case 拆分)、P2-5 (ENTRY 宏编译期类型安全)
- 全局: 无命名空间, 桩函数无标记, 缺文档注释
- COM 分发表: 417/417 DISPID 全覆盖 (2026-08-13 补齐 40 个缺失条目, API_REFERENCE.md v1.4)
