<template>
  <div>
    <!-- 课堂列表 -->
    <panel title="课堂管理">
      <div slot="header">
        <el-button type="primary" size="small" icon="el-icon-plus" @click="openCreate">新建课堂</el-button>
      </div>
      <el-table :data="sessions" v-loading="loading"
                :header-cell-style="{background:'#f5f7fa',color:'#606266',fontWeight:'600'}">
        <el-table-column label="课堂名称" prop="title" min-width="160">
          <template slot-scope="s">
            <span style="font-weight:600;color:#1a237e;">{{ s.row.title }}</span>
          </template>
        </el-table-column>
        <el-table-column label="绑定章节" prop="chapter_title" min-width="140" show-overflow-tooltip></el-table-column>
        <el-table-column label="开始时间" width="160">
          <template slot-scope="s">{{ formatTime(s.row.start_time) }}</template>
        </el-table-column>
        <el-table-column label="结束时间" width="160">
          <template slot-scope="s">{{ formatTime(s.row.end_time) }}</template>
        </el-table-column>
        <el-table-column label="状态" width="80" align="center">
          <template slot-scope="s">
            <el-tag :type="s.row.is_active ? 'success' : 'info'" size="small" effect="dark">
              {{ s.row.is_active ? '进行中' : '未开始/已结束' }}
            </el-tag>
          </template>
        </el-table-column>
        <el-table-column label="操作" width="200" align="center">
          <template slot-scope="s">
            <el-tooltip content="大屏看板" placement="top">
              <el-button size="mini" icon="el-icon-monitor" type="primary" circle
                         @click="openDashboard(s.row)"></el-button>
            </el-tooltip>
            <el-tooltip content="编辑" placement="top">
              <el-button size="mini" icon="el-icon-edit" type="warning" circle style="margin:0 6px;"
                         @click="openEdit(s.row)"></el-button>
            </el-tooltip>
            <el-tooltip content="删除" placement="top">
              <el-button size="mini" icon="el-icon-delete" type="danger" circle
                         @click="deleteSession(s.row)"></el-button>
            </el-tooltip>
          </template>
        </el-table-column>
      </el-table>
    </panel>

    <!-- IP 绑定管理 -->
    <panel title="IP 座位绑定">
      <div slot="header">
        <el-button type="primary" size="small" icon="el-icon-plus" @click="openIPCreate">添加绑定</el-button>
      </div>
      <el-table :data="bindings" v-loading="ipLoading" size="small"
                :header-cell-style="{background:'#f5f7fa',color:'#606266',fontWeight:'600'}">
        <el-table-column label="IP 地址" prop="ip_address" width="150"></el-table-column>
        <el-table-column label="座位/主机名" prop="hostname" width="160"></el-table-column>
        <el-table-column label="备注" prop="note" show-overflow-tooltip></el-table-column>
        <el-table-column label="更新时间" width="160">
          <template slot-scope="s">{{ formatTime(s.row.updated_at) }}</template>
        </el-table-column>
        <el-table-column label="操作" width="100" align="center">
          <template slot-scope="s">
            <el-button size="mini" icon="el-icon-edit" type="primary" circle
                       @click="openIPEdit(s.row)" style="margin-right:6px;"></el-button>
            <el-button size="mini" icon="el-icon-delete" type="danger" circle
                       @click="deleteBinding(s.row)"></el-button>
          </template>
        </el-table-column>
      </el-table>
    </panel>

    <!-- 新建/编辑课堂对话框 -->
    <el-dialog :title="sessionMode === 'create' ? '新建课堂' : '编辑课堂'"
               :visible.sync="sessionDialogVisible" width="520px" :close-on-click-modal="false">
      <el-form :model="sessionForm" label-width="90px">
        <el-form-item label="课堂名称" required>
          <el-input v-model="sessionForm.title" placeholder="如：第3讲 循环结构"></el-input>
        </el-form-item>
        <el-form-item label="绑定章节" required>
          <el-select v-model="sessionForm.chapter_id" placeholder="选择章节" style="width:100%;">
            <el-option v-for="c in chapters" :key="c.id" :label="c.title" :value="c.id"></el-option>
          </el-select>
        </el-form-item>
        <el-form-item label="开始时间" required>
          <el-date-picker v-model="sessionForm.start_time" type="datetime"
                          format="yyyy-MM-dd HH:mm" value-format="yyyy-MM-ddTHH:mm:ss"
                          placeholder="选择开始时间" style="width:100%;"></el-date-picker>
        </el-form-item>
        <el-form-item label="结束时间" required>
          <el-date-picker v-model="sessionForm.end_time" type="datetime"
                          format="yyyy-MM-dd HH:mm" value-format="yyyy-MM-ddTHH:mm:ss"
                          placeholder="选择结束时间" style="width:100%;"></el-date-picker>
        </el-form-item>
      </el-form>
      <div slot="footer">
        <el-button @click="sessionDialogVisible = false">取消</el-button>
        <el-button type="primary" @click="saveSession" :loading="saving">保存</el-button>
      </div>
    </el-dialog>

    <!-- IP 绑定对话框 -->
    <el-dialog :title="ipMode === 'create' ? '添加 IP 绑定' : '编辑 IP 绑定'"
               :visible.sync="ipDialogVisible" width="440px" :close-on-click-modal="false">
      <el-form :model="ipForm" label-width="90px">
        <el-form-item label="IP 地址" required>
          <el-input v-model="ipForm.ip_address" placeholder="如：192.168.1.10"></el-input>
        </el-form-item>
        <el-form-item label="座位/主机名" required>
          <el-input v-model="ipForm.hostname" placeholder="如：A01排第3座 / PC-023"></el-input>
        </el-form-item>
        <el-form-item label="备注">
          <el-input v-model="ipForm.note" placeholder="选填"></el-input>
        </el-form-item>
      </el-form>
      <div slot="footer">
        <el-button @click="ipDialogVisible = false">取消</el-button>
        <el-button type="primary" @click="saveBinding" :loading="ipSaving">保存</el-button>
      </div>
    </el-dialog>
  </div>
</template>

<script>
  import api from '@admin/api'

  export default {
    name: 'ClassSession',
    data () {
      return {
        sessions: [], loading: false, saving: false,
        chapters: [],
        sessionDialogVisible: false, sessionMode: 'create',
        sessionForm: { title: '', chapter_id: null, start_time: '', end_time: '' },
        bindings: [], ipLoading: false, ipSaving: false,
        ipDialogVisible: false, ipMode: 'create',
        ipForm: { ip_address: '', hostname: '', note: '' }
      }
    },
    mounted () {
      this.loadSessions()
      this.loadBindings()
      this.loadChapters()
    },
    methods: {
      loadSessions () {
        this.loading = true
        api.getClassSessions().then(r => { this.sessions = r.data.data; this.loading = false })
          .catch(() => { this.loading = false })
      },
      loadBindings () {
        this.ipLoading = true
        api.getIPBindings().then(r => { this.bindings = r.data.data; this.ipLoading = false })
          .catch(() => { this.ipLoading = false })
      },
      loadChapters () {
        api.getChapterList().then(r => { this.chapters = r.data.data })
      },
      openCreate () {
        this.sessionMode = 'create'
        this.sessionForm = { title: '', chapter_id: null, start_time: '', end_time: '' }
        this.sessionDialogVisible = true
      },
      openEdit (row) {
        this.sessionMode = 'edit'
        this.sessionForm = {
          id: row.id, title: row.title,
          chapter_id: row.chapter, start_time: row.start_time, end_time: row.end_time
        }
        this.sessionDialogVisible = true
      },
      saveSession () {
        if (!this.sessionForm.title || !this.sessionForm.chapter_id ||
            !this.sessionForm.start_time || !this.sessionForm.end_time) {
          this.$error('请填写所有必填项')
          return
        }
        this.saving = true
        const fn = this.sessionMode === 'create' ? api.createClassSession : api.editClassSession
        fn(this.sessionForm).then(() => {
          this.$success('保存成功')
          this.sessionDialogVisible = false
          this.loadSessions()
        }).catch(() => { this.$error('保存失败') }).finally(() => { this.saving = false })
      },
      deleteSession (row) {
        this.$confirm(`确定删除课堂「${row.title}」？`, '确认', { type: 'warning' }).then(() => {
          api.deleteClassSession(row.id).then(() => { this.$success('已删除'); this.loadSessions() })
        }).catch(() => {})
      },
      openDashboard (row) {
        const url = `/admin/class_dashboard?session_id=${row.id}`
        window.open(url, '_blank')
      },
      openIPCreate () {
        this.ipMode = 'create'
        this.ipForm = { ip_address: '', hostname: '', note: '' }
        this.ipDialogVisible = true
      },
      openIPEdit (row) {
        this.ipMode = 'edit'
        this.ipForm = { id: row.id, ip_address: row.ip_address, hostname: row.hostname, note: row.note || '' }
        this.ipDialogVisible = true
      },
      saveBinding () {
        if (!this.ipForm.ip_address || !this.ipForm.hostname) {
          this.$error('IP 地址和座位名称不能为空')
          return
        }
        this.ipSaving = true
        const fn = this.ipMode === 'create' ? api.saveIPBinding : api.editIPBinding
        fn(this.ipForm).then(() => {
          this.$success('保存成功')
          this.ipDialogVisible = false
          this.loadBindings()
        }).catch(() => { this.$error('保存失败') }).finally(() => { this.ipSaving = false })
      },
      deleteBinding (row) {
        this.$confirm(`确定删除 ${row.ip_address} 的绑定？`, '确认', { type: 'warning' }).then(() => {
          api.deleteIPBinding(row.id).then(() => { this.$success('已删除'); this.loadBindings() })
        }).catch(() => {})
      },
      formatTime (t) {
        if (!t) return '—'
        return new Date(t).toLocaleString('zh-CN', { hour12: false })
      }
    }
  }
</script>
