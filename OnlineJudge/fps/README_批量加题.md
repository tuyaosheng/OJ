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
