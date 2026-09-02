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
- `ai_allowed_results`：开放诊断的判题状态码列表（默认 `[-1, 1, 2, 3, 4, 8]`，**默认不含编译错误 -2**，避免在语法错误上浪费 token）。未在列表中的状态：前端不显示按钮、后端 POST 直接拒绝

**后端新增模型**（`submission/migrations/0013_aicodediagnosis.py`）：
- `AICodeDiagnosis`：`submission`(OneToOne)、`problem`、`user_id`、`username`、`submission_result`、`result`(AI 文本/Markdown)、`create_time`
- 一条提交只诊断一次并缓存，重复点击直接返回缓存、**不消耗次数**

**后端 API**：
- 用户端 `AICodeDiagnosisAPI`（`/api/ai_diagnosis`）：
  - `POST {submission_id}`：仅限**本人**（管理员可对任意提交）的**非 AC 已判完**提交（CE/WA/TLE/MLE/RE/部分正确）；且结果须在 `ai_allowed_results` 白名单内；按"今日本人记录数 < `ai_daily_limit`"限流（管理员不限）；已诊断走缓存不扣次数；用 `requests` 调 `/chat/completions`
  - `GET ?submission_id=`：返回该提交已有诊断（**本人或管理员**可读，与 POST 缓存逻辑一致）、今日剩余次数、总开关、`allowed_results` 白名单
  - prompt 设计：拼题面+样例+语言+代码+判题结果+未通过测试点编号（不泄露测试数据），system 提示"指出 bug 与修改方向，可给关键提示/伪代码，但**不直接给整题完整正确代码**"
  - ⚠️ **GET/POST 查询缓存须一致**：两者都按 `submission_id` 找缓存并以"本人或管理员"鉴权；早期 GET 误加 `user_id=request.user.id` 过滤，导致教师查看学生提交时预加载不到已有诊断（按钮停留"未诊断"态），已修正
- 管理端：
  - `AIDiagnosisConfigAPI`（超管，`/api/admin/ai_diagnosis/config`）：GET/POST 配置（含 `allowed_results`），`api_key` 不回显（只返回 `api_key_set`），POST 时 `api_key` 留空表示不修改
  - `AIDiagnosisListAPI`（教师/admin_role，`/api/admin/ai_diagnosis/list`）：分页列出全部诊断，可按 `username` / 题目显示 ID 筛选

**前端用户端**（两处入口）：
- `Problem.vue`：做题页判题完且结果非 AC、且总开关开启、且状态在白名单内时，状态区即时出现"AI 诊断"按钮 + "今日剩余 N 次"
- `SubmissionDetails.vue`（`/status/:id` 提交详情页，**主入口**）：打开即调 GET 预加载状态——未诊断显示蓝色"AI 诊断"，**已诊断显示绿色"查看已生成的 AI 诊断"**（点击直接看缓存、不扣次数），适用于任意历史提交
- 结果均用 `marked` 渲染进 Modal，附"仅供参考、请独立完成"提示

**前端管理端**：
- `AIConfig.vue`（路由 `/ai/config`，侧边栏"常规 → AI 诊断配置"）：开关、每日次数、**开放诊断的判题状态多选**（6 组：编译错误/答案错误/运行超时/内存超限/运行错误/部分正确，前端按 key 分组、保存时展开成结果码）、API Base、模型、API Key 表单
- `AIDiagnosisList.vue`（路由 `/ai/diagnosis`，侧边栏"题目 → AI 诊断记录"）：按用户/题目筛选、分页、弹窗查看诊断全文

### 九、教程章节初始化 + 章节序列化器 Bug 修复

**教程章节批量初始化**（按《教程ppt课件(第五版)》目录，每个 PPT 一章）：
- 共建 **30 个章节**，分三部分，用 `chapter.order` 排序，`title` 前缀 `【C++语言】/【基础算法】/【数据结构】` 以在扁平列表中区分归属：
  - 第一部分 语言C++（C++版，9 章）：初识C++/简单程序设计/选择结构/循环结构/数组/函数和递归/文件和结构体/实用技巧与模版库/指针
  - 第二部分 基础算法（单套，12 章）：概述/高精度/数据排序/递推/递归/搜索回溯/贪心/分治/广搜/DP基础/DP背包/DP经典
  - 第三部分 数据结构（9 章）：栈/队列/树及二叉树/堆/图论×2/并查集/最小生成树/拓扑排序
- **65 道题按题面考点（非标题）分类挂入**，覆盖 13 个章节（其余为空架子，待后续加题）。题库存在大量同名重复题，**每个逻辑题只挂提交数最多的那一份**，重复副本保留在题库、不入章节
- 操作方式：直接 SQL 批量 `INSERT chapter / chapter_problem`（单事务），脚本见 `backups/` 同期备份。**改库前用 `pg_dump -Fc` 备份到 `E:\QingDaoOJ\backups\`**

**章节序列化器 Bug 修复**（`problem/serializers.py` → `ChapterProblemBriefSerializer`）：
- 症状：用户端「章节模式」/ `GET /api/chapters` 返回 `server-error`（500），日志 `ImproperlyConfigured: Field name 'my_status' is not valid for model 'Problem'`
- 根因：`my_status` 只写进了 `Meta.fields`，却既非模型字段也未声明为序列化字段；旧代码在 `to_representation` 里手动补 `my_status`，但 DRF 在字段构建阶段（早于 `to_representation`）就会校验失败。**章节为空时嵌套题目序列化器从不执行，Bug 不暴露；一旦章节挂了题就必触发**
- 修复：把 `my_status` 声明为 `SerializerMethodField`（`get_my_status` 返回 `None`，再由 `ChapterAPI.get` 按用户做题记录覆盖真实状态），移除 `to_representation` 手补逻辑

### 十、做题页提交体验优化（默认 C++ + 提交后跳转状态页）

**默认语言改为 C++**（`OnlineJudgeFE/src/pages/oj/views/problem/Problem.vue` → `init()`）：
- 原逻辑 `this.language = this.problem.languages[0]`——`languages` 经 `.sort()` 后首个是 `C`（`C` < `C++`），导致默认选中 C 而非 C++
- 改为：题目允许 C++ 时默认选中 `C++`，否则回退到第一个语言
- 本地草稿优先级不变：`beforeRouteEnter` 恢复的草稿语言仍优先于此默认值

**提交后跳转到提交状态详情页**（`Problem.vue` → `submitFunc`）：
- 提交成功后不再在做题页内联轮询显示状态，而是 `this.$router.push({name: 'submission-details', params: {id: submissionId}})` 跳到 `/status/:id`（可看整体结果 + 每个测试点通过情况）
- 仅在 `detailsVisible` 分支（普通题目 / 有实时权限的 OI 竞赛）跳转；无实时权限的 OI 竞赛仍保留"提交成功"弹窗、不跳转（学生本就看不到状态）

**提交详情页加入轮询**（`OnlineJudgeFE/src/pages/oj/views/submission/SubmissionDetails.vue`）：
- 原页面只在 `mounted` 请求一次、无刷新，跳转过去会卡在"评测中"
- `getSubmission` 成功后若 `result ∈ {6 Pending, 7 Judging, 9 Submitting}` 则 `setTimeout(this.getSubmission, 2000)` 继续轮询；加 `beforeDestroy` 清理定时器（`this.refreshStatus`，与 `Problem.vue` 一致，不放进 data）

### 十一、章节模式视图状态记忆（返回不丢位置）

**症状**：题库页在「章节模式」下点进某题，做完点返回时，页面回到「列表模式」第一页，而非原来的章节位置。

**根因**：`ProblemList.vue` 的章节视图状态（`viewMode` / 展开的章节 `expandedChapters` / 滚动位置）全是纯组件内存状态，未反映到 URL。离开题库页组件即销毁，返回时重新 `mounted`，`viewMode` 复位为默认的 `'list'`。列表模式因分页在 URL query 里反而不受影响。

**修复**（`OnlineJudgeFE/src/pages/oj/views/problem/ProblemList.vue`，纯前端）：用 `sessionStorage`（key `problemListViewState`）持久化视图状态：
- **保存时机**（`saveViewState`）：点击章节内题目跳转前（`goToProblem`，同时记录 `window.scrollY`）、展开/折叠章节（`toggleChapter`）、切换列表/章节模式（`switchListView` / `switchChapterView`）
- **恢复时机**（`restoreViewState`，`mounted` 里 `init` 之前）：若上次停在章节模式则切回章节模式 → 拉章节 → 用 `pendingExpanded` 恢复展开集合 → `$nextTick` 后 `window.scrollTo` 回到原滚动位置（放 nextTick 确保题目 DOM 高度已渲染，否则滚不到位）
- 章节加载抽出公共方法 `loadChapters(restore)`：`restore=true` 时用记忆的展开集合，否则沿用「默认展开第一个章节」
- 取舍：用 `sessionStorage` 而非 `localStorage`——状态仅在当前标签页会话内有效，关标签页重开回到默认列表模式；恢复逻辑只对 `viewMode==='chapter'` 生效，列表模式行为完全不变

### 十二、题库建设：基础算法第 11~15 章（30 题）

按《题库章节与题目建议.md》的交付规范，为基础算法第 11~15 章各出 6 道题，素材放 `题库/基础算法/chXX-.../`，导入 XML 在 `题库/导入用XML/ch11~ch15.xml`。

| 章 | 目录 | 章节 ID | 题目 |
|----|------|--------|------|
| 11 高精度计算 | `ch11-高精度计算/` | 12 | 加法/减法/乘单精度/乘法/阶乘之和/数楼梯 |
| 12 数据排序 | `ch12-数据排序/` | 13 | 选择排序/快速排序/二分查找/众数/A-B数对/逆序对 |
| 13 递推算法 | `ch13-递推算法/` | 14 | 爬楼梯/骨牌铺方格/数字三角形/错位排列/卡特兰数/传球游戏 |
| 14 递归算法 | `ch14-递归算法/` | 15 | 递归斐波那契/进制转换/快速幂/全排列/汉诺塔/自然数拆分 |
| 15 搜索与回溯 | `ch15-搜索与回溯算法/` | 16 | 组合输出/选数/迷宫路径计数/细胞个数/N皇后/单词方阵 |

每题交付：`config.json` + `statement/input/output/hint.md` + `solution.cpp`（带注释）+ `samples/`(2 组) + `tests/`(6 个测试点) + 中文讲解稿 + `<标题>_测试点数据.zip`。

**教学设计上的刻意安排**：第 14 章「全排列」建立**回溯三步曲**（选择→递归→撤销），第 15 章用「迷宫路径计数」（`vis`="在当前路径上" → **必须撤销**）与「细胞个数」（`vis`="曾被访问过" → **绝不撤销**）构成**正反对照**，让学生理解"要不要撤销取决于标记的语义"，而非死背模板。

**测试点正确性保证（重要）**：所有 `.out` 均由 `solution.cpp` 在 judge 容器（g++ 13.2，与真实评测环境一致）实际跑出，**并另用 Python 独立实现算法重算一遍做交叉验证**——只跑标程只能保证"自洽"，独立重算才能发现"标程本身写错了"。经典值已核对：N 皇后 2/92/14200、C(20,10)=184756、6×6 自避路径 1262816、F(90)=2880067194370816120、30 的拆分数 5603。

**新增工具** `OnlineJudge/fps/build_chapter.py`：批量编译标程 + 生成全部 `.out` + 报告最慢测试点耗时。
> 关键坑（已写进 `fps/README_批量加题.md`）：judge 容器根文件系统**只读**，`/tmp`、`/dev/shm` 是 **`noexec` 的 tmpfs**（`docker cp` 也写不进去）；但 `/judger` 是 `OnlineJudgeDeploy/data/judge_server/run` 的**可读写绑定挂载**——脚本借道它编译执行。又因 9p 文件系统 I/O 慢会虚高耗时，评测时把输入拷到 `/tmp` 跑、二进制留在 `/judger`。

**时限**：默认 1000ms；ch12-002~006（10 万级数据）放宽到 3000ms，ch13-005（O(n²) 卡特兰 n=5000）、ch15-005（N 皇后 n=12）放宽到 2000ms。

**已导入**：30 题已全部导入、可见并挂章（DB `_id` = `ch11-001` ~ `ch15-006`）。**改库前先 `pg_dump -Fc` 备份到 `E:\QingDaoOJ\backups\`**。

### 十三、题面 HTML 化 + 例题/练习题体系 + 编码规范（2026-07）

**题面 HTML 化（重要坑）**：前端 `Problem.vue` 用 `v-html` 直接渲染题面、**不解析 Markdown**。早期 ch01~ch15 共 91 题以 Markdown 原文入库，页面上显示裸的 `**`/``` 记号、段落挤在一起。已修复：
- 91 题的描述/输入/输出/提示四字段已批量转成 HTML 回写数据库（转换**必须从磁盘 .md 源文件出发**，库里的文本被 HTML 转义过，直接转会把代码段二次转义）
- `fps_generator.py` 已内置 md→HTML 转换（需 `pip install markdown`），**以后生成的 XML 天然是 HTML 题面**，新章节不会再踩这个坑
- 汉诺塔（ch14-005）题面配了 4 张示意图，图片部署在 `OnlineJudgeDeploy/data/backend/public/upload/hanoi/`（该目录不进 git，源文件在题目文件夹 `images/` 下），URL `/public/upload/hanoi/*.png`

**例题/练习题体系（新交付规范，详见《题库章节与题目建议.md》）**：
- 每章题目分**例题**（课堂讲授，标题前缀`【例N】`）与**练习题**（变式巩固，前缀`【练N】`），`config.json` 加 `"role": "example"|"practice"`
- **题量不固定 6 道**，以覆盖本章常见题型+变式为准；各章《章节讲稿.md》的 ☆ 清单即补题清单
- **每章至少 1 道练习题带"知名游戏背景"**（如汉诺塔=《原神》木偶齿轮谜题）以提升兴趣；游戏只是外壳、考点不变，选题对照《题库章节与题目建议.md》的「游戏背景选题参考表」
- 每章**第一道例题**目录放整章《章节讲稿.md》（知识点讲授稿），每题另有 `<题目标题>.md` 题目讲解稿（文件名不带前缀）
- ch01~ch15 共 91 题已全部完成划分（config.json + DB 标题 + XML 三处同步），15 份章节讲稿就位

**游戏背景练习题批次（2026-07-24 已交付，共 15 题，每章 1 道）**：
- 显示 ID 均为 `chXX-007`（ch02 因原有 7 题，新题为 `ch02-008`），选题同时命中"游戏外壳 + 该章此前缺失的经典题型"，完整清单见《题库章节与题目建议.md》"游戏背景练习题交付清单"
- 交付流程：Python 脚本批量生成素材（config/四个md/GBK版solution.cpp/samples/tests输入）→ `build_chapter.py` 在 judge 容器编译标程生成 `.out` → 另写 15 个独立 Python 参考实现逐字节交叉验证（15/15 通过）→ `pack_tests.py` 打包 → **Django shell 脚本**复刻 `FPSProblemImport._create_problem` 逻辑批量导入并挂章（绕开管理端网页，直接在 backend 容器内用 `python manage.py shell` 执行）→ 15 个章节 XML 重新生成
- 导入前 `pg_dump -Fc` 备份到 `backups/backup_20260724_before_15_new_problems.dump`
- **重要发现**：`chapter_problem` 表里部分章节（如 ch03~ch06 对应的 chapter_id 3~6）同时挂着"教程初始化"批次的历史遗留题目（CLAUDE.md 十节记录的 65 题）和新版 `chXX-*` 系列——两批共存是正常现象，新题按 `order = 现有题数` 接在新版序列末尾，不影响旧批次

**cpp 编码规范（重要坑）**：教学机 Dev-C++ 只认 **ANSI(GBK)**，UTF-8 带 BOM 也乱码。全部 91 个 `solution.cpp` 已转 GBK。
- **不能因此让评测数据变 GBK**：标程含中文输出（如汉诺塔），测试点 `.out` 是 UTF-8。`build_chapter.py` 编译统一带 `-finput-charset=GBK -fexec-charset=UTF-8`——源码按 GBK 读、字符串字面量按 UTF-8 编进二进制，输出与测试数据一致（已在 judge 容器验证逐字节相同）
- 脚本写 cpp 用 `encoding="gbk"`；`.md`/`.json`/XML 等其余文件仍是 UTF-8

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
