<template>
  <div class="class-mode">
    <!-- 课堂头部 -->
    <div class="class-header">
      <div class="class-info">
        <div class="class-name">
          <Icon type="ios-school" size="22" style="margin-right:8px;"/>
          {{ session.title }}
        </div>
        <div class="class-meta">
          章节：{{ session.chapter_title }} &nbsp;·&nbsp;
          剩余时间：<span class="countdown" :class="{'urgent': countdown < 600}">{{ countdownText }}</span>
        </div>
      </div>
      <Button type="ghost" size="small" @click="$router.push('/problem')">退出课堂模式</Button>
    </div>

    <!-- 题目列表 -->
    <div class="class-body">
      <Spin v-if="loading" fix size="large"></Spin>

      <div v-if="!loading && problems.length === 0" class="empty-tip">
        本课堂暂无题目
      </div>

      <div v-for="(p, index) in problems" :key="p.id"
           class="problem-card"
           :class="statusClass(p)"
           @click="openProblem(p)">
        <div class="card-left">
          <div class="problem-index">{{ index + 1 }}</div>
          <div class="status-icon">
            <Icon v-if="p.my_status === 0" type="checkmark-circled" color="#67c23a" size="26"/>
            <Icon v-else-if="p.my_status !== null && p.my_status !== undefined"
                  type="minus-circled" color="#e6a23c" size="26"/>
            <Icon v-else type="radio-button-off" color="#c0c4cc" size="26"/>
          </div>
        </div>
        <div class="card-body">
          <div class="problem-title">{{ p._id }} &nbsp;{{ p.title }}</div>
          <div class="problem-meta">
            <Tag :color="diffColor(p.difficulty)" size="small">{{ diffLabel(p.difficulty) }}</Tag>
          </div>
        </div>
        <div class="card-right">
          <div class="ac-count">
            <Icon type="ios-people" size="16" style="margin-right:4px;"/>
            {{ p.ac_count }} 位同学已通过
          </div>
          <div class="card-arrow">
            <Icon type="ios-arrow-forward" size="20" color="#c0c4cc"/>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
  import api from '@oj/api'

  export default {
    name: 'ClassMode',
    data () {
      return {
        sessionId: null,
        session: {},
        problems: [],
        loading: false,
        now: Date.now(),
        ticker: null,
        refreshTimer: null
      }
    },
    computed: {
      endTime () {
        return this.session.end_time ? new Date(this.session.end_time).getTime() : 0
      },
      countdown () {
        return Math.max(0, Math.floor((this.endTime - this.now) / 1000))
      },
      countdownText () {
        const s = this.countdown
        const h = Math.floor(s / 3600)
        const m = Math.floor((s % 3600) / 60)
        const sec = s % 60
        if (h > 0) return `${h}时${m}分${sec}秒`
        if (m > 0) return `${m}分${sec}秒`
        return `${sec}秒`
      }
    },
    mounted () {
      this.sessionId = this.$route.params.sessionId || this.$route.query.session_id
      if (!this.sessionId) {
        this.$router.replace('/problem')
        return
      }
      this.load()
      this.ticker = setInterval(() => { this.now = Date.now() }, 1000)
      this.refreshTimer = setInterval(() => this.load(), 60000)
      // 上报进入课堂（无具体题目）
      api.reportActivity(this.sessionId, null).catch(() => {})
    },
    beforeDestroy () {
      clearInterval(this.ticker)
      clearInterval(this.refreshTimer)
    },
    methods: {
      load () {
        this.loading = true
        api.getClassProgress(this.sessionId).then(res => {
          const d = res.data.data
          this.session = d.session
          this.problems = d.problems
          this.loading = false
        }).catch(() => {
          this.loading = false
          this.$Message.error('课堂已结束或无法访问')
          this.$router.replace('/problem')
        })
      },
      openProblem (p) {
        api.reportActivity(this.sessionId, p.id).catch(() => {})
        this.$router.push({
          name: 'problem-details',
          params: { problemID: p._id },
          query: { class_session: this.sessionId }
        })
      },
      statusClass (p) {
        if (p.my_status === 0) return 'status-solved'
        if (p.my_status !== null && p.my_status !== undefined) return 'status-tried'
        return ''
      },
      diffColor (d) { return d === 'Low' ? 'green' : d === 'High' ? 'yellow' : 'blue' },
      diffLabel (d) { return d === 'Low' ? '低' : d === 'High' ? '高' : '中' }
    }
  }
</script>

<style lang="less" scoped>
  .class-mode {
    min-height: 100vh;
    background: #f0f2f5;
  }

  .class-header {
    position: sticky;
    top: 0;
    z-index: 100;
    background: linear-gradient(135deg, #1a237e 0%, #1565c0 100%);
    padding: 14px 32px;
    display: flex;
    justify-content: space-between;
    align-items: center;
    box-shadow: 0 2px 12px rgba(21,101,192,0.4);
  }

  .class-name {
    font-size: 20px;
    font-weight: 700;
    color: #fff;
    display: flex;
    align-items: center;
  }

  .class-meta {
    font-size: 13px;
    color: rgba(255,255,255,0.75);
    margin-top: 4px;
  }

  .countdown {
    color: #40c4ff;
    font-weight: 600;
    font-family: monospace;
    &.urgent { color: #ff6b6b; animation: blink 1s infinite; }
  }

  @keyframes blink {
    50% { opacity: 0.5; }
  }

  .class-body {
    max-width: 860px;
    margin: 32px auto;
    padding: 0 16px;
    position: relative;
    min-height: 300px;
  }

  .empty-tip {
    text-align: center;
    padding: 80px;
    color: #909399;
    font-size: 16px;
  }

  .problem-card {
    display: flex;
    align-items: center;
    background: #fff;
    border-radius: 12px;
    box-shadow: 0 2px 10px rgba(0,0,0,0.07);
    margin-bottom: 14px;
    padding: 16px 20px;
    cursor: pointer;
    transition: all 0.25s;
    border-left: 4px solid #e4e7ed;

    &:hover {
      box-shadow: 0 6px 20px rgba(21,101,192,0.15);
      transform: translateY(-2px);
      border-left-color: #1565c0;
    }

    &.status-solved {
      border-left-color: #67c23a;
      background: #f0faf4;
    }
    &.status-tried {
      border-left-color: #e6a23c;
      background: #fdf8ee;
    }
  }

  .card-left {
    display: flex;
    align-items: center;
    gap: 12px;
    flex-shrink: 0;
    margin-right: 16px;
  }

  .problem-index {
    width: 32px;
    height: 32px;
    border-radius: 50%;
    background: #e8f0fe;
    color: #1565c0;
    font-weight: 700;
    font-size: 14px;
    display: flex;
    align-items: center;
    justify-content: center;
  }

  .card-body {
    flex: 1;
    min-width: 0;
  }

  .problem-title {
    font-size: 16px;
    font-weight: 600;
    color: #303133;
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
  }

  .problem-meta {
    margin-top: 4px;
  }

  .card-right {
    flex-shrink: 0;
    margin-left: 16px;
    text-align: right;
    display: flex;
    align-items: center;
    gap: 12px;
  }

  .ac-count {
    font-size: 13px;
    color: #909399;
    white-space: nowrap;
    display: flex;
    align-items: center;
  }
</style>
