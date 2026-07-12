#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
把「每题一个文件夹」里的 tests/ 测试点数据，分别打包成独立 zip。

- 每题一个 zip，命名 = 题目标题 + 测试点数据.zip（标题里的 Windows 非法字符 \\ / : * ? " < > | 会被替换成下划线）
- zip 内是**扁平**的 1.in / 1.out / 2.in / 2.out ...（可直接在管理端「上传测试用例」使用）
- 只打包 tests/（真正评测用的测试点）；samples/ 是展示样例、已在题面里，不入包
- zip 默认放在该题文件夹内，也可用 -o 指定统一收集目录（保持 章/题 结构）

用法：
  cd E:\\QingDaoOJ\\OnlineJudge\\fps
  # 打包某一章（zip 放各题文件夹内）
  python pack_tests.py "E:/QingDaoOJ/题库/语言C++/ch05-数组"
  # 打包整个部分，zip 统一收集到一个目录
  python pack_tests.py "E:/QingDaoOJ/题库/语言C++" -o "E:/QingDaoOJ/题库/测试点压缩包/语言C++"
"""
import argparse
import glob
import json
import os
import re
import sys
import zipfile

INVALID = r'[\\/:*?"<>|]'          # Windows 文件名非法字符


def safe_name(title):
    name = re.sub(INVALID, "_", title).strip()
    return name or "problem"


def find_problem_dirs(root):
    """root 本身是题目文件夹则返回自己，否则递归找所有含 config.json 的文件夹。"""
    if os.path.isfile(os.path.join(root, "config.json")):
        return [root]
    dirs = []
    for cfg in glob.glob(os.path.join(root, "**", "config.json"), recursive=True):
        dirs.append(os.path.dirname(cfg))
    return sorted(dirs)


def pack_one(problem_dir, out_dir):
    tests_dir = os.path.join(problem_dir, "tests")
    ins = sorted(glob.glob(os.path.join(tests_dir, "*.in")))
    if not ins:
        print(f"  [跳过] 无 tests/ 测试点: {problem_dir}", file=sys.stderr)
        return None
    try:
        cfg = json.load(open(os.path.join(problem_dir, "config.json"), encoding="utf-8"))
        title = cfg.get("title") or os.path.basename(problem_dir)
    except Exception:
        title = os.path.basename(problem_dir)

    zip_name = f"{safe_name(title)}_测试点数据.zip"
    dest_dir = out_dir if out_dir else problem_dir
    os.makedirs(dest_dir, exist_ok=True)
    zip_path = os.path.join(dest_dir, zip_name)

    count = 0
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as z:
        for inf in ins:
            stem = os.path.splitext(os.path.basename(inf))[0]
            outf = os.path.join(tests_dir, stem + ".out")
            if not os.path.exists(outf):
                print(f"  [警告] {inf} 缺少同名 .out", file=sys.stderr)
                continue
            z.write(inf, os.path.basename(inf))     # 扁平写入：1.in
            z.write(outf, os.path.basename(outf))   # 扁平写入：1.out
            count += 1
    print(f"  {zip_name}  ({count} 个测试点)")
    return zip_path


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("root", help="题目文件夹 / 章文件夹 / 部分文件夹")
    ap.add_argument("-o", "--output", default=None,
                    help="统一收集 zip 的目录（保持 章/题 层级）；省略则放各题文件夹内")
    args = ap.parse_args()

    problem_dirs = find_problem_dirs(args.root)
    if not problem_dirs:
        print("未找到任何题目（含 config.json 的文件夹）", file=sys.stderr)
        sys.exit(1)

    root_abs = os.path.abspath(args.root)
    made = 0
    for pdir in problem_dirs:
        out_dir = None
        if args.output:
            # 在收集目录下保留相对层级（去掉最后一层题目文件夹名，zip 名已含题目）
            rel = os.path.relpath(os.path.dirname(os.path.abspath(pdir)), os.path.dirname(root_abs))
            out_dir = os.path.join(args.output, rel)
        if pack_one(pdir, out_dir):
            made += 1
    print(f"\n完成：{made} 个测试点压缩包")


if __name__ == "__main__":
    main()
