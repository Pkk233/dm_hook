#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
驱动 test_dm_all.exe 对每个 dm_* 函数做独立子进程调用 (隔离崩溃)。
用法: python3 run_smoke.py
输出: tests/test_smoke_results.csv  (func,result,detail)
"""
import subprocess
import sys
import os

HERE = os.path.dirname(os.path.abspath(__file__))
EXE = os.path.join(HERE, "tests", "test_dm_all.exe")
LIST = os.path.join(HERE, "tests", "smoke_funcs.txt")
CSV = os.path.join(HERE, "tests", "test_smoke_results.csv")
TIMEOUT = 8  # 秒, 防止卡死


def main():
    with open(LIST, "r", encoding="utf-8") as f:
        names = [l.strip() for l in f if l.strip()]
    n = len(names)
    print("funcs to test:", n)

    rows = []
    crash = miss = ok = other = 0
    for i, name in enumerate(names):
        try:
            r = subprocess.run([EXE, str(i)], capture_output=True,
                               timeout=TIMEOUT)
            out = r.stdout.decode("utf-8", "replace").strip()
            if out:
                # 期望 "name,RESULT,detail"
                rows.append(out)
                if out.startswith(name + ",OK"):
                    ok += 1
                elif ",MISSING," in out:
                    miss += 1
                elif ",CRASH," in out:
                    crash += 1
                else:
                    other += 1
            else:
                rows.append("%s,CRASH,no-stdout-exit%d" % (name, r.returncode))
                crash += 1
        except subprocess.TimeoutExpired:
            rows.append("%s,CRASH,timeout" % name)
            crash += 1
        except Exception as e:  # 子进程崩溃 (非零退出且无输出)
            rows.append("%s,CRASH,%s" % (name, type(e).__name__))
            crash += 1
        if (i + 1) % 50 == 0:
            print("progress %d/%d  ok=%d crash=%d miss=%d" %
                  (i + 1, n, ok, crash, miss))

    with open(CSV, "w", encoding="utf-8") as f:
        f.write("func,result,detail\n")
        for row in rows:
            f.write(row + "\n")

    print("=== DONE ===")
    print("total=%d ok=%d crash=%d missing=%d other=%d" %
          (n, ok, crash, miss, other))


if __name__ == "__main__":
    main()
