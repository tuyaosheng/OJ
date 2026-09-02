# 在 Windows Server 2016 上部署 OJ 系统（Hyper-V + Linux VM 方案）

## 背景

判题服务依赖 Linux 内核的 seccomp / cgroup 沙箱机制，Windows 原生不支持，必须跑在 Linux 环境里。

目前开发机（Windows 11）上系统能跑，是因为 Docker Desktop 默认用 **WSL2 后端**——本质是在后台拉起一个真正的 Linux 内核来跑容器，只是过程被隐藏得很丝滑。但 **WSL2 需要 Windows 10 1903+ / Windows 11，或 Windows Server 2019/2022**，Windows Server 2016（内核版本 14393）不支持 WSL2。

因此在 Server 2016 上要手动重现 WSL2 后台做的事：装 Hyper-V + 建一个 Linux 虚拟机 + 在虚拟机里跑原生 Docker，把 docker-compose 项目部署在这个 VM 里。

## 第一步：确认硬件支持虚拟化

PowerShell（管理员）执行：

```powershell
Get-ComputerInfo -Property "HyperV*"
systeminfo | findstr /i "Hyper-V"
```

需要 CPU 支持 VT-x/AMD-V，且 BIOS/UEFI 里已开启。

## 第二步：启用 Hyper-V 角色

图形界面：服务器管理器 → 添加角色和功能 → 勾选"Hyper-V" → 下一步走完 → 重启。

或 PowerShell：
```powershell
Install-WindowsFeature -Name Hyper-V -IncludeManagementTools -Restart
```

重启后打开"Hyper-V 管理器"确认能看到界面。

## 第三步：建虚拟交换机（网络）

推荐用**外部交换机**，让 VM 直接拿局域网 IP，不用折腾端口转发：

Hyper-V 管理器 → 右侧"虚拟交换机管理器" → 新建"外部" → 绑定到这台服务器实际使用的物理网卡 → 应用。

（会短暂断一下网，服务器本机网络正常，不用担心）

## 第四步：新建 Linux 虚拟机

1. 下载 Ubuntu Server 22.04 LTS ISO
2. Hyper-V 管理器 → 新建 → 虚拟机：
   - **代际**：选"第 2 代"（支持 UEFI，更快）
   - **内存**：至少 8GB（要同时跑 backend / frontend / postgres / redis / judge-server）
   - **网络**：选第三步建的外部交换机
   - **硬盘**：建议 100GB+（视现有 947 条提交 + 1941 测试文件的数据量，留足增长空间）
   - **安装介质**：选刚下载的 ISO
3. 装 Ubuntu Server，正常向导走完（**记得勾选 OpenSSH server**，方便之后用 SSH/SCP 传文件）
4. 若是第 2 代 VM，安装前先在 VM 设置里关掉"安全启动"（Secure Boot），否则 Ubuntu 安装介质可能起不来

## 第五步：VM 里装 Docker

SSH 进 Ubuntu VM：

```bash
curl -fsSL https://get.docker.com | sudo sh
sudo usermod -aG docker $USER
sudo apt install -y docker-compose-plugin
```

重新登录一次让用户组生效，跑 `docker run hello-world` 验证。

## 第六步：把现有镜像/数据搬过去

不需要在 VM 里重新 build 前端（省得装 Node）。在**现有 Windows 开发机**上直接导出镜像：

```powershell
docker save oj-backend-custom:latest -o oj-backend-custom.tar
```

连同整个 `OnlineJudgeDeploy` 目录（含 docker-compose.yml、data 目录里的数据库/测试用例/上传文件）一起 scp 传到 Linux VM：

```powershell
scp oj-backend-custom.tar OnlineJudgeDeploy -r user@<VM的IP>:/home/user/
```

VM 里导入镜像：
```bash
docker load -i oj-backend-custom.tar
```

## 第七步：迁移数据库

如果 Postgres 数据随 `data` 目录一起拷过去，通常直接能用（同一大版本 Postgres、同一套挂载方式）。更保险的做法是走熟悉的 `pg_dump -Fc` / `pg_restore` 流程：先在新环境用空数据库跑起来，再 restore 进去，避免路径权限或版本差异导致的问题。

## 第八步：启动

```bash
cd OnlineJudgeDeploy
docker compose up -d
docker exec onlinejudgedeploy-oj-backend-1 python manage.py migrate
```

VM 会有自己的局域网 IP（外部交换机分配），局域网内直接用 `http://<VM的IP>` 访问，不需要在 Windows Server 上做端口转发。

## 关于稳定性

之前用 VMware 出现的"隔一段时间要点一下才能唤醒"，通常是宿主机或虚拟机的电源管理策略把机器/虚拟机/网卡挂起了（桌面机默认电源计划容易触发睡眠）。用外部交换机 + Hyper-V 之后，只要这台物理服务器本身不进入睡眠（服务器操作系统默认电源计划一般就是"从不睡眠"），VM 会一直跑，不会再出现类似问题。
