<template>
  <Row type="flex" :gutter="18">
    <Col :span=19>

    <!-- 视图切换 + 课堂入口 -->
    <div class="view-switch">
      <Button :type="viewMode==='list'?'primary':'ghost'" @click="viewMode='list'" icon="ios-list">列表模式</Button>
      <Button :type="viewMode==='chapter'?'primary':'ghost'" @click="switchChapterView" icon="ios-bookmarks" style="margin-left:8px;">章节模式</Button>
      <template v-if="activeSessions.length">
        <Button v-for="s in activeSessions" :key="s.id"
                type="error" icon="ios-school"
                style="margin-left:12px;"
                @click="$router.push({name:'class-mode',params:{sessionId:s.id}})">
          进入课堂：{{ s.title }}
        </Button>
      </template>
    </div>

    <!-- 章节模式 -->
    <div v-if="viewMode==='chapter'">
      <Spin v-if="chapterLoading" fix size="large"></Spin>
      <div v-if="!chapterLoading && chapters.length === 0" style="text-align:center;padding:60px;color:#909399;">
        暂无章节，请管理员在后台添加章节并分配题目。
      </div>
      <div v-for="chapter in chapters" :key="chapter.id" class="chapter-card">
        <div class="chapter-header" @click="toggleChapter(chapter.id)">
          <span class="chapter-title">
            <Icon :type="expandedChapters.has(chapter.id)?'ios-arrow-down':'ios-arrow-forward'" style="margin-right:6px;"/>
            {{ chapter.title }}
          </span>
          <span class="chapter-meta">{{ chapter.problems.length }} 道题</span>
        </div>
        <div v-if="chapter.description" class="chapter-desc">{{ chapter.description }}</div>
        <transition name="chapter-slide">
          <div v-show="expandedChapters.has(chapter.id)" class="chapter-problems">
            <div v-if="chapter.problems.length === 0" class="empty-tip">该章节暂无题目</div>
            <table v-else class="problem-table">
              <thead>
                <tr>
                  <th style="width:80px">编号</th>
                  <th>题目名称</th>
                  <th style="width:80px">难度</th>
                  <th style="width:80px">通过率</th>
                  <th style="width:60px">状态</th>
                </tr>
              </thead>
              <tbody>
                <tr v-for="p in chapter.problems" :key="p.id"
                    @click="$router.push({name:'problem-details',params:{problemID:p._id}})"
                    class="problem-row">
                  <td><span class="pid">{{ p._id }}</span></td>
                  <td class="ptitle">{{ p.title }}</td>
                  <td>
                    <Tag :color="difficultyColor(p.difficulty)" size="small">
                      {{ $t('m.'+p.difficulty) }}
                    </Tag>
                  </td>
                  <td>{{ getACRate(p.accepted_number, p.submission_number) }}</td>
                  <td>
                    <Icon v-if="p.my_status===0" type="checkmark-circled" color="#67c23a" size="18" title="已通过"/>
                    <Icon v-else-if="p.my_status!==null&&p.my_status!==undefined" type="minus-circled" color="#e6a23c" size="18" title="尝试过"/>
                  </td>
                </tr>
              </tbody>
            </table>
          </div>
        </transition>
      </div>
    </div>

    <!-- 列表模式（原有） -->
    <Panel shadow v-if="viewMode==='list'">
      <div slot="title">{{$t('m.Problem_List')}}</div>
      <div slot="extra">
        <ul class="filter">
          <li>
            <Dropdown @on-click="filterByDifficulty">
              <span>{{query.difficulty === '' ? this.$i18n.t('m.Difficulty') : this.$i18n.t('m.' + query.difficulty)}}
                <Icon type="arrow-down-b"></Icon>
              </span>
              <Dropdown-menu slot="list">
                <Dropdown-item name="">{{$t('m.All')}}</Dropdown-item>
                <Dropdown-item name="Low">{{$t('m.Low')}}</Dropdown-item>
                <Dropdown-item name="Mid" >{{$t('m.Mid')}}</Dropdown-item>
                <Dropdown-item name="High">{{$t('m.High')}}</Dropdown-item>
              </Dropdown-menu>
            </Dropdown>
          </li>
          <li>
            <i-switch size="large" @on-change="handleTagsVisible">
              <span slot="open">{{$t('m.Tags')}}</span>
              <span slot="close">{{$t('m.Tags')}}</span>
            </i-switch>
          </li>
          <li>
            <Input v-model="query.keyword"
                   @on-enter="filterByKeyword"
                   @on-click="filterByKeyword"
                   placeholder="keyword"
                   icon="ios-search-strong"/>
          </li>
          <li>
            <Button type="info" @click="onReset">
              <Icon type="refresh"></Icon>
              {{$t('m.Reset')}}
            </Button>
          </li>
        </ul>
      </div>
      <Table style="width: 100%; font-size: 16px;"
             :columns="problemTableColumns"
             :data="problemList"
             :loading="loadings.table"
             disabled-hover></Table>
    </Panel>
    <Pagination v-if="viewMode==='list'"
      :total="total" :page-size.sync="query.limit" @on-change="pushRouter" @on-page-size-change="pushRouter" :current.sync="query.page" :show-sizer="true"></Pagination>

    </Col>

    <Col :span="5">
    <Panel :padding="10">
      <div slot="title" class="taglist-title">{{$t('m.Tags')}}</div>
      <Button v-for="tag in tagList"
              :key="tag.name"
              @click="filterByTag(tag.name)"
              type="ghost"
              :disabled="query.tag === tag.name"
              shape="circle"
              class="tag-btn">{{tag.name}}
      </Button>

      <Button long id="pick-one" @click="pickone">
        <Icon type="shuffle"></Icon>
        {{$t('m.Pick_One')}}
      </Button>
    </Panel>
    <Spin v-if="loadings.tag" fix size="large"></Spin>
    </Col>
  </Row>
</template>

<script>
  import { mapGetters } from 'vuex'
  import api from '@oj/api'
  import utils from '@/utils/utils'
  import { ProblemMixin } from '@oj/components/mixins'
  import Pagination from '@oj/components/Pagination'

  export default {
    name: 'ProblemList',
    mixins: [ProblemMixin],
    components: {
      Pagination
    },
    data () {
      return {
        tagList: [],
        problemTableColumns: [
          {
            title: '#',
            key: '_id',
            width: 80,
            render: (h, params) => {
              return h('Button', {
                props: {
                  type: 'text',
                  size: 'large'
                },
                on: {
                  click: () => {
                    this.$router.push({name: 'problem-details', params: {problemID: params.row._id}})
                  }
                },
                style: {
                  padding: '2px 0'
                }
              }, params.row._id)
            }
          },
          {
            title: this.$i18n.t('m.Title'),
            width: 400,
            render: (h, params) => {
              return h('Button', {
                props: {
                  type: 'text',
                  size: 'large'
                },
                on: {
                  click: () => {
                    this.$router.push({name: 'problem-details', params: {problemID: params.row._id}})
                  }
                },
                style: {
                  padding: '2px 0',
                  overflowX: 'auto',
                  textAlign: 'left',
                  width: '100%'
                }
              }, params.row.title)
            }
          },
          {
            title: this.$i18n.t('m.Level'),
            render: (h, params) => {
              let t = params.row.difficulty
              let color = 'blue'
              if (t === 'Low') color = 'green'
              else if (t === 'High') color = 'yellow'
              return h('Tag', {
                props: {
                  color: color
                }
              }, this.$i18n.t('m.' + params.row.difficulty))
            }
          },
          {
            title: this.$i18n.t('m.Total'),
            key: 'submission_number'
          },
          {
            title: this.$i18n.t('m.AC_Rate'),
            render: (h, params) => {
              return h('span', this.getACRate(params.row.accepted_number, params.row.submission_number))
            }
          }
        ],
        problemList: [],
        limit: 20,
        total: 0,
        loadings: {
          table: true,
          tag: true
        },
        viewMode: 'list',
        chapters: [],
        chapterLoading: false,
        expandedChapters: new Set(),
        activeSessions: [],
        routeName: '',
        query: {
          keyword: '',
          difficulty: '',
          tag: '',
          page: 1,
          limit: 10
        }
      }
    },
    mounted () {
      this.init()
      api.getActiveClassSessions().then(res => {
        this.activeSessions = res.data.data || []
      }).catch(() => {})
    },
    methods: {
      init (simulate = false) {
        this.routeName = this.$route.name
        let query = this.$route.query
        this.query.difficulty = query.difficulty || ''
        this.query.keyword = query.keyword || ''
        this.query.tag = query.tag || ''
        this.query.page = parseInt(query.page) || 1
        if (this.query.page < 1) {
          this.query.page = 1
        }
        this.query.limit = parseInt(query.limit) || 10
        if (!simulate) {
          this.getTagList()
        }
        this.getProblemList()
      },
      pushRouter () {
        this.$router.push({
          name: 'problem-list',
          query: utils.filterEmptyValue(this.query)
        })
      },
      getProblemList () {
        let offset = (this.query.page - 1) * this.query.limit
        this.loadings.table = true
        api.getProblemList(offset, this.limit, this.query).then(res => {
          this.loadings.table = false
          this.total = res.data.data.total
          this.problemList = res.data.data.results
          if (this.isAuthenticated) {
            this.addStatusColumn(this.problemTableColumns, res.data.data.results)
          }
        }, res => {
          this.loadings.table = false
        })
      },
      getTagList () {
        api.getProblemTagList().then(res => {
          this.tagList = res.data.data
          this.loadings.tag = false
        }, res => {
          this.loadings.tag = false
        })
      },
      filterByTag (tagName) {
        this.query.tag = tagName
        this.query.page = 1
        this.pushRouter()
      },
      filterByDifficulty (difficulty) {
        this.query.difficulty = difficulty
        this.query.page = 1
        this.pushRouter()
      },
      filterByKeyword () {
        this.query.page = 1
        this.pushRouter()
      },
      handleTagsVisible (value) {
        if (value) {
          this.problemTableColumns.push(
            {
              title: this.$i18n.t('m.Tags'),
              align: 'center',
              render: (h, params) => {
                let tags = []
                params.row.tags.forEach(tag => {
                  tags.push(h('Tag', {}, tag))
                })
                return h('div', {
                  style: {
                    margin: '8px 0'
                  }
                }, tags)
              }
            })
        } else {
          this.problemTableColumns.splice(this.problemTableColumns.length - 1, 1)
        }
      },
      onReset () {
        this.$router.push({name: 'problem-list'})
      },
      pickone () {
        api.pickone().then(res => {
          this.$success('Good Luck')
          this.$router.push({name: 'problem-details', params: {problemID: res.data.data}})
        })
      },
      switchChapterView () {
        this.viewMode = 'chapter'
        if (this.chapters.length === 0) {
          this.chapterLoading = true
          api.getChapters().then(res => {
            this.chapters = res.data.data
            // 默认展开第一个章节
            if (this.chapters.length > 0) {
              this.expandedChapters = new Set([this.chapters[0].id])
            }
            this.chapterLoading = false
          }).catch(() => { this.chapterLoading = false })
        }
      },
      toggleChapter (id) {
        const s = new Set(this.expandedChapters)
        if (s.has(id)) {
          s.delete(id)
        } else {
          s.add(id)
        }
        this.expandedChapters = s
      },
      difficultyColor (d) {
        return d === 'Low' ? 'green' : d === 'High' ? 'yellow' : 'blue'
      }
    },
    computed: {
      ...mapGetters(['isAuthenticated'])
    },
    watch: {
      '$route' (newVal, oldVal) {
        if (newVal !== oldVal) {
          this.init(true)
        }
      },
      'isAuthenticated' (newVal) {
        if (newVal === true) {
          this.init()
        }
      }
    }
  }
</script>

<style scoped lang="less">
  .taglist-title {
    margin-left: -10px;
    margin-bottom: -10px;
    font-weight: 600;
    color: #303133;
  }

  .tag-btn {
    margin-right: 6px;
    margin-bottom: 10px;
    border-radius: 14px !important;
    font-size: 12px;
    transition: all 0.2s;
    &:hover {
      background: #1565c0 !important;
      color: #fff !important;
      border-color: #1565c0 !important;
    }
  }

  #pick-one {
    margin-top: 14px;
    border-radius: 20px !important;
    background: linear-gradient(135deg, #1565c0, #0288d1) !important;
    color: #fff !important;
    border: none !important;
    font-weight: 500;
    box-shadow: 0 2px 8px rgba(21,101,192,0.25);
    transition: all 0.3s;
    &:hover {
      box-shadow: 0 4px 14px rgba(21,101,192,0.4);
      opacity: 0.92;
    }
  }

  .filter {
    list-style: none;
    padding: 0;
    margin: 0;
    display: flex;
    align-items: center;
    gap: 10px;
    li {
      display: inline-flex;
      align-items: center;
    }
  }

  .view-switch {
    margin-bottom: 14px;
  }

  .chapter-card {
    background: #fff;
    border-radius: 10px;
    box-shadow: 0 2px 10px rgba(0,0,0,0.07);
    margin-bottom: 16px;
    overflow: hidden;
    transition: box-shadow 0.3s;
    &:hover { box-shadow: 0 4px 18px rgba(0,0,0,0.11); }
  }

  .chapter-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 14px 20px;
    cursor: pointer;
    background: linear-gradient(90deg, #e8f0fe 0%, #fff 100%);
    border-left: 4px solid #1565c0;
    user-select: none;
    &:hover { background: linear-gradient(90deg, #d0e4fc 0%, #f5f9ff 100%); }
  }

  .chapter-title {
    font-size: 16px;
    font-weight: 600;
    color: #1a237e;
  }

  .chapter-meta {
    font-size: 13px;
    color: #909399;
  }

  .chapter-desc {
    padding: 8px 20px;
    font-size: 13px;
    color: #606266;
    background: #fafbff;
    border-bottom: 1px solid #eef0f6;
  }

  .chapter-problems {
    padding: 4px 0;
  }

  .empty-tip {
    padding: 20px;
    text-align: center;
    color: #909399;
    font-size: 13px;
  }

  .problem-table {
    width: 100%;
    border-collapse: collapse;
    font-size: 14px;
    thead tr {
      background: #f5f7fa;
      th {
        padding: 10px 16px;
        text-align: left;
        color: #606266;
        font-weight: 600;
        font-size: 13px;
      }
    }
    tbody .problem-row {
      cursor: pointer;
      transition: background 0.2s;
      td { padding: 10px 16px; border-bottom: 1px solid #f0f0f0; }
      &:hover td { background: #f0f7ff; }
      &:last-child td { border-bottom: none; }
    }
    .pid { color: #1565c0; font-weight: 600; }
    .ptitle { color: #303133; }
  }

  .chapter-slide-enter-active, .chapter-slide-leave-active {
    transition: all 0.3s ease;
    overflow: hidden;
  }
  .chapter-slide-enter, .chapter-slide-leave-to {
    max-height: 0;
    opacity: 0;
  }
  .chapter-slide-enter-to, .chapter-slide-leave {
    max-height: 2000px;
    opacity: 1;
  }
</style>
