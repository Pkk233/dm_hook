#!/usr/bin/env python3
"""Remove dm_ prefix from internal function calls in dm.cpp and dm_com.cpp"""
import re, os

for fname in ['dm.cpp', 'dm_com.cpp']:
    path = os.path.join(os.path.dirname(__file__), fname)
    with open(path, 'r', encoding='utf-8') as f:
        content = f.read()

    # 替换所有以 dm_ 开头、后跟字母的函数调用
    content, n = re.subn(r'\bdm_([a-zA-Z]\w*)\b', lambda m: m.group(1), content)
    
    # 恢复被误改的 dm_hook
    content = content.replace('hook.dll', 'dm_hook.dll')
    content = content.replace('hook', 'dm_hook')  # 恢复 dm_hook
    
    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f'{fname}: {n} replacements')