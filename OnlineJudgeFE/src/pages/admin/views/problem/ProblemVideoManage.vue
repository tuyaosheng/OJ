<template>
  <div>
    <panel title="题目视频管理">
      <div slot="header">
        <el-row :gutter="20">
          <el-col :span="8">
            <el-input v-model="query.keyword" placeholder="按显示ID或标题搜索" @keyup.enter.native="search">
              <el-button slot="append" icon="el-icon-search" @click="search"></el-button>
            </el-input>
          </el-col>
          <el-col :span="6">
            <el-select v-model="query.hasVideo" placeholder="视频状态" @change="search" clearable>
              <el-option label="全部" value=""></el-option>
              <el-option label="已上传" value="true"></el-option>
              <el-option label="未上传" value="false"></el-option>
            </el-select>
          </el-col>
        </el-row>
      </div>

      <div class="batch-upload-area">
        <h4>批量上传视频</h4>
        <p class="tip">
          文件名支持三种写法，系统会按顺序自动匹配对应题目：<br>
          1. <code>题目显示ID.mp4</code>（如 <code>ch11-001.mp4</code>）—— 最推荐，唯一不会匹配错<br>
          2. <code>题目显示ID_标题.mp4</code>（如 <code>ch11-001_两数之和.mp4</code>）—— 下划线前必须是准确的显示ID<br>
          3. <code>标题.mp4</code>（如 <code>两数之和.mp4</code>）—— 按标题搜索，题库里若有多道同名题会列出来手动选择
        </p>
        <input type="file" multiple accept="video/mp4,video/webm,video/ogg"
               @change="onFilesSelected" ref="fileInput">

        <div v-if="matchResults.length" style="margin-top:15px;">
          <el-table :data="matchResults" size="small">
            <el-table-column label="文件名" prop="fileName"></el-table-column>
            <el-table-column label="匹配到的题目">
              <template slot-scope="{row}">
                <span v-if="row.status === 'matched'">{{row.matchedProblem._id}} - {{row.matchedProblem.title}}</span>
                <el-select v-else-if="row.status === 'ambiguous'" v-model="row.selectedId"
                           placeholder="标题重复，请选择具体题目" size="small" style="width:100%;">
                  <el-option v-for="p in row.candidates" :key="p.id"
                             :label="`${p._id} - ${p.title}`" :value="p.id"></el-option>
                </el-select>
                <span v-else style="color:#f56c6c;">未匹配到题目</span>
              </template>
            </el-table-column>
            <el-table-column label="状态" width="100">
              <template slot-scope="{row}">
                <el-tag :type="statusTagType(row)" size="small">{{statusText(row)}}</el-tag>
              </template>
            </el-table-column>
          </el-table>
          <div style="margin-top:10px;">
            <el-button type="primary" :loading="uploading" @click="startBatchUpload">开始上传</el-button>
            <el-button @click="clearMatches">清空</el-button>
          </div>
        </div>
      </div>

      <el-table :data="list" v-loading="loading" style="margin-top:20px;">
        <el-table-column prop="_id" label="显示ID" width="140"></el-table-column>
        <el-table-column prop="title" label="标题"></el-table-column>
        <el-table-column label="视频状态" width="120">
          <template slot-scope="{row}">
            <el-tag :type="row.video ? 'success' : 'info'" size="small">{{row.video ? '已上传' : '未上传'}}</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="操作" width="220">
          <template slot-scope="{row}">
            <el-button v-if="row.video" size="small" @click="preview(row)">预览</el-button>
            <el-upload
              v-if="!row.video"
              action=""
              :show-file-list="false"
              :before-upload="(file) => manualUpload(row, file)"
              accept="video/mp4,video/webm,video/ogg"
              style="display:inline-block;margin-right:6px;">
              <el-button size="small" type="primary">上传</el-button>
            </el-upload>
            <el-button v-if="row.video" size="small" type="danger" @click="deleteVideo(row)">删除</el-button>
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

    <el-dialog title="视频预览" :visible.sync="previewVisible" width="600px">
      <video v-if="previewUrl" :src="previewUrl" controls style="width:100%;"></video>
    </el-dialog>
  </div>
</template>

<script>
  import api from '../../api'

  export default {
    name: 'ProblemVideoManage',
    data () {
      return {
        loading: false,
        list: [],
        total: 0,
        page: 1,
        limit: 15,
        query: {
          keyword: '',
          hasVideo: ''
        },
        matchResults: [],
        allProblemsCache: null,
        uploading: false,
        previewVisible: false,
        previewUrl: ''
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
          keyword: this.query.keyword,
          has_video: this.query.hasVideo
        }
        api.getProblemList(params).then(res => {
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
      preview (row) {
        this.previewUrl = row.video
        this.previewVisible = true
      },
      manualUpload (row, file) {
        const allowed = ['video/mp4', 'video/webm', 'video/ogg']
        if (!allowed.includes(file.type)) {
          this.$error('仅支持 mp4/webm/ogg 格式')
          return false
        }
        if (file.size > 500 * 1024 * 1024) {
          this.$error('视频文件大小不能超过 500MB')
          return false
        }
        api.uploadProblemVideo(row.id, file).then(() => {
          this.getList()
          this.allProblemsCache = null
        })
        return false
      },
      deleteVideo (row) {
        this.$confirm('确定要删除这道题的视频吗？', '提示', {type: 'warning'}).then(() => {
          api.deleteProblemVideo(row.id).then(() => {
            this.getList()
            this.allProblemsCache = null
          })
        }).catch(() => {})
      },
      // 拉取全部题目（不分页）用于批量上传时在浏览器内做文件名匹配
      fetchAllProblems () {
        if (this.allProblemsCache) {
          return Promise.resolve(this.allProblemsCache)
        }
        let all = []
        const limit = 250
        const step = (offset) => {
          return api.getProblemList({limit, offset}).then(res => {
            let data = res.data.data
            all = all.concat(data.results)
            if (data.results.length === limit && all.length < data.total) {
              return step(offset + limit)
            }
            this.allProblemsCache = all
            return all
          })
        }
        return step(0)
      },
      // 按 “ID / ID_标题 / 标题” 三种规则匹配文件名对应的题目
      matchFile (baseName, problems) {
        let byId = problems.find(p => p._id === baseName)
        if (byId) {
          return {status: 'matched', candidates: [byId]}
        }

        let idx = baseName.indexOf('_')
        if (idx > 0) {
          let prefix = baseName.slice(0, idx)
          let byPrefixId = problems.find(p => p._id === prefix)
          if (byPrefixId) {
            return {status: 'matched', candidates: [byPrefixId]}
          }
        }

        let byTitle = problems.filter(p => p.title === baseName)
        if (byTitle.length === 1) {
          return {status: 'matched', candidates: byTitle}
        }
        if (byTitle.length > 1) {
          return {status: 'ambiguous', candidates: byTitle}
        }

        return {status: 'unmatched', candidates: []}
      },
      onFilesSelected (e) {
        let files = Array.from(e.target.files)
        if (!files.length) {
          return
        }
        this.matchResults = []
        this.fetchAllProblems().then(problems => {
          this.matchResults = files.map(file => {
            let baseName = file.name.replace(/\.[^./]+$/, '')
            let result = this.matchFile(baseName, problems)
            let single = result.candidates.length === 1 ? result.candidates[0] : null
            return {
              fileName: file.name,
              file,
              status: result.status,
              matchedProblem: result.status === 'matched' ? single : null,
              candidates: result.candidates,
              selectedId: single ? single.id : null,
              uploadStatus: 'pending'
            }
          })
        })
      },
      statusText (row) {
        if (row.uploadStatus === 'success') return '上传成功'
        if (row.uploadStatus === 'error') return '上传失败'
        if (row.uploadStatus === 'uploading') return '上传中'
        if (row.status === 'matched') return '待上传'
        if (row.status === 'ambiguous') return '需选择'
        return '未匹配'
      },
      statusTagType (row) {
        if (row.uploadStatus === 'success') return 'success'
        if (row.uploadStatus === 'error') return 'danger'
        if (row.status === 'matched') return ''
        if (row.status === 'ambiguous') return 'warning'
        return 'danger'
      },
      startBatchUpload () {
        let uploadable = this.matchResults.filter(r =>
          r.status === 'matched' || (r.status === 'ambiguous' && r.selectedId))
        if (!uploadable.length) {
          this.$error('没有可上传的文件，请先解决未匹配/需选择的项')
          return
        }
        this.uploading = true
        let tasks = uploadable.map(row => {
          row.uploadStatus = 'uploading'
          let problemId = row.status === 'matched' ? row.matchedProblem.id : row.selectedId
          return api.uploadProblemVideo(problemId, row.file).then(() => {
            row.uploadStatus = 'success'
          }).catch(() => {
            row.uploadStatus = 'error'
          })
        })
        Promise.all(tasks).then(() => {
          this.uploading = false
          this.allProblemsCache = null
          this.getList()
        })
      },
      clearMatches () {
        this.matchResults = []
        this.$refs.fileInput.value = ''
      }
    }
  }
</script>

<style scoped lang="less">
  .batch-upload-area {
    border: 1px dashed #dcdfe6;
    border-radius: 4px;
    padding: 15px 20px;
    margin-bottom: 10px;
    background: #fafafa;
    h4 {
      margin: 0 0 10px 0;
    }
    .tip {
      color: #909399;
      font-size: 12px;
      line-height: 1.8;
      margin-bottom: 12px;
      code {
        background: #eef1f6;
        padding: 1px 5px;
        border-radius: 3px;
      }
    }
  }
</style>
