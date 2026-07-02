<template>
  <Row type="flex" justify="space-around">
    <Col :span="20" id="status">
      <Alert :type="status.type" showIcon>
        <span class="title">{{$t('m.' + status.statusName.replace(/ /g, "_"))}}</span>
        <div slot="desc" class="content">
          <template v-if="isCE">
            <pre>{{submission.statistic_info.err_info}}</pre>
          </template>
          <template v-else>
            <span>{{$t('m.Time')}}: {{submission.statistic_info.time_cost | submissionTime}}</span>
            <span>{{$t('m.Memory')}}: {{submission.statistic_info.memory_cost | submissionMemory}}</span>
            <span>{{$t('m.Lang')}}: {{submission.language}}</span>
            <span>{{$t('m.Author')}}: {{submission.username}}</span>
          </template>
        </div>
      </Alert>
    </Col>

    <Col v-if="canDiagnose && aiEnabled && aiResultAllowed" :span="20">
      <div id="ai-diagnosis-bar">
        <Button :type="aiHasResult ? 'success' : 'info'"
                :icon="aiHasResult ? 'ios-checkmark-outline' : 'ios-lightbulb-outline'"
                :loading="aiLoading" @click="runAIDiagnosis">
          {{ aiHasResult ? '查看已生成的 AI 诊断' : 'AI 诊断' }}
        </Button>
        <span class="ai-tip">
          <template v-if="aiHasResult">该提交已诊断，点击查看（不再消耗次数）</template>
          <template v-else>代码没通过？让 AI 帮你分析错误原因</template>
          <template v-if="aiRemaining !== null"> · 今日剩余 {{aiRemaining}} 次</template>
        </span>
      </div>
    </Col>

    <!--后台返info就显示出来， 权限控制放后台 -->
    <Col v-if="submission.info && !isCE" :span="20">
      <Table stripe :loading="loading" :disabled-hover="true" :columns="columns" :data="submission.info.data"></Table>
    </Col>

    <Col :span="20">
      <Highlight :code="submission.code" :language="submission.language" :border-color="status.color"></Highlight>
    </Col>
    <Col v-if="submission.can_unshare" :span="20">
      <div id="share-btn">
        <Button v-if="submission.shared"
                type="warning" size="large" @click="shareSubmission(false)">
          {{$t('m.UnShare')}}
        </Button>
        <Button v-else
                type="primary" size="large" @click="shareSubmission(true)">
          {{$t('m.Share')}}
        </Button>
      </div>
    </Col>

    <!-- 管理员：测试点数据弹窗 -->
    <Modal v-model="tcModal" :title="'测试点 #' + (tcModalIndex + 1) + ' 数据'" width="800" footer-hide>
      <div v-if="tcDetailLoading" style="text-align:center;padding:40px;">
        <Spin size="large"></Spin>
      </div>
      <div v-else-if="currentDetail">
        <div class="tc-section">
          <div class="tc-label">
            输入
            <span v-if="currentDetail.input_truncated" class="tc-truncated">（内容较长，已截断至 2000 字符）</span>
          </div>
          <pre class="tc-pre">{{ currentDetail.input || '（空）' }}</pre>
        </div>
        <div class="tc-section">
          <div class="tc-label">
            预期输出
            <span v-if="currentDetail.expected_truncated" class="tc-truncated">（内容较长，已截断至 2000 字符）</span>
          </div>
          <pre class="tc-pre">{{ currentDetail.expected_output || '（空）' }}</pre>
        </div>
        <div class="tc-section">
          <div class="tc-label">
            实际输出
            <span v-if="currentDetail.actual_truncated" class="tc-truncated">（内容较长，已截断至 2000 字符）</span>
          </div>
          <pre class="tc-pre" :class="{'tc-no-output': !currentDetail.actual_output}">
            {{ currentDetail.actual_output || '（未保存，仅新提交有记录）' }}</pre>
        </div>
      </div>
      <div v-else style="text-align:center;color:#999;padding:30px;">
        数据加载失败
      </div>
    </Modal>

    <Modal v-model="aiModalVisible" title="AI 代码诊断" width="720">
      <Alert type="warning" show-icon style="margin-bottom:12px;">
        AI 诊断仅供参考，目的是帮你定位思路，请独立完成代码修改。
      </Alert>
      <div v-if="aiDiagnosis" class="markdown-body ai-diagnosis-content" v-html="aiDiagnosisHtml"></div>
      <div slot="footer">
        <Button type="ghost" @click="aiModalVisible=false">关闭</Button>
      </div>
    </Modal>
  </Row>
</template>

<script>
  import marked from 'marked'
  import api from '@oj/api'
  import {JUDGE_STATUS} from '@/utils/constants'
  import utils from '@/utils/utils'
  import Highlight from '@/pages/oj/components/Highlight'

  export default {
    name: 'submissionDetails',
    components: {
      Highlight
    },
    data () {
      return {
        columns: [
          {
            title: this.$i18n.t('m.ID'),
            align: 'center',
            type: 'index'
          },
          {
            title: this.$i18n.t('m.Status'),
            align: 'center',
            render: (h, params) => {
              return h('Tag', {
                props: {
                  color: JUDGE_STATUS[params.row.result].color
                }
              }, this.$i18n.t('m.' + JUDGE_STATUS[params.row.result].name.replace(/ /g, '_')))
            }
          },
          {
            title: this.$i18n.t('m.Memory'),
            align: 'center',
            render: (h, params) => {
              return h('span', utils.submissionMemoryFormat(params.row.memory))
            }
          },
          {
            title: this.$i18n.t('m.Time'),
            align: 'center',
            render: (h, params) => {
              return h('span', utils.submissionTimeFormat(params.row.cpu_time))
            }
          }
        ],
        submission: {
          result: '0',
          code: '',
          info: {
            data: []
          },
          statistic_info: {
            time_cost: '',
            memory_cost: ''
          }
        },
        isConcat: false,
        loading: false,
        adminColAdded: false,
        testCaseDetails: [],
        tcDetailLoading: false,
        tcDetailLoaded: false,
        tcModal: false,
        tcModalIndex: 0,
        aiModalVisible: false,
        aiLoading: false,
        aiDiagnosis: '',
        aiEnabled: false,
        aiHasResult: false,
        aiRemaining: null,
        aiAllowedResults: []
      }
    },
    mounted () {
      this.getSubmission()
    },
    beforeDestroy () {
      if (this.refreshStatus) {
        clearTimeout(this.refreshStatus)
      }
    },
    methods: {
      getSubmission () {
        this.loading = true
        api.getSubmission(this.$route.params.id).then(res => {
          this.loading = false
          let data = res.data.data
          if (data.info && data.info.data && !this.isConcat) {
            if (data.info.data[0] && data.info.data[0].score !== undefined) {
              this.isConcat = true
              const scoreColumn = {
                title: this.$i18n.t('m.Score'),
                align: 'center',
                key: 'score'
              }
              this.columns.push(scoreColumn)
              this.loadingTable = false
            }
            if (this.isAdminRole) {
              this.isConcat = true
              const adminColumn = [
                {
                  title: this.$i18n.t('m.Real_Time'),
                  align: 'center',
                  render: (h, params) => {
                    return h('span', utils.submissionTimeFormat(params.row.real_time))
                  }
                },
                {
                  title: this.$i18n.t('m.Signal'),
                  align: 'center',
                  key: 'signal'
                }
              ]
              this.columns = this.columns.concat(adminColumn)
            }
          }
          if (this.isAdminRole && !this.adminColAdded) {
            this.adminColAdded = true
            this.columns.push({
              title: '操作',
              align: 'center',
              width: 100,
              render: (h, params) => {
                return h('Button', {
                  props: { size: 'small', type: 'default' },
                  on: { click: () => { this.openTCDetail(params.index) } }
                }, '查看数据')
              }
            })
          }
          this.submission = data
          // 仍在评测中（Submitting/Pending/Judging）则定时刷新
          if ([6, 7, 9].includes(Number(data.result))) {
            this.refreshStatus = setTimeout(this.getSubmission, 2000)
            return
          }
          if (this.canDiagnose) {
            this.loadAIStatus()
          }
        }, () => {
          this.loading = false
        })
      },
      loadAIStatus () {
        api.getAIDiagnosis(this.submission.id).then(res => {
          let d = res.data.data
          this.aiEnabled = d.enabled
          this.aiRemaining = d.remaining
          this.aiAllowedResults = d.allowed_results || []
          if (d.result) {
            this.aiDiagnosis = d.result
            this.aiHasResult = true
          }
        }).catch(() => {})
      },
      openTCDetail (index) {
        this.tcModalIndex = index
        this.tcModal = true
        if (!this.tcDetailLoaded && !this.tcDetailLoading) {
          this.tcDetailLoading = true
          api.getSubmissionTestCaseDetail(this.submission.id).then(res => {
            this.testCaseDetails = res.data.data
            this.tcDetailLoaded = true
            this.tcDetailLoading = false
          }).catch(() => {
            this.tcDetailLoading = false
          })
        }
      },
      shareSubmission (shared) {
        let data = {id: this.submission.id, shared: shared}
        api.updateSubmission(data).then(res => {
          this.getSubmission()
          this.$success(this.$i18n.t('m.Succeeded'))
        }, () => {
        })
      },
      runAIDiagnosis () {
        // 已有诊断：直接看缓存，不再请求、不扣次数
        if (this.aiHasResult) {
          this.aiModalVisible = true
          return
        }
        this.aiLoading = true
        api.requestAIDiagnosis(this.submission.id).then(res => {
          this.aiLoading = false
          this.aiDiagnosis = res.data.data.result
          this.aiHasResult = true
          if (res.data.data.remaining !== undefined) {
            this.aiRemaining = res.data.data.remaining
          }
          this.aiModalVisible = true
        }, () => {
          this.aiLoading = false
        })
      }
    },
    computed: {
      status () {
        return {
          type: JUDGE_STATUS[this.submission.result].type,
          statusName: JUDGE_STATUS[this.submission.result].name,
          color: JUDGE_STATUS[this.submission.result].color
        }
      },
      isCE () {
        return this.submission.result === -2
      },
      canDiagnose () {
        // 仅本人（或管理员）可诊断自己的非 AC 提交；can_unshare 表示拥有该提交的权限
        return this.submission.can_unshare &&
          [-2, -1, 1, 2, 3, 4, 8].includes(Number(this.submission.result))
      },
      aiResultAllowed () {
        // 该判题状态是否被管理员开放诊断
        return this.aiAllowedResults.includes(Number(this.submission.result))
      },
      aiDiagnosisHtml () {
        return this.aiDiagnosis ? marked(this.aiDiagnosis) : ''
      },
      isAdminRole () {
        return this.$store.getters.isAdminRole
      },
      currentDetail () {
        return this.testCaseDetails[this.tcModalIndex] || null
      }
    }
  }
</script>

<style scoped lang="less">
  #status {
    .title {
      font-size: 20px;
    }
    .content {
      margin-top: 10px;
      font-size: 14px;
      span {
        margin-right: 10px;
      }
      pre {
        white-space: pre-wrap;
        word-wrap: break-word;
        word-break: break-all;
      }
    }
  }

  .admin-info {
    margin: 5px 0;
    &-content {
      font-size: 16px;
      padding: 10px;
    }
  }

  #share-btn {
    float: right;
    margin-top: 5px;
    margin-right: 10px;
  }

  #ai-diagnosis-bar {
    margin: 4px 0;
    .ai-tip {
      margin-left: 12px;
      font-size: 13px;
      color: #999;
    }
  }

  .ai-diagnosis-content {
    max-height: 60vh;
    overflow-y: auto;
    line-height: 1.7;
  }

  pre {
    border: none;
    background: none;
  }

  .tc-section {
    margin-bottom: 16px;
  }

  .tc-label {
    font-weight: 600;
    font-size: 13px;
    color: #515a6e;
    margin-bottom: 6px;
  }

  .tc-truncated {
    font-weight: normal;
    font-size: 12px;
    color: #e6a23c;
    margin-left: 6px;
  }

  .tc-pre {
    background: #f7f8fa;
    border: 1px solid #e8eaec;
    border-radius: 4px;
    padding: 10px 14px;
    font-size: 13px;
    line-height: 1.6;
    white-space: pre-wrap;
    word-break: break-all;
    max-height: 200px;
    overflow-y: auto;
    margin: 0;
  }

  .tc-no-output {
    color: #999;
    font-style: italic;
  }
</style>
