# CLAUDE.md — 项目上下文与协作指南

## 项目概述

本项目是基于青岛大学开源 OJ 系统二次开发的在线判题平台，面向教学场景定制化改造。

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

## 已完成的改动

### 前端（OnlineJudgeFE）

| 文件 | 改动内容 |
|------|---------|
| `src/i18n/index.js` | 默认语言从 `en-US` 改为 `zh-CN`，全平台中文显示 |
| `src/pages/oj/components/NavBar.vue` | 导航栏改为蓝色渐变主题（`#1a237e → #1565c0 → #0288d1`），白色文字，蓝色下划线高亮当前页 |
| `src/pages/oj/components/Panel.vue` | 面板标题加左侧蓝色边框，浅蓝渐变背景 |
| `src/styles/common.less` | 全局背景色从 `#eee` 改为 `#f0f2f5`，字体优先中文字体 |
| `src/styles/iview-custom.less` | 卡片圆角阴影、表格悬停高亮、按钮渐变、分页器配色等全面优化 |
| `src/pages/oj/App.vue` | 页脚样式优化，加分割线 |
| `src/pages/oj/views/general/Home.vue` | 首页竞赛卡片样式优化 |
| `src/pages/oj/views/problem/Problem.vue` | 题目详情页标题样式、示例代码块、信息栏优化 |
| `src/pages/oj/views/problem/ProblemList.vue` | 标签按钮圆角、随机选题按钮渐变蓝色 |
| `src/pages/oj/views/user/Login.vue` | 登录按钮圆角，链接颜色统一 |

### 部署（OnlineJudgeDeploy）

| 文件 | 改动内容 |
|------|---------|
| `docker-compose.yml` | `version` 字段已保留（有警告但无影响） |

### 数据迁移

- 从旧系统（`OnlineJudgeDeploy (旧系统，包括数据)`）迁移了：
  - PostgreSQL 数据：60 用户、103 题目、947 条提交记录
  - 测试用例文件：1941 个
  - 上传文件：9 个
- 迁移方式：`pg_dump -Fc`（二进制格式）→ `docker cp` → `pg_restore`，避免 Windows 编码问题

---

## 待完成的功能

### 视频题解功能（开发中）

**需求：** root 管理员在创建/编辑题目时可上传题解视频，所有用户在题目详情页可观看。

**改动范围：**

- **后端 `OnlineJudge/`**
  - `problem/models.py`：`Problem` 模型增加 `video_url` 字段
  - `problem/views/admin.py`：增加视频文件上传接口
  - `problem/serializers.py`：序列化器包含 `video_url`
  - 新增 migration 文件
  - 需要重新构建后端 Docker 镜像

- **管理前端 `OnlineJudgeFE/src/pages/admin/views/problem/Problem.vue`**
  - 创建/编辑题目表单中增加"上传题解视频"控件

- **用户前端 `OnlineJudgeFE/src/pages/oj/views/problem/Problem.vue`**
  - 题目详情页增加"查看题解视频"入口，点击弹出视频播放器

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
# 访问 http://localhost （后端 API + 内置前端）
```

### 启动前端开发服务器

```powershell
cd E:\QingDaoOJ\OnlineJudgeFE
$env:NODE_ENV="development"
$env:NODE_OPTIONS="--openssl-legacy-provider"  # Node v17+ 兼容旧版webpack必须加
$env:TARGET="http://localhost"                  # 指向Docker后端

# 首次运行需先构建DLL
npm run build:dll

# 启动热重载开发服务器
npm run dev
# 访问 http://localhost:8080
```

### 注意事项

- Node v24 + Webpack 3 需要 `--openssl-legacy-provider` 才能运行
- 前端 dev server 运行在 8080 端口，API 请求代理到 80 端口（Docker 后端）
- Docker 镜像拉取需配置国内镜像源（已在 `~/.docker/daemon.json` 配置 163/百度/中科大）
- 修改后端代码需重新构建 Docker 镜像（当前尚未实施，后续视频功能开发时需要）

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
| 后端 | Python 3、Django、Django REST Framework |
| 数据库 | PostgreSQL 10 |
| 缓存 | Redis 4 |
| 判题服务 | 自研沙箱（Seccomp） |
| 部署 | Docker / docker-compose |
