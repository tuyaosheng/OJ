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
               :visible.sync="problemDialogVisible" width="1100px"
               @open="onProblemDialogOpen">
      <el-row :gutter="20">

        <!-- 左侧：已加入章节的题目 -->
        <el-col :span="11">
          <div class="section-label">
            已加入本章节
            <span class="count-badge">{{ currentProblems.length }}</span>
          </div>
          <el-table :data="currentProblems" size="small" v-loading="detailLoading" max-height="440">
            <el-table-column label="ID" prop="_id" width="80"></el-table-column>
            <el-table-column label="题目名称" prop="title" show-overflow-tooltip></el-table-column>
            <el-table-column label="移除" width="60" align="center">
              <template slot-scope="scope">
                <el-button size="mini" type="danger" icon="el-icon-close" circle
                           @click="removeProblem(scope.row)"></el-button>
              </template>
            </el-table-column>
          </el-table>
        </el-col>

        <!-- 分割线 -->
        <el-col :span="1" style="display:flex;align-items:center;justify-content:center;min-height:440px;">
          <div style="width:1px;height:100%;background:#ebeef5;"></div>
        </el-col>

        <!-- 右侧：全部题目，多选批量加入 -->
        <el-col :span="12">
          <div class="section-label">
            从题库中选择
            <span v-if="selectedProblems.length" class="count-badge selected">已选 {{ selectedProblems.length }} 题</span>
          </div>

          <!-- 搜索栏 -->
          <div class="search-bar">
            <el-input v-model="searchKeyword" placeholder="搜索题目ID或名称" size="small"
                      prefix-icon="el-icon-search" clearable
                      @input="onSearchInput" @clear="loadAllProblems" style="flex:1"></el-input>
          </div>

          <!-- 题目多选表格 -->
          <el-table ref="problemTable"
                    :data="allProblems"
                    size="small"
                    v-loading="allLoading"
                    max-height="360"
                    @selection-change="onSelectionChange">
            <el-table-column type="selection" width="45"
                             :selectable="isSelectable"></el-table-column>
            <el-table-column label="ID" prop="_id" width="75"></el-table-column>
            <el-table-column label="题目名称" prop="title" show-overflow-tooltip></el-table-column>
            <el-table-column label="难度" width="70">
              <template slot-scope="scope">
                <el-tag :type="difficultyTag(scope.row.difficulty)" size="mini">
                  {{ difficultyLabel(scope.row.difficulty) }}
                </el-tag>
              </template>
            </el-table-column>
          </el-table>

          <!-- 分页 -->
          <div class="list-footer">
            <el-pagination small layout="prev, pager, next" :total="allTotal"
                           :page-size="allLimit" :current-page.sync="allPage"
                           @current-change="loadAllProblems"></el-pagination>
            <el-button type="primary" size="small"
                       :disabled="selectedProblems.length === 0"
                       :loading="batchAdding"
                       @click="batchAdd">
              批量加入（{{ selectedProblems.length }}）
            </el-button>
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
        batchAdding: false,
        chapterDialogVisible: false,
        problemDialogVisible: false,
        dialogMode: 'create',
        chapterForm: { title: '', description: '', order: 0 },
        currentChapter: {},
        currentProblems: [],
        detailLoading: false,
        // 右侧全量题目表格
        allProblems: [],
        allLoading: false,
        allTotal: 0,
        allPage: 1,
        allLimit: 15,
        searchKeyword: '',
        searchTimer: null,
        selectedProblems: []
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
        this.selectedProblems = []
        this.searchKeyword = ''
        this.allPage = 1
        this.problemDialogVisible = true
      },
      onProblemDialogOpen () {
        this.loadChapterDetail()
        this.loadAllProblems()
      },
      loadChapterDetail () {
        this.detailLoading = true
        api.getChapter(this.currentChapter.id).then(res => {
          this.currentProblems = res.data.data.problems || []
          this.detailLoading = false
        }).catch(() => { this.detailLoading = false })
      },
      loadAllProblems (page) {
        if (typeof page === 'number') this.allPage = page
        this.allLoading = true
        const offset = (this.allPage - 1) * this.allLimit
        api.getProblemList({
          keyword: this.searchKeyword,
          limit: this.allLimit,
          offset
        }).then(res => {
          this.allProblems = res.data.data.results || []
          this.allTotal = res.data.data.total || 0
          this.allLoading = false
        }).catch(() => { this.allLoading = false })
      },
      onSearchInput () {
        clearTimeout(this.searchTimer)
        this.allPage = 1
        this.searchTimer = setTimeout(() => this.loadAllProblems(), 400)
      },
      onSelectionChange (val) {
        this.selectedProblems = val
      },
      // 已在章节中的题目禁用勾选
      isSelectable (row) {
        const existing = new Set(this.currentProblems.map(p => p.id))
        return !existing.has(row.id)
      },

      removeProblem (problem) {
        api.removeChapterProblem(this.currentChapter.id, problem.id).then(() => {
          this.currentProblems = this.currentProblems.filter(p => p.id !== problem.id)
          this.loadChapters()
          // 刷新右侧表格的禁用状态
          this.$refs.problemTable && this.$refs.problemTable.clearSelection()
        })
      },

      batchAdd () {
        if (!this.selectedProblems.length) return
        const ids = this.selectedProblems.map(p => p.id)
        this.batchAdding = true
        api.batchAddChapterProblems(this.currentChapter.id, ids).then(res => {
          const { added, skipped } = res.data.data
          this.$success(`成功添加 ${added} 道题目${skipped ? `，${skipped} 道已跳过` : ''}`)
          this.selectedProblems = []
          this.$refs.problemTable && this.$refs.problemTable.clearSelection()
          this.loadChapterDetail()
          this.loadChapters()
        }).catch(() => {
          this.$error('批量添加失败')
        }).finally(() => { this.batchAdding = false })
      },

      difficultyTag (d) {
        return d === 'Low' ? 'success' : d === 'High' ? 'danger' : 'warning'
      },
      difficultyLabel (d) {
        return d === 'Low' ? '低' : d === 'High' ? '高' : '中'
      }
    }
  }
</script>

<style scoped lang="less">
  .section-label {
    font-weight: 600;
    color: #303133;
    margin-bottom: 10px;
    font-size: 14px;
    display: flex;
    align-items: center;
    gap: 8px;
  }

  .count-badge {
    display: inline-block;
    background: #1565c0;
    color: #fff;
    border-radius: 10px;
    padding: 1px 8px;
    font-size: 12px;
    font-weight: 500;
    &.selected { background: #67c23a; }
  }

  .search-bar {
    display: flex;
    gap: 8px;
    margin-bottom: 10px;
  }

  .list-footer {
    display: flex;
    justify-content: space-between;
    align-items: center;
    margin-top: 10px;
  }
</style>
