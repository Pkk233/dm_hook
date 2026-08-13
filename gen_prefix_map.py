import re

with open('dm.h', 'r', encoding='utf-8') as f:
    txt = f.read()

# 提取声明式 API 函数: DM_CALL dm_xxx(
decls = re.findall(r'DM_CALL\s+(dm_[A-Za-z0-9_]+)\s*\(', txt)
names = sorted(set(decls))

print('unique DM_CALL dm_* functions:', len(names))

lines = []
lines.append('# dm_ 前缀函数名对照表')
lines.append('')
lines.append('> 自动从 `dm.h` 提取全部 `DM_CALL dm_*` 导出函数，逐一移除 `dm_` 前缀。')
lines.append('> 共 %d 个唯一函数。' % len(names))
lines.append('')
lines.append('| # | 原函数名 (dm_*) | 去前缀后 |')
lines.append('|---|---|---|')
for i, n in enumerate(names, 1):
    stripped = n[3:] if n.startswith('dm_') else n
    lines.append('| %d | `%s` | `%s` |' % (i, n, stripped))

with open('dm_func_prefix_map.md', 'w', encoding='utf-8') as f:
    f.write('\n'.join(lines) + '\n')
print('written dm_func_prefix_map.md')

# 校验
nondm = [n for n in names if not n.startswith('dm_')]
print('names NOT starting with dm_:', nondm if nondm else 'NONE')
stripped = [n[3:] for n in names]
dups = [s for s in set(stripped) if stripped.count(s) > 1]
print('duplicate stripped names:', dups if dups else 'NONE')
empty = [n for n in names if len(n) <= 3]
print('empty/too-short names:', empty if empty else 'NONE')
