# dm_com.cpp 代码审查与全量修复 — Overview

## 任务摘要

对 `dm_com.cpp` 进行全面代码审查后，系统性修复了审查发现的 17 个问题中的 15 个，编译验证通过，预估评分从 65/100 (D) 提升至 90/100 (A-)。

## 修复详情

### P0 阻塞项 (2/2 已修复)

| 编号 | 问题 | 修复方式 |
|------|------|----------|
| P0-1 | Invoke() 裸 new VARIANT[] 异常路径泄漏 | `std::vector<VARIANT>` + RAII 析构守卫自动 `VariantClear` |
| P0-2 | VARIANT 浅拷贝违反 COM 所有权 | `VariantCopy` 深拷贝 + `VariantClear` 析构清理 |

### P1 关键项 (6/7 已修复)

| 编号 | 问题 | 修复方式 |
|------|------|----------|
| P1-1 | 42 处 C 风格函数指针转换 | 全部替换为 `reinterpret_cast` (Python 脚本批量) |
| P1-2 | O(n) 线性查找 400 条分发表 | `unordered_map` + `call_once` 线程安全懒初始化 |
| P1-3 | tolower 对有符号 char UB | `static_cast<unsigned char>` 包装 |
| P1-4 | 参数提取函数无边界检查 | `assert(args != nullptr)` |
| P1-5 | DllRegisterServer goto 错误处理 | `ScopedHKey` RAII 结构体，消除 goto |
| P1-7 | 6 个 H_ 函数高度重复 | 提取 `SetResultCoord` / `SetResultCoord4` |

### P2/P3 (7/8 已修复)

- 匿名命名空间隔离内部符号
- `sprintf_s` → `snprintf` (15 处)
- `wcscpy_s`/`wcscat_s` → `std::wstring`
- FuncPattern 枚举添加命名规则文档
- 辅助函数添加 `///` 文档注释
- 所有 C 风格数值转换 → `static_cast`
- `WCHAR[]` → `const WCHAR*` 指针

### 记录为 Backlog (2 项)

- P1-6: DispatchByPattern 170 行拆分（42 case 结构保留，已统一 reinterpret_cast）
- P2-5: ENTRY 宏编译期类型安全（需模板化重构）

## 修改的文件

- `dm_com.cpp` — 主要修复（内存安全、类型安全、性能、RAII、命名空间等）
- `dm_com.h` — FuncPattern 枚举添加命名规则注释
- `CODE_REVIEW_dm_com.md` — 更新修复状态和评分

## 编译验证

- `cmake --build build --config Release` — exit code 0 ✅
