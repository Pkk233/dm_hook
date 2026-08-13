#!/usr/bin/env python3
"""批量移除函数名中的 dm_ 前缀"""
import re
import os

files = ['dm.h', 'dm.cpp', 'dm_com.cpp']

for fname in files:
    path = os.path.join(os.path.dirname(__file__), fname)
    with open(path, 'r', encoding='utf-8') as f:
        content = f.read()
    
    count = 0
    # 替换 DM_CALL dm_ → DM_CALL  (函数声明/定义中)
    content, n = re.subn(r'(DM_CALL\s+)dm_', r'\1', content)
    count += n
    # 替换 ENTRY_xxx(dm_ → ENTRY_xxx(  (分发表中)
    content, n = re.subn(r'(ENTRY_\w+\()dm_', r'\1', content)
    count += n
    # 替换 (void*)dm_ → (void*)  (分发表 funcPtr 中)
    content, n = re.subn(r'\(void\*\)dm_', r'(void*)', content)
    count += n
    # 替换 dm_setComState 这种内部函数名
    content, n = re.subn(r'\bdm_setComState\b', r'setComState', content)
    count += n
    # 替换 DISPID_dm → DISPID_dm (不要改! 跳过)
    
    with open(path, 'w', encoding='utf-8') as f:
        f.write(content)
    print(f'{fname}: {count} replacements')

print("\nDone! Now need to update dm_hook.py and dm_hook.ahk")