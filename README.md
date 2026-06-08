# 在线判题平台（教学定制版）

基于 [青岛大学 OnlineJudge](https://github.com/QingdaoU/OnlineJudge) 开源项目二次开发，针对高校编程课堂教学场景深度定制。

---

## 功能特性

### 基础功能（继承原版）
- 支持 C、C++、Java、Python2、Python3 多语言评测
- ACM / OI 赛制，实时排名
- 管理员后台创建题目、比赛、公告
- 题目支持 Markdown + LaTeX 数学公式
- 题目标签、难度分级、随机挑题

### 教学定制功能

| 功能 | 说明 |
|------|------|
| **全中文界面** | 默认中文，无需切换 |
| **管理端美化** | 蓝色渐变侧边栏，现代卡片风格 |
| **视频题解** | 管理员上传题解视频（mp4/webm/ogg），学生题目页弹窗播放 |
| **FPS 导入增强** | 支持在 XML 中指定题目 ID、标签、测试点分数，导入后自动设 OI 模式 |
| **章节模块** | 按知识点组织题目；用户端支持列表/章节两种视图切换 |
| **上课模式** | 基于章节创建限时课堂，学生进入专注视图；管理员大屏实时查看全班进度 |
| **用户分类** | 区分教师/学生；学生注册填年级班级；批量升年级（一键+1） |

---

## 上课模式详解

### 管理员端
1. **课堂管理**（后台 → 题目 → 课堂管理）：绑定章节、设置开始/结束时间
2. **大屏看板**：点击课堂的"大屏看板"按钮，新窗口打开适合投影的深色界面
   - 学生 × 题目 进度矩阵：✓ 已通过 | — 尝试中 | 👁 正在查看 | 灰色 未开始
   - 显示学生 IP 及手动绑定的座位号
   - 每 30 秒自动刷新
3. **IP 座位绑定**：手动录入 IP → 座位号对应关系（如 192.168.1.10 → A01排3座）

### 学生端
- 课堂时间内，题目列表顶部出现"进入课堂"按钮
- 进入后看到本节课章节所有题目、倒计时、每题的通过人数
- 打开题目页面时自动上报到大屏（教师可见谁在看哪道题）

---

## 项目结构

```
OJ/
├── OnlineJudge/        # Django 后端（Python 3）
├── OnlineJudgeFE/      # Vue 2.x 前端（已深度改造）
├── OnlineJudgeDeploy/  # Docker 部署配置
├── CLAUDE.md           # 开发上下文与详细改动记录
└── README.md           # 本文件
```

---

## 快速部署

### 环境要求

- Docker Desktop（Windows 需开启 WSL2 / Linux 原生）
- Git

### 启动步骤

```bash
# 克隆项目
git clone https://github.com/tuyaosheng/OJ.git
cd OJ/OnlineJudgeDeploy

# 启动所有服务（首次需拉取镜像，约 5-15 分钟）
docker-compose up -d
```

| 地址 | 说明 |
|------|------|
| http://localhost | 用户前台 |
| http://localhost/admin | 管理后台 |

默认超级管理员：`root` / `rootroot`（**请立即修改**）

---

## 本地前端开发

```powershell
cd OnlineJudgeFE

# 安装依赖（仅首次）
npm install --legacy-peer-deps

# 构建 DLL（仅首次）
$env:NODE_ENV="development"
$env:NODE_OPTIONS="--openssl-legacy-provider"
npm run build:dll

# 启动热重载开发服务器
$env:TARGET="http://localhost"
npm run dev
# 访问 http://localhost:8080
```

---

## 修改后端后的重新构建

```powershell
# 构建自定义镜像
cd OnlineJudge
docker build -f Dockerfile.custom -t oj-backend-custom:latest .

# 重启并执行迁移
cd ..\OnlineJudgeDeploy
docker-compose up -d --no-deps oj-backend
docker exec onlinejudgedeploy-oj-backend-1 python manage.py migrate
```

---

## 数据库 Migration 列表

| App | Migration | 内容 |
|-----|-----------|------|
| problem | 0015 | Problem 加 video 字段 |
| problem | 0016 | Chapter / ChapterProblem 章节模型 |
| problem | 0017 | ClassSession / StudentActivity / IPBinding 上课模式 |
| account | 0013 | UserProfile 加 identity / grade / class_name |

---

## 上游项目

| 仓库 | 地址 |
|------|------|
| 原版后端 | https://github.com/QingdaoU/OnlineJudge |
| 原版前端 | https://github.com/QingdaoU/OnlineJudgeFE |
| 原版部署 | https://github.com/QingdaoU/OnlineJudgeDeploy |

---

## License

基于上游项目 MIT License 进行二次开发。
