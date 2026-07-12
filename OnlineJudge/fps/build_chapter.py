#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
用 judge 容器里的 g++ 编译每题 solution.cpp，跑遍 samples/*.in 和 tests/*.in 生成 .out。

为什么这么绕：
- judge 容器根文件系统只读，/tmp 与 /dev/shm 是 noexec 的 tmpfs，docker cp 也写不进 tmpfs。
- 但 /judger 是 E:\\QingDaoOJ\\OnlineJudgeDeploy\\data\\judge_server\\run 的 9p 绑定挂载（可读写、可执行）。
  于是：主机侧把素材拷进这个 run 目录，容器侧直接在 /judger 里编译。
- 9p 的 I/O 很慢，会把大数据点的耗时测得偏高；所以真正跑评测时把输入拷到 /tmp（tmpfs）上跑，
  再把 .out 拷回 /judger，主机侧取回。二进制放 /judger（tmpfs 不可执行）。

用法：
  python build_chapter.py "E:/QingDaoOJ/题库/基础算法/ch11-高精度计算"
  python build_chapter.py <章节目录> --only ch11-003     # 只重建某一题
"""
import argparse
import glob
import json
import os
import shutil
import subprocess
import sys

CONTAINER = "onlinejudgedeploy-oj-judge-1"
STAGE_HOST = r"E:\QingDaoOJ\OnlineJudgeDeploy\data\judge_server\run\ojbuild"
STAGE_CT = "/judger/ojbuild"


def dexec(script):
    return subprocess.run(["docker", "exec", CONTAINER, "bash", "-c", script],
                          capture_output=True, text=True, encoding="utf-8", errors="replace")


def build_one(pdir):
    name = os.path.basename(pdir)
    cfg = json.load(open(os.path.join(pdir, "config.json"), encoding="utf-8"))
    tl = int(cfg.get("time_limit", 1000))

    stage = os.path.join(STAGE_HOST, name)
    shutil.rmtree(stage, ignore_errors=True)
    os.makedirs(os.path.join(stage, "tests"), exist_ok=True)
    os.makedirs(os.path.join(stage, "samples"), exist_ok=True)
    shutil.copy(os.path.join(pdir, "solution.cpp"), os.path.join(stage, "solution.cpp"))
    for sub in ("tests", "samples"):
        for inf in glob.glob(os.path.join(pdir, sub, "*.in")):
            shutil.copy(inf, os.path.join(stage, sub, os.path.basename(inf)))

    ct = f"{STAGE_CT}/{name}"
    r = dexec(f"cd {ct} && g++ -O2 -std=c++17 -o sol solution.cpp 2>&1")
    if r.returncode != 0:
        return name, False, f"编译失败:\n{r.stdout}{r.stderr}", 0, "", tl

    # 输入放 tmpfs 上跑，避免 9p 拖慢计时；二进制在 /judger 上执行
    script = f"""
set -e
W=/tmp/w/{name}
rm -rf $W && mkdir -p $W/tests $W/samples
cp {ct}/tests/*.in $W/tests/ 2>/dev/null || true
cp {ct}/samples/*.in $W/samples/ 2>/dev/null || true
cd $W
for f in tests/*.in samples/*.in; do
  [ -e "$f" ] || continue
  s=$(date +%s%N)
  {ct}/sol < "$f" > "${{f%.in}}.out" || {{ echo "RUNFAIL $f"; exit 1; }}
  e=$(date +%s%N)
  echo "TIME $f $(( (e-s)/1000000 ))"
done
cp $W/tests/*.out {ct}/tests/ 2>/dev/null || true
cp $W/samples/*.out {ct}/samples/ 2>/dev/null || true
rm -rf $W
"""
    r = dexec(script)
    if r.returncode != 0 or "RUNFAIL" in r.stdout:
        return name, False, f"运行失败:\n{r.stdout}{r.stderr}", 0, "", tl

    worst, worst_f = 0, ""
    for line in r.stdout.splitlines():
        if line.startswith("TIME "):
            _, f, ms = line.split()
            if int(ms) > worst:
                worst, worst_f = int(ms), f

    for sub in ("tests", "samples"):
        for outf in glob.glob(os.path.join(stage, sub, "*.out")):
            shutil.copy(outf, os.path.join(pdir, sub, os.path.basename(outf)))

    shutil.rmtree(stage, ignore_errors=True)
    return name, True, "", worst, worst_f, tl


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("chapter")
    ap.add_argument("--only", default=None, help="只处理 id 含该串的题目文件夹")
    args = ap.parse_args()

    probs = sorted(os.path.dirname(c) for c in
                   glob.glob(os.path.join(os.path.abspath(args.chapter), "*", "config.json")))
    if args.only:
        probs = [p for p in probs if args.only in os.path.basename(p)]
    if not probs:
        print("没有找到题目（含 config.json 的子文件夹）")
        sys.exit(1)

    os.makedirs(STAGE_HOST, exist_ok=True)
    ok = fail = 0
    for pdir in probs:
        name, good, err, worst, wf, tl = build_one(pdir)
        if good:
            flag = "   ⚠ 耗时超过时限1/3，考虑放宽 time_limit" if worst * 3 > tl else ""
            print(f"[OK]   {name}  最慢 {worst}ms ({wf})  限 {tl}ms{flag}")
            ok += 1
        else:
            print(f"[FAIL] {name}  {err}")
            fail += 1

    shutil.rmtree(STAGE_HOST, ignore_errors=True)
    print(f"\n完成：{ok} 成功 / {fail} 失败")
    sys.exit(1 if fail else 0)


if __name__ == "__main__":
    main()
