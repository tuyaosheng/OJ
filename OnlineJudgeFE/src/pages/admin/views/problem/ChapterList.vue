<template>
  <div>
    <panel title="章节管理">
      <div slot="header">
        <el-button type="primary" size="small" icon="el-icon-plus" @click="openCreateDialog">新建章节</el-button>
      </div>

      <el-table :data="chapters" v-loading="loading" row-key="id">
        <el-table-column label="排序" width="70" prop="order"></el-table-column>
        <el-table-column label="章节名称" prop="title" min-width="200"></el-table-column>
        <el-table-column label="描述" prop="description" min-width="260" show-overflow-tooltip></el-table-column>
        <el-table-column label="题目数" width="80" prop="problem_count"></el-table-column>
        <el-table-column label="操作" width="240">
          <template slot-scope="scope">
            <el-button size="mini" type="primary" @click="openEditDialog(scope.row)">编辑</el-button>
            <el-button size="mini" type="success" @click="openProblemDialog(scope.row)">管理题目</el-button>
            <el-button size="mini" type="danger" @click="deleteChapter(scope.row)">删除</el-button>
          </template>
        </el-table-column>
      </el-table>
    </panel>

    <!-- 新建/编辑章节对话框 -->
    <el-dialog :title="dialogMode === 'create' ? '新建章节' : '编辑章节'"
               :visible.sync="chapterDialogVisible" width="500px">
      <el-form :model="chapterForm" label-width="80px">
        <el-form-item label="章节名称" required>
          <el-input v-model="chapterForm.title" placeholder="请输入章节名称"></el-input>
        </el-form-item>
        <el-form-item label="描述">
          <el-input type="textarea" :rows="3" v-model="chapterForm.description" placeholder="章节简介（选填）"></el-input>
        </el-form-item>
        <el-form-item label="排序号">
          <el-input-number v-model="chapterForm.order" :min="0" :max="9999"></el-input-number>
          <span style="color:#909399;font-size:12px;margin-left:8px;">数字越小越靠前</span>
        </el-form-item>
      </el-form>
      <div slot="footer">
        <el-button @click="chapterDialogVisible = false">取消</el-button>
        <el-button type="primary" @click="saveChapter" :loading="saving">保存</el-button>
      </div>
    </el-dialog>

    <!-- 管理题目对话框 -->
    <el-dialog :title="'「' + currentChapter.title + '」— 题目管理'"
               :visible.sync="problemDialogVisible" width="800px" @open="loadChapterDetail">
      <el-row :gutter="16">
        <el-col :span="14">
          <div class="section-label">章节内题目</div>
          <el-table :data="currentProblems" size="small" v-loading="detailLoading">
            <el-table-column label="ID" prop="_id" width="80"></el-table-column>
            <el-table-column label="题目名称" prop="title" show-overflow-tooltip></el-table-column>
            <el-table-column label="操作" width="70">
              <template slot-scope="scope">
                <el-button size="mini" type="danger" icon="el-icon-delete"
                           @click="removeProblem(scope.row)"></el-button>
              </template>
            </el-table-column>
          </el-table>
        </el-col>
        <el-col :span="10">
          <div class="section-label">添加题目</div>
          <el-input v-model="searchKeyword" placeholder="搜索题目ID或名称"
                    prefix-icon="el-icon-search" size="small"
                    @input="searchProblems"></el-input>
          <div class="search-result">
            <div v-for="p in searchResults" :key="p.id" class="search-item"
                 @click="addProblem(p)">
              <span class="pid">{{ p._id }}</span>
              <span class="ptitle">{{ p.title }}</span>
              <i class="el-icon-plus"></i>
            </div>
            <div v-if="searchResults.length === 0 && searchKeyword" class="no-result">未找到题目</div>
          </div>
        </el-col>
      </el-row>
    </el-dialog>
  </div>
</template>

<script>
  import api from '@admin/api'

  export default {
    name: 'ChapterList',
    data () {
      return {
        chapters: [],
        loading: false,
        saving: false,
        chapterDialogVisible: false,
        problemDialogVisible: false,
        dialogMode: 'create',
        chapterForm: { title: '', description: '', order: 0 },
        currentChapter: {},
        currentProblems: [],
        detailLoading: false,
        searchKeyword: '',
        searchResults: [],
        searchTimer: null
      }
    },
    mounted () {
      this.loadChapters()
    },
    methods: {
      loadChapters () {
        this.loading = true
        api.getChapterList().then(res => {
          this.chapters = res.data.data
          this.loading = false
        }).catch(() => { this.loading = false })
      },
      openCreateDialog () {
        this.dialogMode = 'create'
        this.chapterForm = { title: '', description: '', order: this.chapters.length }
        this.chapterDialogVisible = true
      },
      openEditDialog (row) {
        this.dialogMode = 'edit'
        this.chapterForm = { id: row.id, title: row.title, description: row.description || '', order: row.order }
        this.chapterDialogVisible = true
      },
      saveChapter () {
        if (!this.chapterForm.title.trim()) {
          this.$error('章节名称不能为空')
          return
        }
        this.saving = true
        const fn = this.dialogMode === 'create' ? api.createChapter : api.editChapter
        fn(this.chapterForm).then(() => {
          this.$success(this.dialogMode === 'create' ? '创建成功' : '保存成功')
          this.chapterDialogVisible = false
          this.loadChapters()
        }).catch(() => {
          this.$error('操作失败')
        }).finally(() => { this.saving = false })
      },
      deleteChapter (row) {
        this.$confirm(`确定删除章节「${row.title}」吗？题目不会被删除。`, '确认删除', { type: 'warning' }).then(() => {
          api.deleteChapter(row.id).then(() => {
            this.$success('删除成功')
            this.loadChapters()
          })
        }).catch(() => {})
      },
      openProblemDialog (row) {
        this.currentChapter = row
        this.searchKeyword = ''
        this.searchResults = []
        this.problemDialogVisible = true
      },
      loadChapterDetail () {
        this.detailLoading = true
        api.getChapter(this.currentChapter.id).then(res => {
          this.currentProblems = res.data.data.problems || []
          this.detailLoading = false
        }).catch(() => { this.detailLoading = false })
      },
      searchProblems () {
        clearTimeout(this.searchTimer)
        if (!this.searchKeyword.trim()) {
          this.searchResults = []
          return
        }
        this.searchTimer = setTimeout(() => {
          api.getProblemList({ keyword: this.searchKeyword, limit: 10, offset: 0 }).then(res => {
            const existing = new Set(this.currentProblems.map(p => p.id))
            this.searchResults = (res.data.data.results || []).filter(p => !existing.has(p.id))
          })
        }, 400)
      },
      addProblem (problem) {
        api.addChapterProblem({ chapter_id: this.currentChapter.id, problem_id: problem.id }).then(() => {
          this.currentProblems.push(problem)
          this.searchResults = this.searchResults.filter(p => p.id !== problem.id)
          this.loadChapters()
        }).catch(err => {
          this.$error(err.data.data || '添加失败')
        })
      },
      removeProblem (problem) {
        api.removeChapterProblem(this.currentChapter.id, problem.id).then(() => {
          this.currentProblems = this.currentProblems.filter(p => p.id !== problem.id)
          this.loadChapters()
        })
      }
    }
  }
</script>

<style scoped lang="less">
  .section-label {
    font-weight: 600;
    color: #303133;
    margin-bottom: 8px;
    font-size: 13px;
  }
  .search-result {
    margin-top: 8px;
    max-height: 300px;
    overflow-y: auto;
    border: 1px solid #ebeef5;
    border-radius: 4px;
  }
  .search-item {
    display: flex;
    align-items: center;
    padding: 8px 12px;
    cursor: pointer;
    font-size: 13px;
    border-bottom: 1px solid #f5f7fa;
    transition: background 0.2s;
    &:hover { background: #f0f7ff; }
    .pid {
      width: 60px;
      color: #1565c0;
      font-weight: 600;
      flex-shrink: 0;
    }
    .ptitle {
      flex: 1;
      overflow: hidden;
      text-overflow: ellipsis;
      white-space: nowrap;
      color: #303133;
    }
    i { color: #67c23a; margin-left: 8px; }
  }
  .no-result {
    padding: 20px;
    text-align: center;
    color: #909399;
    font-size: 13px;
  }
</style>
