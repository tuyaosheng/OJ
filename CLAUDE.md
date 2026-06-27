# CLAUDE.md — 项目上下文与协作指南

## 项目概述

本项目是基于青岛大学开源 OJ 系统二次开发的在线判题平台，面向教学场景（高校课堂）定制化改造。

---

## 源项目 GitHub 链接（上游原版）

| 仓库 | 地址 | 说明 |
|------|------|------|
| 后端 | https://github.com/QingdaoU/OnlineJudge | Django 后端，含判题逻辑 |
| 前端 | https://github.com/QingdaoU/OnlineJudgeFE | Vue 2.x 前端 |
| 部署 | https://github.com/QingdaoU/OnlineJudgeDeploy (branch: 2.0) | docker-compose 部署配置 |

## 本项目 GitHub 链接

```
https://github.com/tuyaosheng/OJ
```

---

## 仓库结构（Monorepo）

```
OJ/
├── OnlineJudge/        # Django 后端源码（Python）
├── OnlineJudgeFE/      # Vue 2.x 前端源码（已改造）
├── OnlineJudgeDeploy/  # Docker 部署配置
├── CLAUDE.md           # 本文件
└── README.md           # 项目说明
```

---

## 已完成的所有改动

### 一、界面汉化 & 美化

| 文件 | 改动内容 |
|------|---------|
| `src/i18n/index.js` | 默认语言从 `en-US` 改为 `zh-CN` |
| `src/pages/oj/components/NavBar.vue` | 导航栏蓝色渐变（`#1a237e→#1565c0→#0288d1`），全局样式用两个 `<style>` 块：scoped 管布局，非 scoped 管 iView 组件颜色覆盖 |
| `src/pages/oj/components/Panel.vue` | 标题左蓝边 + 浅蓝渐变背景 |
| `src/styles/common.less` | 全局背景 `#f0f2f5`，优先中文字体 |
| `src/styles/iview-custom.less` | 卡片圆角阴影、表格悬停高亮、按钮渐变 |
| `src/pages/admin/App.vue` | 管理端字体优先中文 |
| `src/pages/admin/views/Home.vue` | 管理端内容区浅灰背景、顶部白色带阴影 |
| `src/pages/admin/components/SideMenu.vue` | 侧边栏蓝色渐变，当前项右侧蓝色指示器 |
| `src/pages/admin/components/Panel.vue` | 管理端卡片圆角、阴影、标题左蓝边 |
| `src/pages/admin/components/TopNav.vue` | 面包屑"首页"汉化 |

### 二、视频题解

- **后端**：`Problem` 模型加 `video` 字段（TextField, null=True），`ProblemVideoAPI` 处理上传/删除，文件存 `/data/public/upload/videos/`，URL 格式 `/public/upload/videos/filename.mp4`，限 mp4/webm/ogg，最大 500MB。
- **管理端**：编辑题目页底部加"题解视频（选填）"区域，新建题目时提示先保存再上传；已有视频显示预览 + 删除按钮。
- **用户端**：题目有视频时右侧出现"题解视频"卡片，点击弹出 HTML5 播放器 Modal。
- **FPS 导入不受影响**（`_create_problem` 不传 video 字段，默认 null）。
- **Migration**：`problem/migrations/0015_problem_video.py`

### 三、FPS 批量导入优化

- 默认 `rule_type` 改为 `OI`（原为 ACM）
- 自动为每道题添加"C++"标签（FPS 格式无标签字段）
- **扩展 FPS 格式**（自定义标签，不破坏原有格式）：
  - `<id>T001</id>` — 自定义显示 ID（不填则自动生成 fps-xxxx，重复时追加后缀）
  - `<tag>C++</tag>` — 自定义标签，可多个
  - `<test_input score="25">` — 单个测试点分数属性
  - 不指定分数时：100 分平均分配到所有测试点
- **FPS 生成脚本**（`fps/fps_generator.py`）：把"每题一个文件夹"的目录（`config.json` + `statement.md` + `tests/*.in/.out`）一键打包成可导入的 FPS XML，免手写 XML。样例见 `fps/example_problems/`（源文件夹 + 生成结果 `fps/example_problems/example_import.xml`），说明见 `fps/README_批量加题.md`
- **导入即入章 + 导入即可见**：`POST /api/admin/import_fps` 新增两个可选表单字段——`chapter_id`（导入完自动按序挂到该章节末尾）、`visible=true`（导入后直接对学生可见，默认仍为隐藏）。不传则行为与原版一致

### 四、题目标签与 Rule Type 默认值

- 后端 `CreateOrEditProblemSerializer`：`tags` 的 `allow_empty` 改为 `True`（标签非必填）
- 前端新建题目表单：`rule_type` 默认 `OI`（两处：data() 初始值 和 `mounted()` 里的 getLanguages 回调）
- 前端移除标签必填验证

### 五、章节（知识点模块）功能

**后端新增模型**（`problem/migrations/0016_chapter.py`）：
- `Chapter`：id、title、description、order、created_by、problems(M2M through ChapterProblem)
- `ChapterProblem`：chapter、problem、order（章节内题目顺序）

**后端 API**：
- `ChapterAdminAPI`：CRUD（管理员）
- `ChapterProblemAdminAPI`：单个添加/删除/重排序
- `BatchAddChapterProblemAPI`：批量添加（`problem_ids` 数组，跳过已存在的）
- `ChapterAPI`：用户端获取所有章节及其题目

**前端管理端**：
- 新增 `ChapterList.vue`（路由 `/problem/chapters`），侧边栏"章节管理"入口
- 左栏显示已加入题目（可逐个移除）
- 右栏全题库多选表格，支持搜索、按 ID/难度/通过率排序（升序/降序），已在章节的题目禁用勾选，点击"批量加入"一次添加

**前端用户端**（`ProblemList.vue`）：
- 列表模式 / 章节模式 切换按钮
- 章节模式：每个章节可展开/折叠，显示题目列表（编号、名称、难度 Tag、通过率、做题状态图标）

### 六、上课模式（Class Mode）

**后端新增模型**（`problem/migrations/0017_class_session.py`）：
- `ClassSession`：课堂（绑定章节，start_time / end_time 控制有效期）
- `StudentActivity`：学生当前状态（session、user、ip_address、current_problem、updated_at）每个 session+user 唯一
- `IPBinding`：IP → 座位号/主机名 手动绑定表

**后端 API**：
- 管理端：`ClassSessionAdminAPI`（CRUD）、`ClassDashboardAPI`（大屏数据）、`IPBindingAdminAPI`（CRUD）
- 用户端：`ActiveClassSessionAPI`（查询当前是否有进行中课堂）、`StudentActivityAPI`（上报当前查看题目）、`ClassProgressAPI`（学生专注视图数据）

**前端管理端**：
- `ClassSession.vue`（路由 `/problem/class_session`）：课堂列表 + IP 绑定管理，含编辑/删除/打开大屏按钮
- `ClassDashboard.vue`（路由 `/class_dashboard?session_id=N`）：深色全屏大屏，学生×题目进度矩阵，自动30s刷新
  - ✓ 绿 = 已通过，— 黄 = 未通过但尝试，👁 蓝闪 = 正在查看（来自 StudentActivity），灰 = 未开始
  - 显示学生 IP 和绑定座位名，按通过数排序

**前端用户端**：
- `ClassMode.vue`（路由 `/class/:sessionId`）：学生专注视图，倒计时、每题显示"X位同学已通过"、点击题目跳转并上报
- `ProblemList.vue`：存在进行中课堂时顶部出现红色"进入课堂"按钮
- `Problem.vue`：进入题目页时自动调用 `reportActivity` 上报当前题目 ID 到服务端

### 七、用户系统改造（教师/学生）

**后端新增字段**（`account/migrations/0013_userprofile_identity_grade.py`）：
- `UserProfile.identity`：`'student'` | `'teacher'` | `''`（默认空 = 旧账号）
- `UserProfile.grade`：Integer（年级，如 1/2/3/4）
- `UserProfile.class_name`：Text（班级名称，如"计算机2301班"）

**注册逻辑变更**（只允许学生注册）：
- 移除 email 字段（不再必填）
- 新增 grade（下拉 1-8）和 class_name（文本输入）
- 注册后自动设置 `identity = 'student'`

**教师账号**：只能由超级管理员通过 `CreateTeacherAPI`（`POST /api/admin/create_teacher`）创建，`identity = 'teacher'`，不走注册页面

**批量升年级**（`BatchUpgradeGradeAPI`，`POST /api/admin/batch_upgrade_grade`）：
- 所有未禁用的学生 `grade += 1`
- 可传 `max_grade` 参数：超过该年级的学生账号自动禁用（视为毕业）

**前端管理端用户页**：
- 新增身份筛选、年级筛选下拉
- 用户表格新增身份 Tag、年级、班级列
- 顶部新增"新建教师账号"和"批量升年级"按钮

### 八、AI 代码诊断（WA 归因）

学生手动点击，对自己**未通过（非 AC）**的提交调用大模型分析错误原因；按天限次（管理员可配）；教师可在管理端查看所有诊断记录。

**后端新增配置**（`SysOptions`，复用 options 机制，超管可改）：
- `ai_diagnosis_enabled`：总开关（默认 `False`）
- `ai_daily_limit`：每人每天调用次数（默认 `5`，管理员/教师不受限）
- `ai_api_config`：`{api_base, api_key, model}`，**OpenAI 兼容**接口（DeepSeek/通义/Kimi/智谱/本地 vLLM 等通用），`api_base` 形如 `https://api.deepseek.com/v1`

**后端新增模型**（`submission/migrations/0013_aicodediagnosis.py`）：
- `AICodeDiagnosis`：`submission`(OneToOne)、`problem`、`user_id`、`username`、`submission_result`、`result`(AI 文本/Markdown)、`create_time`
- 一条提交只诊断一次并缓存，重复点击直接返回缓存、**不消耗次数**

**后端 API**：
- 用户端 `AICodeDiagnosisAPI`（`/api/ai_diagnosis`）：
  - `POST {submission_id}`：仅限**本人**的**非 AC 已判完**提交（CE/WA/TLE/MLE/RE/部分正确）；按"今日本人记录数 < `ai_daily_limit`"限流；用 `requests` 调 `/chat/completions`
  - `GET ?submission_id=`：返回本人该提交已有诊断 + 今日剩余次数
  - prompt 设计：拼题面+样例+语言+代码+判题结果+未通过测试点编号（不泄露测试数据），system 提示"指出 bug 与修改方向，可给关键提示/伪代码，但**不直接给整题完整正确代码**"
- 管理端：
  - `AIDiagnosisConfigAPI`（超管，`/api/admin/ai_diagnosis/config`）：GET/POST 配置，`api_key` 不回显（只返回 `api_key_set`），POST 时 `api_key` 留空表示不修改
  - `AIDiagnosisListAPI`（教师/admin_role，`/api/admin/ai_diagnosis/list`）：分页列出全部诊断，可按 `username` / 题目显示 ID 筛选

**前端用户端**（`Problem.vue`）：
- 判题完且结果非 AC 时，状态区出现"AI 诊断"按钮 + "今日剩余 N 次"
- 结果用 `marked` 渲染进 Modal，附"仅供参考、请独立完成"提示

**前端管理端**：
- `AIConfig.vue`（路由 `/ai/config`，侧边栏"常规 → AI 诊断配置"）：开关、每日次数、API Base、模型、API Key 表单
- `AIDiagnosisList.vue`（路由 `/ai/diagnosis`，侧边栏"题目 → AI 诊断记录"）：按用户/题目筛选、分页、弹窗查看诊断全文

---

## 构建说明

### 修改后端后重新构建

```powershell
# 必须从 monorepo 根目录构建（Dockerfile 会同时打包前端 dist）
cd E:\QingDaoOJ
docker build -f OnlineJudge/Dockerfile.custom -t oj-backend-custom:latest .

# 重启 backend 容器
cd OnlineJudgeDeploy
docker-compose up -d --no-deps oj-backend

# 如有新 migration
docker exec onlinejudgedeploy-oj-backend-1 python manage.py migrate
```

### 修改前端后重新构建

```powershell
# 第一步：构建前端（必须先 build:dll 再 build，否则 DLL 哈希不一致导致页面空白）
cd E:\QingDaoOJ\OnlineJudgeFE
$env:NODE_OPTIONS="--openssl-legacy-provider"
$env:NODE_ENV="production"
npm run build:dll
npm run build

# 第二步：重新构建 Docker 镜像（同上）
cd E:\QingDaoOJ
docker build -f OnlineJudge/Dockerfile.custom -t oj-backend-custom:latest .
cd OnlineJudgeDeploy
docker-compose up -d --no-deps oj-backend
```

> **注意**：`Dockerfile.custom` 的构建上下文必须是 `E:\QingDaoOJ`（monorepo 根目录），不能在 `OnlineJudge/` 子目录里运行 docker build。

`docker-compose.yml` 中 `oj-backend` 的 image 已改为 `oj-backend-custom:latest`。

---

## 数据迁移记录

从旧系统（`OnlineJudgeDeploy (旧系统，包括数据)`）迁移：
- PostgreSQL：60 用户、103 题目、947 条提交记录
- 测试用例：1941 个文件
- 迁移方式：`pg_dump -Fc`（二进制）→ `docker cp` → `pg_restore`，避免 Windows UTF-16 编码问题

---

## 本地开发环境

### 环境要求

| 工具 | 版本 | 用途 |
|------|------|------|
| Docker Desktop | 28.x | 运行后端服务 |
| Node.js | v24.x | 前端开发服务器 |
| Git | 任意 | 版本管理 |

### 启动后端（Docker）

```powershell
cd E:\QingDaoOJ\OnlineJudgeDeploy
docker-compose up -d
# 访问 http://127.0.0.1（必须用 IP，不能用 localhost，WSL 占用了 IPv6 的 localhost:80）
```

### 启动前端开发服务器

```powershell
cd E:\QingDaoOJ\OnlineJudgeFE
$env:NODE_ENV="development"
$env:NODE_OPTIONS="--openssl-legacy-provider"   # Node v17+ 兼容旧版 Webpack 必须加
$env:TARGET="http://localhost"                   # 指向 Docker 后端

npm run build:dll   # 首次运行
npm run dev         # 访问 http://localhost:8080
```

### 版本管理

```powershell
cd E:\QingDaoOJ
git add .
git commit -m "改动说明"
git push
```

---

## 技术栈

| 层 | 技术 |
|----|------|
| 前端 | Vue 2.x、iView 2.x、Element UI、Webpack 3、Less |
| 后端 | Python 3、Django 3.2、Django REST Framework |
| 数据库 | PostgreSQL 10 |
| 缓存 | Redis 4 |
| 判题服务 | 自研沙箱（Seccomp） |
| 部署 | Docker / docker-compose，自定义后端镜像 `oj-backend-custom` |
