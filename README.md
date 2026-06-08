# 在线判题平台（青岛OJ 定制版）

基于 [青岛大学 OnlineJudge](https://github.com/QingdaoU/OnlineJudge) 开源项目二次开发，针对教学场景进行定制化改造。

---

## 功能特性

- 支持 C、C++、Java、Python2、Python3 多语言评测
- ACM / OI 赛制，实时排名
- 管理员后台创建题目、比赛、公告
- 题目支持 Markdown + LaTeX 数学公式
- 题目标签、难度分级、随机挑题
- 用户注册、登录、个人主页、提交历史
- **全中文界面**（已从英文默认切换为中文）
- **视频题解功能**（开发中）：管理员上传题解视频，用户在题目页观看

---

## 项目结构

```
OJ/
├── OnlineJudge/        # Django 后端（Python 3）
├── OnlineJudgeFE/      # Vue 2.x 前端（已汉化 + 美化）
├── OnlineJudgeDeploy/  # Docker 部署配置
├── CLAUDE.md           # 开发上下文与协作记录
└── README.md           # 本文件
```

---

## 快速部署

### 环境要求

- Docker Desktop（Windows / Linux）
- Git

### 启动步骤

```bash
# 1. 克隆项目
git clone https://github.com/tuyaosheng/OJ.git
cd OJ

# 2. 启动所有服务
cd OnlineJudgeDeploy
docker-compose up -d
```

等待 1-3 分钟（首次需拉取镜像），完成后访问：

| 地址 | 说明 |
|------|------|
| http://localhost | 用户前台 |
| http://localhost/admin | 管理后台 |

默认管理员账号：`root` / `rootroot`（**请立即修改密码**）

> **Windows 用户**：需要安装 Docker Desktop 并开启 WSL2。生产环境建议使用 Linux。

---

## 本地前端开发

如需修改前端代码并实时预览：

```powershell
cd OnlineJudgeFE

# 安装依赖（仅首次）
npm install --legacy-peer-deps

# 构建DLL（仅首次或依赖变更后）
$env:NODE_ENV="development"
$env:NODE_OPTIONS="--openssl-legacy-provider"
npm run build:dll

# 启动开发服务器（热重载）
$env:TARGET="http://localhost"
npm run dev
```

访问 http://localhost:8080 查看效果，修改代码后浏览器自动刷新。

---

## 改动说明

相较于原版上游项目，本版本主要改动：

### 界面改造

| 改动 | 详情 |
|------|------|
| 默认中文 | 全平台切换为简体中文显示 |
| 导航栏重设计 | 蓝色渐变主题（深蓝→蓝→天蓝），更专业的视觉风格 |
| 卡片组件优化 | 圆角、阴影、悬停动效，视觉层次更清晰 |
| 表格优化 | 去除边框、添加悬停高亮、表头加粗 |
| 全局色彩统一 | 主色调 `#1565c0`，背景 `#f0f2f5` |
| 按钮渐变 | Primary / Warning 按钮改为渐变风格 |
| 面板标题 | 添加左侧蓝色边框 + 浅蓝渐变背景 |

### 待开发功能

- [ ] 视频题解：管理员上传视频 → 用户题目页播放
  - 后端：`Problem` 模型增加 `video_url` 字段，新增上传接口
  - 前端管理端：题目编辑页增加视频上传控件
  - 前端用户端：题目详情页增加视频播放入口

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
