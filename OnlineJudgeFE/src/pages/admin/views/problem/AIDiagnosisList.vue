<template>
  <div>
    <panel title="正在进行的 AI 诊断" v-if="inProgressList.length">
      <el-table :data="inProgressList" size="small">
        <el-table-column label="学生" prop="username" width="140"></el-table-column>
        <el-table-column label="题目">
          <template slot-scope="{row}">{{row.problem_id}} - {{row.problem_title}}</template>
        </el-table-column>
        <el-table-column label="提交结果" width="120">
          <template slot-scope="{row}">
            <el-tag type="danger" size="small">{{resultText(row.submission_result)}}</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="状态" width="160">
          <template slot-scope="{row}">
            <el-tag v-if="row.status === 'pending' && row.elapsed_seconds <= staleSeconds" type="primary" size="small">
              诊断中 · 已运行 {{row.elapsed_seconds}}s
            </el-tag>
            <el-tag v-else-if="row.status === 'pending'" type="warning" size="small">
              疑似卡住 · 已运行 {{row.elapsed_seconds}}s
            </el-tag>
            <el-tag v-else type="danger" size="small">诊断失败</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="操作" width="100">
          <template slot-scope="{row}">
            <el-button size="small" type="text" @click="clearInProgress(row)">清除</el-button>
          </template>
        </el-table-column>
      </el-table>
    </panel>

    <panel title="AI 诊断记录">
      <div slot="header">
        <el-row :gutter="20">
          <el-col :span="8">
            <el-input v-model="query.username" placeholder="按用户名搜索" @keyup.enter.native="search">
              <el-button slot="append" icon="el-icon-search" @click="search"></el-button>
            </el-input>
          </el-col>
          <el-col :span="8">
            <el-input v-model="query.problem" placeholder="按题目显示ID搜索" @keyup.enter.native="search">
              <el-button slot="append" icon="el-icon-search" @click="search"></el-button>
            </el-input>
          </el-col>
        </el-row>
      </div>
      <el-table :data="list" v-loading="loading">
        <el-table-column label="时间" width="160">
          <template slot-scope="{row}">{{row.create_time | localtime}}</template>
        </el-table-column>
        <el-table-column prop="username" label="学生" width="140"></el-table-column>
        <el-table-column label="题目">
          <template slot-scope="{row}">{{row.problem_id}} - {{row.problem_title}}</template>
        </el-table-column>
        <el-table-column label="提交结果" width="140">
          <template slot-scope="{row}">
            <el-tag type="danger" size="small">{{resultText(row.submission_result)}}</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="操作" width="120">
          <template slot-scope="{row}">
            <el-button size="small" @click="view(row)">查看诊断</el-button>
          </template>
        </el-table-column>
      </el-table>
      <div class="panel-options">
        <el-pagination class="page" layout="prev, pager, next"
                       @current-change="pageChange"
                       :page-size="limit"
                       :total="total"
                       :current-page.sync="page">
        </el-pagination>
      </div>
    </panel>

    <el-dialog title="AI 诊断结果" :visible.sync="dialogVisible" width="70%">
      <div v-if="current">
        <p style="margin-bottom:10px;color:#666;">
          {{current.username}} ·
          <span class="problem-link" @click="toggleProblemDetail">
            {{current.problem_id}} {{current.problem_title}}
            <i :class="problemDetailVisible ? 'el-icon-arrow-up' : 'el-icon-arrow-down'"></i>
          </span>
          · {{current.create_time | localtime}}
        </p>

        <div v-if="problemDetailVisible" class="problem-detail" v-loading="problemDetailLoading">
          <template v-if="problemDetail">
            <h4>题目描述</h4>
            <div class="markdown-body" v-html="problemDetail.description"></div>
            <h4>输入描述</h4>
            <div class="markdown-body" v-html="problemDetail.input_description"></div>
            <h4>输出描述</h4>
            <div class="markdown-body" v-html="problemDetail.output_description"></div>
            <h4>样例</h4>
            <div v-for="(s, i) in problemDetail.samples" :key="i" class="sample">
              <div>
                <b>输入 {{i + 1}}</b>
                <pre>{{s.input}}</pre>
              </div>
              <div>
                <b>输出 {{i + 1}}</b>
                <pre>{{s.output}}</pre>
              </div>
            </div>
          </template>
        </div>

        <h4>学生提交代码（{{current.language}}）</h4>
        <Highlight :code="current.code" :language="current.language"></Highlight>

        <h4>AI 诊断结果</h4>
        <div class="markdown-body" v-html="currentHtml"></div>
      </div>
    </el-dialog>
  </div>
</template>

<script>
  import marked from 'marked'
  import api from '../../api'
  import Highlight from '@/pages/oj/components/Highlight'

  const RESULT_MAP = {
    '-2': '编译错误',
    '-1': '答案错误',
    '1': '超时',
    '2': '超时',
    '3': '内存超限',
    '4': '运行错误',
    '8': '部分正确'
  }

  export default {
    name: 'AIDiagnosisList',
    components: {
      Highlight
    },
    data () {
      return {
        loading: false,
        list: [],
        total: 0,
        page: 1,
        limit: 15,
        query: {
          username: '',
          problem: ''
        },
        dialogVisible: false,
        current: null,
        problemDetailVisible: false,
        problemDetailLoading: false,
        problemDetail: null,
        inProgressList: [],
        inProgressTimer: null,
        staleSeconds: 120
      }
    },
    mounted () {
      this.getList()
      this.pollInProgress()
    },
    beforeDestroy () {
      clearTimeout(this.inProgressTimer)
    },
    methods: {
      pollInProgress () {
        clearTimeout(this.inProgressTimer)
        api.getAIDiagnosisInProgress().then(res => {
          this.inProgressList = res.data.data
        }, () => {}).finally(() => {
          this.inProgressTimer = setTimeout(this.pollInProgress, 5000)
        })
      },
      clearInProgress (row) {
        this.$confirm(`确定清除「${row.username} · ${row.problem_id}」的诊断记录吗？清除后该学生可重新发起诊断。`, '提示', {
          type: 'warning'
        }).then(() => {
          api.clearAIDiagnosisInProgress(row.id).then(() => {
            this.$message.success('已清除')
            this.pollInProgress()
          })
        }).catch(() => {})
      },
      getList () {
        this.loading = true
        let params = {
          offset: (this.page - 1) * this.limit,
          limit: this.limit,
          username: this.query.username,
          problem: this.query.problem
        }
        api.getAIDiagnosisList(params).then(res => {
          this.loading = false
          this.list = res.data.data.results
          this.total = res.data.data.total
        }, () => {
          this.loading = false
        })
      },
      search () {
        this.page = 1
        this.getList()
      },
      pageChange (p) {
        this.page = p
        this.getList()
      },
      view (row) {
        this.current = row
        this.dialogVisible = true
        this.problemDetailVisible = false
        this.problemDetail = null
      },
      toggleProblemDetail () {
        this.problemDetailVisible = !this.problemDetailVisible
        if (this.problemDetailVisible && !this.problemDetail) {
          this.problemDetailLoading = true
          api.getProblem(this.current.problem_pk).then(res => {
            this.problemDetailLoading = false
            this.problemDetail = res.data.data
          }, () => {
            this.problemDetailLoading = false
          })
        }
      },
      resultText (code) {
        return RESULT_MAP[String(code)] || '未通过'
      }
    },
    computed: {
      currentHtml () {
        return this.current ? marked(this.current.result) : ''
      }
    }
  }
</script>

<style scoped lang="less">
  .problem-link {
    cursor: pointer;
    color: #409EFF;
    &:hover {
      text-decoration: underline;
    }
  }
  .problem-detail {
    background: #f7f8fa;
    border: 1px solid #eee;
    border-radius: 4px;
    padding: 10px 15px;
    margin-bottom: 15px;
    h4 {
      margin: 10px 0 5px 0;
    }
    .sample {
      display: flex;
      gap: 15px;
      margin-bottom: 10px;
      > div {
        flex: 1;
        pre {
          background: #fff;
          border: 1px solid #eee;
          padding: 8px;
          white-space: pre-wrap;
        }
      }
    }
  }
</style>
