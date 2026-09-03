# 批量加题 & 分章节 使用说明

本目录提供两样东西，配合管理端的「章节管理」可以高效地批量录题并按章节组织。

## 1. FPS 生成脚本：`fps_generator.py`

把"每题一个文件夹"的目录，一键打包成可导入的 FPS XML。

### 目录约定

```
example_problems/                 # 根目录，下面每个子文件夹是一道题
  ch01-001-aplusb/
    config.json                   # 必填：元信息
    statement.md                  # 必填：题面
    input.md                      # 选填：输入格式
    output.md                     # 选填：输出格式
    hint.md                       # 选填：提示
    samples/  1.in 1.out ...      # 选填：展示给学生的样例
    tests/    1.in 1.out 2.in ... # 必填：评测测试点
```

`config.json`：

```json
{
  "id": "ch01-001",                 // 显示 ID，建议带章节前缀，便于排序 + 批量加章
  "title": "A + B 问题",            // 必填
  "tags": ["第1章 顺序结构", "C++"],// 标签，建议带一个"章节标签"
  "time_limit": 1000,               // ms，默认 1000
  "memory_limit": 256,              // MB，默认 256
  "scores": {"1": 40, "2": 60}      // 选填，测试点->分数；省略则导入时自动平分 100
}
```

### 用法

```powershell
cd E:\QingDaoOJ\OnlineJudge\fps
python fps_generator.py example_problems -o example_problems/example_import.xml
```

生成的 XML 即可直接在管理端「导入题目」上传。
（现成样例：源文件夹 `example_problems/`，生成结果 `example_problems/example_import.xml`，可照着改。）

## 1.5 标程编译 & 测试点生成：`build_chapter.py`

出完题后，用它**批量编译每题的参考代码（题目文件夹下唯一的 `.cpp`，命名约定 `<题目标题>_参考代码.cpp`），跑遍所有 `.in` 生成对应 `.out`**——保证测试点答案是标程**实际跑出来**的，而不是手写臆测的。

```powershell
cd E:\QingDaoOJ\OnlineJudge\fps
python build_chapter.py "E:/QingDaoOJ/题库/基础算法/ch11-高精度计算"
python build_chapter.py "<章节目录>" --only ch11-003    # 只重建某一题
```

输出每题的**最慢测试点耗时**，并在耗时超过时限 1/3 时给出 ⚠ 提示，方便决定要不要放宽 `time_limit`。

**它为什么要绕道 judge 容器？**（这些坑踩过一次就够了）

- 宿主机（Windows）没有 g++，而 `onlinejudgedeploy-oj-judge-1` 容器里有 g++ 13.2，且**与真实评测环境一致**。
- 该容器根文件系统**只读**，`/tmp` 与 `/dev/shm` 是 **`noexec` 的 tmpfs**（编译产物无法执行），`docker cp` 也**写不进 tmpfs**。
- 但 `/judger` 是 `OnlineJudgeDeploy/data/judge_server/run` 的**可读写绑定挂载** —— 脚本把素材拷进这个宿主目录，容器侧直接在 `/judger` 里编译执行。
- 9p 文件系统 I/O 很慢，会让大数据点的耗时**测得偏高**；所以真正跑评测时把输入拷到 `/tmp`（tmpfs）上跑，二进制放 `/judger`（tmpfs 不可执行），兼顾**准确计时**与**可执行**。

> **强烈建议**：生成 `.out` 后，**再用另一种语言（如 Python）独立实现一遍算法交叉验证**。
> 只跑标程只能保证"自洽"，独立重算才能发现"标程本身就是错的"。第 11~15 章的 30 道题均已如此验证。

## 2. 导入接口增强：导入即入章 + 导入即可见

`POST /api/admin/import_fps`（multipart 表单）现额外支持两个**可选**字段：

| 字段 | 说明 |
|------|------|
| `file` | FPS XML 文件（必填，原有） |
| `chapter_id` | 填章节 ID，导入完的题目自动按顺序挂到该章节末尾 |
| `visible` | `true` 则导入后直接对学生可见（默认 `false`，保持"先隐藏"的原行为） |

> 不传这两个字段时，行为与原来完全一致（题目隐藏、不入章）。

返回示例：`{"import_count": 2, "chapter_id": 3, "visible": true}`

### 推荐工作流

1. 先在管理端「章节管理」建好章节，记下它的 `id`。
2. 用 `fps_generator.py` 生成本章的 XML（每题打上同一个章节标签 + `chXX-` 前缀 ID）。
3. 调 `import_fps` 时带上 `chapter_id` 和 `visible=true` —— 一步完成"导入→可见→入章"。

> 若仍走管理端默认的「导入题目」按钮（不带这两个参数），导入后记得手动把题目设为可见，
> 再去「章节管理」用搜索（按章节标签或 ID 前缀）+「批量加入」挂到章节。
