#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
FPS 题目批量生成脚本（纯标准库，无需 Django）

把一个"每题一个文件夹"的目录，转换成可直接在 OJ 管理端
「导入题目 / import_fps」上传的 FPS 1.2 XML 文件。

================ 目录约定 ================

<root>/                         # 传给脚本的根目录，下面每个子文件夹是一道题
  ch01-001-aplusb/
    config.json                 # 必填：题目元信息（见下）
    statement.md                # 必填：题面描述（支持 HTML / Markdown 文本）
    input.md                    # 选填：输入格式说明
    output.md                   # 选填：输出格式说明
    hint.md                     # 选填：提示
    samples/                    # 选填：展示给学生的样例
      1.in
      1.out
      2.in
      2.out
    tests/                      # 必填：评测测试点
      1.in
      1.out
      2.in
      2.out

config.json 字段（除 title 外都可省略，用默认值）：
{
  "id": "ch01-001",                 # 显示 ID，建议带章节前缀，便于排序/批量加章
  "title": "A + B Problem",         # 题目标题（必填）
  "tags": ["第1章 顺序结构"],        # 标签列表；想保留 C++ 筛选就再加 "C++"
  "time_limit": 1000,               # 时间限制，单位 ms，默认 1000
  "memory_limit": 256,              # 内存限制，单位 MB，默认 256
  "scores": {"1": 30, "2": 70}      # 选填：测试点 -> 分数。省略则导入时自动平分 100
}

> 说明：导入接口会读取 <id> 和 <tag>。若不指定 scores，
>   后端会把 100 分平均分配到所有测试点。

================ 用法 ================

  python fps_generator.py <root> -o problems.xml

然后到管理端「导入题目」上传 problems.xml 即可（题目默认隐藏，记得设为可见，
或用带 chapter_id/visible 参数的接口一步到位）。
"""
import argparse
import json
import os
import re
import sys

try:
    import markdown as _markdown
except ImportError:
    _markdown = None


def md_to_html(text):
    """题面 .md 转 HTML 再入 XML。

    前端 Problem.vue 用 v-html 直接渲染题面、不做 Markdown 解析，
    所以导入前必须转好 HTML，否则页面上会看到裸的 ** 和 ``` 记号。
    需要 pip install markdown；未安装则原样返回并给出警告。
    """
    if text is None or text.strip() == "":
        return text
    if _markdown is None:
        print("  [警告] 未安装 markdown 库（pip install markdown），题面将按原文导入", file=sys.stderr)
        return text
    md = _markdown.Markdown(extensions=["fenced_code", "tables", "nl2br", "sane_lists"])
    return md.convert(text)


def cdata(text):
    """把任意文本安全包进 CDATA。处理罕见的 ']]>' 串。"""
    if text is None:
        text = ""
    text = str(text)
    # CDATA 内不能出现 ']]>'，拆开重新闭合
    text = text.replace("]]>", "]]]]><![CDATA[>")
    return "<![CDATA[" + text + "]]>"


def read_file(path):
    if os.path.isfile(path):
        with open(path, "r", encoding="utf-8-sig") as f:
            return f.read()
    return None


def natural_key(name):
    """自然排序：1.in, 2.in, ..., 10.in 正确排序。"""
    return [int(t) if t.isdigit() else t.lower()
            for t in re.split(r"(\d+)", name)]


def collect_pairs(folder):
    """收集某文件夹下的 *.in / *.out 配对，按自然序返回 [(stem, in_path, out_path)]。"""
    if not os.path.isdir(folder):
        return []
    stems = {}
    for fn in os.listdir(folder):
        stem, ext = os.path.splitext(fn)
        if ext.lower() == ".in":
            stems.setdefault(stem, {})["in"] = os.path.join(folder, fn)
        elif ext.lower() == ".out":
            stems.setdefault(stem, {})["out"] = os.path.join(folder, fn)
    pairs = []
    for stem in sorted(stems, key=natural_key):
        d = stems[stem]
        if "in" in d and "out" in d:
            pairs.append((stem, d["in"], d["out"]))
        else:
            print(f"  [警告] 测试点 '{stem}' 缺少 .in 或 .out，已跳过", file=sys.stderr)
    return pairs


def build_item(problem_dir):
    """根据单个题目文件夹，生成一个 <item> 字符串。"""
    config_path = os.path.join(problem_dir, "config.json")
    if not os.path.isfile(config_path):
        print(f"  [跳过] {problem_dir} 缺少 config.json", file=sys.stderr)
        return None
    with open(config_path, "r", encoding="utf-8") as f:
        cfg = json.load(f)

    title = cfg.get("title")
    if not title:
        print(f"  [跳过] {problem_dir} 的 config.json 缺少 title", file=sys.stderr)
        return None

    statement = md_to_html(read_file(os.path.join(problem_dir, "statement.md"))) or "No Description"
    input_desc = md_to_html(read_file(os.path.join(problem_dir, "input.md"))) or ""
    output_desc = md_to_html(read_file(os.path.join(problem_dir, "output.md"))) or ""
    hint = md_to_html(read_file(os.path.join(problem_dir, "hint.md")))

    time_limit = int(cfg.get("time_limit", 1000))   # ms
    memory_limit = int(cfg.get("memory_limit", 256))  # MB
    tags = cfg.get("tags", []) or []
    scores = cfg.get("scores", {}) or {}

    lines = ["\t<item>"]
    if cfg.get("id"):
        lines.append(f"\t\t<id>{cdata(cfg['id'])}</id>")
    lines.append(f"\t\t<title>{cdata(title)}</title>")
    lines.append(f'\t\t<time_limit unit="ms">{cdata(time_limit)}</time_limit>')
    lines.append(f'\t\t<memory_limit unit="mb">{cdata(memory_limit)}</memory_limit>')
    for tag in tags:
        lines.append(f"\t\t<tag>{cdata(tag)}</tag>")
    lines.append(f"\t\t<description>{cdata(statement)}</description>")
    lines.append(f"\t\t<input>{cdata(input_desc)}</input>")
    lines.append(f"\t\t<output>{cdata(output_desc)}</output>")

    # 样例（展示给学生）
    for _, in_path, out_path in collect_pairs(os.path.join(problem_dir, "samples")):
        lines.append(f"\t\t<sample_input>{cdata(read_file(in_path))}</sample_input>")
        lines.append(f"\t\t<sample_output>{cdata(read_file(out_path))}</sample_output>")

    # 测试点（评测用）
    test_pairs = collect_pairs(os.path.join(problem_dir, "tests"))
    if not test_pairs:
        print(f"  [警告] {problem_dir} 没有 tests/ 测试点", file=sys.stderr)
    for stem, in_path, out_path in test_pairs:
        score = scores.get(stem)
        if score is not None:
            lines.append(f'\t\t<test_input score="{int(score)}">{cdata(read_file(in_path))}</test_input>')
        else:
            lines.append(f"\t\t<test_input>{cdata(read_file(in_path))}</test_input>")
        lines.append(f"\t\t<test_output>{cdata(read_file(out_path))}</test_output>")

    if hint:
        lines.append(f"\t\t<hint>{cdata(hint)}</hint>")
    lines.append("\t</item>")
    return "\n".join(lines)


def main():
    parser = argparse.ArgumentParser(description="把题目文件夹批量打包成 FPS XML")
    parser.add_argument("root", help="题目根目录（下面每个子文件夹是一道题）")
    parser.add_argument("-o", "--output", default="problems.xml", help="输出 XML 路径（默认 problems.xml）")
    args = parser.parse_args()

    if not os.path.isdir(args.root):
        print(f"错误：目录不存在 {args.root}", file=sys.stderr)
        sys.exit(1)

    sub_dirs = sorted(
        [os.path.join(args.root, d) for d in os.listdir(args.root)
         if os.path.isdir(os.path.join(args.root, d))],
        key=lambda p: natural_key(os.path.basename(p))
    )

    items = []
    for d in sub_dirs:
        print(f"处理：{os.path.basename(d)}")
        item = build_item(d)
        if item:
            items.append(item)

    if not items:
        print("没有生成任何题目，请检查目录结构。", file=sys.stderr)
        sys.exit(1)

    xml = (
        '<?xml version="1.0" encoding="UTF-8"?>\n'
        '<fps version="1.2" url="https://github.com/zhblue/freeproblemset/">\n'
        '\t<generator name="OJ-fps-generator" url="local"/>\n'
        + "\n".join(items)
        + "\n</fps>\n"
    )
    with open(args.output, "w", encoding="utf-8") as f:
        f.write(xml)
    print(f"\n完成：{len(items)} 道题 -> {args.output}")


if __name__ == "__main__":
    main()
