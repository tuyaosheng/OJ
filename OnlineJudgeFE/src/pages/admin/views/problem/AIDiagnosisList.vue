<template>
  <div>
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

    <el-dialog title="AI 诊断结果" :visible.sync="dialogVisible" width="60%">
      <div v-if="current">
        <p style="margin-bottom:10px;color:#666;">
          {{current.username}} · {{current.problem_id}} {{current.problem_title}} · {{current.create_time | localtime}}
        </p>
        <div class="markdown-body" v-html="currentHtml"></div>
      </div>
    </el-dialog>
  </div>
</template>

<script>
  import marked from 'marked'
  import api from '../../api'

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
        current: null
      }
    },
    mounted () {
      this.getList()
    },
    methods: {
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
