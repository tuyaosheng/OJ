<template>
  <div class="dashboard-wrap">
    <!-- 顶部信息栏 -->
    <div class="dash-header">
      <div class="dash-title">
        <i class="el-icon-monitor"></i>
        <span>{{ session.title || '课堂进度看板' }}</span>
        <el-tag v-if="session.is_active" type="success" size="small" effect="dark" style="margin-left:12px;">进行中</el-tag>
      </div>
      <div class="dash-meta">
        <span v-if="session.chapter_title">章节：{{ session.chapter_title }}</span>
        <span style="margin-left:20px;">共 {{ students.length }} 位学生 · {{ problems.length }} 道题</span>
        <span style="margin-left:20px;color:#a0cfff;">{{ lastRefresh }}</span>
        <el-button size="mini" type="primary" plain style="margin-left:16px;" @click="load">
          <i class="el-icon-refresh"></i> 刷新
        </el-button>
        <el-switch v-model="autoRefresh" active-text="自动刷新" inactive-text=""
                   style="margin-left:16px;" @change="toggleAuto"></el-switch>
      </div>
    </div>

    <!-- 进度矩阵 -->
    <div class="dash-body" v-loading="loading">
      <div v-if="students.length === 0 && !loading" class="empty">
        暂无学生进入课堂模式
      </div>
      <div v-else class="matrix-wrap">
        <table class="matrix">
          <thead>
            <tr>
              <th class="th-student">学生</th>
              <th class="th-student">IP / 座位</th>
              <th v-for="p in problems" :key="p.id" class="th-problem">
                <div class="problem-id">{{ p._id }}</div>
                <div class="problem-title">{{ p.title }}</div>
              </th>
              <th class="th-sum">得分</th>
            </tr>
          </thead>
          <tbody>
            <tr v-for="s in sortedStudents" :key="s.user_id" :class="{'row-active': isRecentlyActive(s)}">
              <td class="td-student">{{ s.username }}</td>
              <td class="td-ip">
                <div class="ip-text">{{ s.ip || '—' }}</div>
                <div v-if="s.binding && s.binding.hostname" class="seat-text">{{ s.binding.hostname }}</div>
              </td>
              <td v-for="p in problems" :key="p.id" class="td-cell"
                  :class="cellClass(s, p)">
                <el-tooltip :content="cellTooltip(s, p)" placement="top" :open-delay="200">
                  <div class="cell-inner">
                    <i v-if="s.solved[p.id] === 0" class="el-icon-check"></i>
                    <i v-else-if="s.solved[p.id] === 1" class="el-icon-minus"></i>
                    <i v-else-if="s.current_problem_id === p.id" class="el-icon-view viewing-icon"></i>
                    <span v-else class="cell-dash">—</span>
                  </div>
                </el-tooltip>
              </td>
              <td class="td-sum">{{ countSolved(s) }} / {{ problems.length }}</td>
            </tr>
          </tbody>
        </table>
      </div>
    </div>

    <!-- 图例 -->
    <div class="dash-legend">
      <span class="legend-item solved"><i class="el-icon-check"></i> 已通过</span>
      <span class="legend-item tried"><i class="el-icon-minus"></i> 尝试中（未通过）</span>
      <span class="legend-item viewing"><i class="el-icon-view"></i> 正在查看</span>
      <span class="legend-item none">— 未开始</span>
    </div>
  </div>
</template>

<script>
  import api from '@admin/api'

  export default {
    name: 'ClassDashboard',
    data () {
      return {
        sessionId: null,
        session: {},
        problems: [],
        students: [],
        loading: false,
        autoRefresh: true,
        timer: null,
        lastRefresh: ''
      }
    },
    computed: {
      sortedStudents () {
        return [...this.students].sort((a, b) => {
          const sa = this.countSolved(a)
          const sb = this.countSolved(b)
          return sb - sa
        })
      }
    },
    mounted () {
      this.sessionId = new URLSearchParams(window.location.search).get('session_id')
      if (this.sessionId) {
        this.load()
        this.toggleAuto(true)
      }
    },
    beforeDestroy () {
      clearInterval(this.timer)
    },
    methods: {
      load () {
        if (!this.sessionId) return
        this.loading = true
        api.getClassDashboard(this.sessionId).then(r => {
          const d = r.data.data
          this.session = d.session
          this.problems = d.problems
          this.students = d.students
          this.lastRefresh = '最后刷新：' + new Date().toLocaleTimeString('zh-CN')
          this.loading = false
        }).catch(() => { this.loading = false })
      },
      toggleAuto (val) {
        clearInterval(this.timer)
        if (val) {
          this.timer = setInterval(() => this.load(), 30000)
        }
      },
      cellClass (student, problem) {
        const pid = problem.id
        if (student.solved[pid] === 0) return 'cell-solved'
        if (student.solved[pid] === 1) return 'cell-tried'
        if (student.current_problem_id === pid) return 'cell-viewing'
        return 'cell-none'
      },
      cellTooltip (student, problem) {
        const pid = problem.id
        if (student.solved[pid] === 0) return `${student.username} 已通过`
        if (student.solved[pid] === 1) return `${student.username} 正在尝试（未通过）`
        if (student.current_problem_id === pid) return `${student.username} 正在查看此题`
        return `${student.username} 未开始`
      },
      countSolved (s) {
        return Object.values(s.solved || {}).filter(v => v === 0).length
      },
      isRecentlyActive (s) {
        if (!s.updated_at) return false
        return (Date.now() - new Date(s.updated_at).getTime()) < 120000
      }
    }
  }
</script>

<style scoped lang="less">
  .dashboard-wrap {
    min-height: 100vh;
    background: #0d1b2a;
    color: #e0e8f0;
    font-family: "PingFang SC","Microsoft YaHei",sans-serif;
    display: flex;
    flex-direction: column;
  }

  .dash-header {
    padding: 16px 28px;
    background: #162436;
    border-bottom: 1px solid #1e3a56;
    display: flex;
    justify-content: space-between;
    align-items: center;
    flex-shrink: 0;
  }

  .dash-title {
    font-size: 22px;
    font-weight: 700;
    color: #40c4ff;
    i { margin-right: 10px; }
  }

  .dash-meta {
    font-size: 13px;
    color: #8aa8c8;
    display: flex;
    align-items: center;
  }

  .dash-body {
    flex: 1;
    overflow: auto;
    padding: 20px 24px;
  }

  .empty {
    text-align: center;
    padding: 80px;
    color: #4a6a8a;
    font-size: 16px;
  }

  .matrix-wrap {
    overflow: auto;
  }

  .matrix {
    border-collapse: collapse;
    width: 100%;
    font-size: 13px;

    th, td {
      border: 1px solid #1e3a56;
      padding: 0;
      white-space: nowrap;
    }

    thead th {
      background: #162436;
      color: #8aa8c8;
      font-weight: 600;
      text-align: center;
      padding: 8px 10px;
      position: sticky;
      top: 0;
      z-index: 2;
    }

    .th-student { min-width: 90px; text-align: left; padding-left: 12px; }
    .th-problem {
      min-width: 80px;
      max-width: 100px;
      .problem-id { color: #40c4ff; font-size: 12px; }
      .problem-title { color: #8aa8c8; font-size: 11px; margin-top: 2px; }
    }
    .th-sum { min-width: 60px; }

    .td-student {
      padding: 10px 12px;
      color: #c8dcf0;
      font-weight: 500;
      background: #162436;
      position: sticky;
      left: 0;
      z-index: 1;
    }

    .td-ip {
      padding: 6px 10px;
      background: #162436;
      position: sticky;
      left: 90px;
      z-index: 1;
      .ip-text { font-size: 11px; color: #6a8aaa; font-family: monospace; }
      .seat-text { font-size: 12px; color: #40c4ff; margin-top: 2px; font-weight: 600; }
    }

    .td-cell {
      text-align: center;
      vertical-align: middle;
      width: 80px;
      height: 48px;
      cursor: default;
      transition: background 0.2s;
    }

    .cell-inner {
      display: flex;
      align-items: center;
      justify-content: center;
      height: 48px;
      font-size: 18px;
    }

    .cell-solved { background: rgba(103, 194, 58, 0.25); i { color: #67c23a; font-weight: bold; } }
    .cell-tried  { background: rgba(230, 162, 60, 0.2);  i { color: #e6a23c; } }
    .cell-viewing { background: rgba(64, 196, 255, 0.15); }
    .cell-none   { background: transparent; }
    .viewing-icon { color: #40c4ff; animation: pulse 1.5s infinite; }
    .cell-dash { color: #2a4060; font-size: 14px; }

    .td-sum {
      text-align: center;
      padding: 10px;
      font-weight: 600;
      color: #40c4ff;
      background: #162436;
    }

    .row-active { td.td-student { color: #40c4ff; } }
  }

  @keyframes pulse {
    0%, 100% { opacity: 1; }
    50% { opacity: 0.4; }
  }

  .dash-legend {
    padding: 12px 28px;
    background: #162436;
    border-top: 1px solid #1e3a56;
    display: flex;
    gap: 24px;
    font-size: 13px;
    flex-shrink: 0;

    .legend-item {
      display: flex;
      align-items: center;
      gap: 6px;
      i { font-size: 16px; }
    }
    .solved { color: #67c23a; }
    .tried  { color: #e6a23c; }
    .viewing { color: #40c4ff; }
    .none   { color: #4a6a8a; }
  }
</style>
