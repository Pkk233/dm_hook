#!/usr/bin/env python3
"""把 sim 参数从 long 改为 double（支持浮点相似度 0.0-1.0）"""
import re

# 只处理 DM_API 函数行
for fname in ['dm.h', 'dm.cpp']:
    path = r"D:\dm\dm_hook" + "\\" + fname
    with open(path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    count = 0
    changed_lines = []
    for line in lines:
        if 'DM_API' in line:
            # 替换参数列表中的 ", long sim," 或 ", long sim)"
            if re.search(r', long sim[,)]', line):
                line = re.sub(r', long sim(?=[,)])', ', double sim', line)
                count += 1
        changed_lines.append(line)
    
    with open(path, 'w', encoding='utf-8') as f:
        f.writelines(changed_lines)
    print(f'{fname}: {count} 处 long sim → double sim')