<template>
  <div>
    <panel title="章节管理">
      <div slot="header">
        <el-button type="primary" size="small" icon="el-icon-plus" @click="openCreateDialog">新建章节</el-button>
      </div>

      <el-table :data="chapters" v-loading="loading" row-key="id"
                :header-cell-style="{background:'#f5f7fa',color:'#606266',fontWeight:'600'}">
        <el-table-column label="排序" width="65" prop="order" align="center"></el-table-column>
        <el-table-column label="章节名称" prop="title" min-width="180">
          <template slot-scope="scope">
            <span style="font-weight:600;color:#1a237e;">{{ scope.row.title }}</span>
          </template>
        </el-table-column>
        <el-table-column label="描述" prop="description" min-width="220" show-overflow-tooltip>
          <template slot-scope="scope">
            <span style="color:#606266;">{{ scope.row.description || '—' }}</span>
          </template>
        </el-table-column>
        <el-table-column label="题目数" width="75" prop="problem_count" align="center">
          <template slot-scope="scope">
            <el-tag type="primary" size="mini">{{ scope.row.problem_count }}</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="操作" width="160" align="center">
          <template slot-scope="scope">
            <el-tooltip content="编辑章节" placement="top">
              <el-button size="mini" icon="el-icon-edit" circle type="primary"
                         @click="openEditDialog(scope.row)"></el-button>
            </el-tooltip>
            <el-tooltip content="管理题目" placement="top">
              <el-button size="mini" icon="el-icon-s-grid" circle type="success"
                         @click="openProblemDialog(scope.row)" style="margin:0 8px;"></el-button>
            </el-tooltip>
            <el-tooltip content="删除章节" placement="top">
              <el-button size="mini" icon="el-icon-delete" circle type="danger"
                         @click="deleteChapter(scope.row)"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>
    </panel>

    <!-- 新建/编辑章节对话框 -->
    <el-dialog :title="dialogMode === 'create' ? '新建章节' : '编辑章节'"
               :visible.sync="chapterDialogVisible" width="480px"
               :close-on-click-modal="false">
      <el-form :model="chapterForm" label-width="80px">
        <el-form-item label="章节名称" required>
          <el-input v-model="chapterForm.title" placeholder="请输入章节名称"
                    maxlength="100" show-word-limit></el-input>
        </el-form-item>
        <el-form-item label="描述">
          <el-input type="textarea" :rows="3" v-model="chapterForm.description"
                    placeholder="章节简介（选填）" maxlength="300" show-word-limit></el-input>
        </el-form-item>
        <el-form-item label="排序号">
          <el-input-number v-model="chapterForm.order" :min="0" :max="9999" style="width:140px;"></el-input-number>
          <span style="color:#909399;font-size:12px;margin-left:10px;">数字越小越靠前</span>
        </el-form-item>
      </el-form>
      <div slot="footer" style="display:flex;justify-content:flex-end;gap:10px;">
        <el-button @click="chapterDialogVisible = false">取消</el-button>
        <el-button type="primary" @click="saveChapter" :loading="saving">
          {{ dialogMode === 'create' ? '创建' : '保存' }}
        </el-button>
      </div>
    </el-dialog>

    <!-- 管理题目对话框 -->
    <el-dialog :title="'章节：' + currentChapter.title"
               :visible.sync="problemDialogVisible"
               width="1060px"
               top="5vh"
               :close-on-click-modal="false"
               @open="onProblemDialogOpen">
      <div class="dialog-body">
        <!-- 左侧：已加入的题目 -->
        <div class="panel-left">
          <div class="panel-header">
            <span class="panel-title">已加入本章节</span>
            <el-tag type="primary" size="small" effect="dark">{{ currentProblems.length }} 题</el-tag>
          </div>
          <el-table :data="currentProblems" size="small" v-loading="detailLoading"
                    height="440" border
                    :header-cell-style="{background:'#f5f7fa',color:'#606266'}">
            <el-table-column label="题目ID" prop="_id" width="100" show-overflow-tooltip>
              <template slot-scope="scope">
                <span style="color:#1565c0;font-weight:600;">{{ scope.row._id }}</span>
              </template>
            </el-table-column>
            <el-table-column label="题目名称" prop="title" show-overflow-tooltip></el-table-column>
            <el-table-column label="" width="46" align="center">
              <template slot-scope="scope">
                <el-tooltip content="移除" placement="top">
                  <el-button size="mini" type="danger" icon="el-icon-minus" circle
                             @click="removeProblem(scope.row)"></el-button>
                </el-tooltip>
              </template>
            </el-table-column>
          </el-table>
        </div>

        <!-- 中间箭头 -->
        <div class="panel-arrow">
          <i class="el-icon-arrow-left" style="font-size:20px;color:#c0c4cc;"></i>
          <div style="font-size:11px;color:#c0c4cc;margin-top:4px;white-space:nowrap;">点击添加</div>
        </div>

        <!-- 右侧：题库选题 -->
        <div class="panel-right">
          <div class="panel-header">
            <span class="panel-title">从题库选择</span>
            <el-tag v-if="selectedProblems.length" type="success" size="small" effect="dark">
              已选 {{ selectedProblems.length }} 题
            </el-tag>
          </div>

          <!-- 搜索 + 排序工具栏 -->
          <div class="toolbar">
            <el-input v-model="searchKeyword" placeholder="搜索题目ID或名称" size="small"
                      prefix-icon="el-icon-search" clearable style="flex:1;"
                      @input="onSearchInput" @clear="onClearSearch"></el-input>
            <el-select v-model="sortField" size="small" placeholder="排序字段"
                       style="width:110px;" @change="applySort">
              <el-option label="题目ID" value="_id"></el-option>
              <el-option label="难度" value="difficulty"></el-option>
              <el-option label="通过率" value="ac_rate"></el-option>
            </el-select>
            <el-tooltip :content="sortOrder === 'asc' ? '当前升序，点击切换降序' : '当前降序，点击切换升序'" placement="top">
              <el-button size="small" @click="toggleSortOrder"
                         :icon="sortOrder === 'asc' ? 'el-icon-sort-up' : 'el-icon-sort-down'"
                         style="padding:7px 10px;">
                {{ sortOrder === 'asc' ? '升序' : '降序' }}
              </el-button>
            </el-tooltip>
          </div>

          <!-- 多选题目表格 -->
          <el-table ref="problemTable"
                    :data="sortedProblems"
                    size="small"
                    v-loading="allLoading"
                    height="360"
                    border
                    :header-cell-style="{background:'#f5f7fa',color:'#606266'}"
                    @selection-change="onSelectionChange">
            <el-table-column type="selection" width="42" :selectable="isSelectable"></el-table-column>
            <el-table-column label="题目ID" prop="_id" width="105" show-overflow-tooltip>
              <template slot-scope="scope">
                <span :style="{color: isSelectable(scope.row) ? '#1565c0' : '#c0c4cc', fontWeight:'600'}">
                  {{ scope.row._id }}
                </span>
              </template>
            </el-table-column>
            <el-table-column label="题目名称" prop="title" show-overflow-tooltip>
              <template slot-scope="scope">
                <span :style="{color: isSelectable(scope.row) ? '#303133' : '#c0c4cc'}">
                  {{ scope.row.title }}
                </span>
              </template>
            </el-table-column>
            <el-table-column label="难度" width="62" align="center">
              <template slot-scope="scope">
                <el-tag :type="difficultyTag(scope.row.difficulty)" size="mini" effect="plain">
                  {{ difficultyLabel(scope.row.difficulty) }}
                </el-tag>
              </template>
            </el-table-column>
            <el-table-column label="通过率" width="72" align="center">
              <template slot-scope="scope">
                <span style="font-size:12px;color:#606266;">
                  {{ getACRate(scope.row.accepted_number, scope.row.submission_number) }}
                </span>
              </template>
            </el-table-column>
          </el-table>

          <!-- 分页 + 批量添加 -->
          <div class="table-footer">
            <el-pagination small layout="total, prev, pager, next"
                           :total="allTotal" :page-size="allLimit"
                           :current-page.sync="allPage"
                           @current-change="loadAllProblems"
                           style="flex:1;"></el-pagination>
            <el-button type="primary" size="small" icon="el-icon-plus"
                       :disabled="selectedProblems.length === 0"
                       :loading="batchAdding"
                       @click="batchAdd">
              批量加入（{{ selectedProblems.length }}）
            </el-button>
          </div>
        </div>
      </div>
    </el-dialog>
  </div>
</template>

<script>
  import api from '@admin/api'

  const DIFFICULTY_ORDER = { Low: 1, Mid: 2, High: 3 }

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
        allProblems: [],
        allLoading: false,
        allTotal: 0,
        allPage: 1,
        allLimit: 15,
        searchKeyword: '',
        searchTimer: null,
        selectedProblems: [],
        sortField: '_id',
        sortOrder: 'asc'
      }
    },
    computed: {
      sortedProblems () {
        const list = [...this.allProblems]
        const field = this.sortField
        const asc = this.sortOrder === 'asc'
        list.sort((a, b) => {
          let va, vb
          if (field === 'difficulty') {
            va = DIFFICULTY_ORDER[a.difficulty] || 0
            vb = DIFFICULTY_ORDER[b.difficulty] || 0
          } else if (field === 'ac_rate') {
            va = a.submission_number ? a.accepted_number / a.submission_number : 0
            vb = b.submission_number ? b.accepted_number / b.submission_number : 0
          } else {
            va = a._id || ''
            vb = b._id || ''
            return asc ? va.localeCompare(vb, undefined, {numeric: true})
                       : vb.localeCompare(va, undefined, {numeric: true})
          }
          return asc ? va - vb : vb - va
        })
        return list
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
        this.$confirm(`确定删除章节「${row.title}」吗？题目不会被删除。`, '确认删除', {type: 'warning'}).then(() => {
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
        this.sortField = '_id'
        this.sortOrder = 'asc'
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
        api.getProblemList({ keyword: this.searchKeyword, limit: this.allLimit, offset }).then(res => {
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
      onClearSearch () {
        this.searchKeyword = ''
        this.allPage = 1
        this.loadAllProblems()
      },
      applySort () {
        // 已通过computed sortedProblems自动排序，无需额外操作
      },
      toggleSortOrder () {
        this.sortOrder = this.sortOrder === 'asc' ? 'desc' : 'asc'
      },
      onSelectionChange (val) {
        this.selectedProblems = val
      },
      isSelectable (row) {
        const existing = new Set(this.currentProblems.map(p => p.id))
        return !existing.has(row.id)
      },
      removeProblem (problem) {
        api.removeChapterProblem(this.currentChapter.id, problem.id).then(() => {
          this.currentProblems = this.currentProblems.filter(p => p.id !== problem.id)
          this.loadChapters()
          this.$refs.problemTable && this.$refs.problemTable.clearSelection()
        })
      },
      batchAdd () {
        if (!this.selectedProblems.length) return
        const ids = this.selectedProblems.map(p => p.id)
        this.batchAdding = true
        api.batchAddChapterProblems(this.currentChapter.id, ids).then(res => {
          const { added, skipped } = res.data.data
          this.$success(`成功添加 ${added} 道题目${skipped ? `，${skipped} 道已在章节中跳过` : ''}`)
          this.selectedProblems = []
          this.$refs.problemTable && this.$refs.problemTable.clearSelection()
          this.loadChapterDetail()
          this.loadChapters()
        }).catch(() => {
          this.$error('批量添加失败')
        }).finally(() => { this.batchAdding = false })
      },
      getACRate (accepted, total) {
        if (!total) return '0%'
        return (accepted / total * 100).toFixed(1) + '%'
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
  .dialog-body {
    display: flex;
    gap: 0;
    align-items: flex-start;
    min-height: 500px;
  }

  .panel-left {
    flex: 0 0 320px;
    min-width: 0;
  }

  .panel-arrow {
    flex: 0 0 50px;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    padding-top: 60px;
    color: #c0c4cc;
  }

  .panel-right {
    flex: 1;
    min-width: 0;
  }

  .panel-header {
    display: flex;
    align-items: center;
    gap: 8px;
    margin-bottom: 10px;
    padding-bottom: 8px;
    border-bottom: 2px solid #e8f0fe;
  }

  .panel-title {
    font-size: 14px;
    font-weight: 600;
    color: #1a237e;
  }

  .toolbar {
    display: flex;
    gap: 8px;
    margin-bottom: 10px;
    align-items: center;
  }

  .table-footer {
    display: flex;
    align-items: center;
    justify-content: space-between;
    margin-top: 10px;
    padding-top: 8px;
    border-top: 1px solid #f0f0f0;
  }
</style>
