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
  </Row>
</template>

<script>
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
        tcModalIndex: 0
      }
    },
    mounted () {
      this.getSubmission()
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
        }, () => {
          this.loading = false
        })
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
