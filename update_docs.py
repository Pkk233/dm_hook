#!/usr/bin/env python3
"""更新 API_REFERENCE.md：移除 dm_ 前缀，修复锚点，统一调用示例"""
import re

path = r"D:\dm\dm_hook\API_REFERENCE.md"
with open(path, 'r', encoding='utf-8') as f:
    content = f.read()

# 1. 替换函数签名中的 dm_xxx → xxx
content = re.sub(r'(DM_CALL\s+)dm_', r'\1', content)
# 2. 替换标题中的 dm_xxx → xxx
content = re.sub(r'(### \d+\.\d+\s+)dm_', r'\1', content)
# 3. 替换 `dm_xxx` → `xxx`
content = re.sub(r'`dm_', r'`', content)
# 4. 替换正文中的 dm_xxx → xxx（函数名位置）
content = re.sub(r'(?<![`\w])dm_([a-z])', r'\1', content)
# 5. 修复被误改的 dm_hook.dll → dm_hook.dll
content = content.replace('hook.dll', 'dm_hook.dll')
# 6. 修复目录中的锚点
def fix_anchor(m):
    num = m.group(1)
    name = m.group(2)
    suffix = m.group(3) or ''
    anchor = num.replace('.', '') + '-' + name.lower()
    return f'<a href="#{anchor}">{num} {name}{suffix}</a>'
content = re.sub(r'<a href="#[a-z0-9]+">(\d+\.\d+) ([a-zA-Z0-9]+)(.*?)</a>', fix_anchor, content)

with open(path, 'w', encoding='utf-8') as f:
    f.write(content)
print('Done')