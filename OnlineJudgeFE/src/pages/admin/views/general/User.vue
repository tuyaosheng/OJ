<template>
  <div class="view">
    <Panel :title="$t('m.User_User') ">
      <div slot="header">
        <el-row :gutter="10" type="flex" align="middle">
          <el-col :span="6">
            <el-input v-model="keyword" prefix-icon="el-icon-search" placeholder="搜索用户名/姓名" size="small"></el-input>
          </el-col>
          <el-col :span="4">
            <el-select v-model="filterIdentity" size="small" placeholder="身份筛选" clearable style="width:100%;"
                       @change="getUserList(1)">
              <el-option label="全部" value=""></el-option>
              <el-option label="学生" value="student"></el-option>
              <el-option label="教师" value="teacher"></el-option>
            </el-select>
          </el-col>
          <el-col :span="4">
            <el-select v-model="filterGrade" size="small" placeholder="年级筛选" clearable style="width:100%;"
                       @change="getUserList(1)">
              <el-option label="全部年级" value=""></el-option>
              <el-option v-for="g in [1,2,3,4,5,6,7,8]" :key="g" :label="`${g}年级`" :value="g"></el-option>
            </el-select>
          </el-col>
          <el-col :span="10" style="display:flex;gap:8px;justify-content:flex-end;">
            <el-button type="success" size="small" icon="el-icon-plus" @click="teacherDialogVisible=true">
              新建教师账号
            </el-button>
            <el-button type="warning" size="small" icon="el-icon-upload2" @click="upgradeDialogVisible=true">
              批量升年级
            </el-button>
            <el-button v-if="selectedUsers.length" type="danger" size="small"
                       icon="el-icon-delete" @click="deleteUsers(selectedUserIDs)">
              删除选中
            </el-button>
          </el-col>
        </el-row>
      </div>
      <el-table
        v-loading="loadingTable"
        element-loading-text="loading"
        @selection-change="handleSelectionChange"
        ref="table"
        :data="userList"
        style="width: 100%">
        <el-table-column type="selection" width="55"></el-table-column>

        <el-table-column prop="id" label="ID"></el-table-column>

        <el-table-column prop="username" label="Username"></el-table-column>

        <el-table-column prop="create_time" label="Create Time">
          <template slot-scope="scope">
            {{scope.row.create_time | localtime }}
          </template>
        </el-table-column>

        <el-table-column prop="last_login" label="Last Login">
          <template slot-scope="scope">
            {{scope.row.last_login | localtime }}
          </template>
        </el-table-column>

        <el-table-column prop="real_name" label="姓名"></el-table-column>
        <el-table-column label="身份" width="75" align="center">
          <template slot-scope="scope">
            <el-tag v-if="scope.row.identity==='student'" type="primary" size="mini" effect="plain">学生</el-tag>
            <el-tag v-else-if="scope.row.identity==='teacher'" type="success" size="mini" effect="plain">教师</el-tag>
            <el-tag v-else type="info" size="mini" effect="plain">—</el-tag>
          </template>
        </el-table-column>
        <el-table-column label="年级" width="65" prop="grade" align="center">
          <template slot-scope="scope">{{ scope.row.grade ? scope.row.grade + '年级' : '—' }}</template>
        </el-table-column>
        <el-table-column label="班级" min-width="120" prop="class_name" show-overflow-tooltip>
          <template slot-scope="scope">{{ scope.row.class_name || '—' }}</template>
        </el-table-column>
        <el-table-column fixed="right" label="操作" width="200">
          <template slot-scope="{row}">
            <icon-btn name="Edit" icon="edit" @click.native="openUserDialog(row.id)"></icon-btn>
            <icon-btn name="Delete" icon="trash" @click.native="deleteUsers([row.id])"></icon-btn>
          </template>
        </el-table-column>
      </el-table>
      <div class="panel-options">
        <el-pagination
          class="page"
          layout="prev, pager, next"
          @current-change="currentChange"
          :page-size="pageSize"
          :total="total">
        </el-pagination>
      </div>
    </Panel>

    <Panel>
      <span slot="title">{{$t('m.Import_User')}}
        <el-popover placement="right" trigger="hover">
          <p>Only support csv file without headers, check the <a
            href="http://docs.onlinejudge.me/#/onlinejudge/guide/import_users">link</a> for details</p>
          <i slot="reference" class="el-icon-fa-question-circle import-user-icon"></i>
        </el-popover>
      </span>
      <el-upload v-if="!uploadUsers.length"
                 action=""
                 :show-file-list="false"
                 accept=".csv"
                 :before-upload="handleUsersCSV">
        <el-button size="small" icon="el-icon-fa-upload" type="primary">Choose File</el-button>
      </el-upload>
      <template v-else>
        <el-table :data="uploadUsersPage">
          <el-table-column label="Username">
            <template slot-scope="{row}">
              {{row[0]}}
            </template>
          </el-table-column>
          <el-table-column label="Password">
            <template slot-scope="{row}">
              {{row[1]}}
            </template>
          </el-table-column>
          <el-table-column label="Email">
            <template slot-scope="{row}">
              {{row[2]}}
            </template>
          </el-table-column>
          <el-table-column label="RealName">
            <template slot-scope="{row}">
              {{row[3]}}
            </template>
          </el-table-column>
        </el-table>
        <div class="panel-options">
          <el-button type="primary" size="small"
                     icon="el-icon-fa-upload"
                     @click="handleUsersUpload">Import All
          </el-button>
          <el-button type="warning" size="small"
                     icon="el-icon-fa-undo"
                     @click="handleResetData">Reset Data
          </el-button>
          <el-pagination
            class="page"
            layout="prev, pager, next"
            :page-size="uploadUsersPageSize"
            :current-page.sync="uploadUsersCurrentPage"
            :total="uploadUsers.length">
          </el-pagination>
        </div>
      </template>
    </Panel>

    <Panel :title="$t('m.Generate_User')">
      <el-form :model="formGenerateUser" ref="formGenerateUser">
        <el-row type="flex" justify="space-between">
          <el-col :span="4">
            <el-form-item label="Prefix" prop="prefix">
              <el-input v-model="formGenerateUser.prefix" placeholder="Prefix"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="4">
            <el-form-item label="Suffix" prop="suffix">
              <el-input v-model="formGenerateUser.suffix" placeholder="Suffix"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="4">
            <el-form-item label="Start Number" prop="number_from" required>
              <el-input-number v-model="formGenerateUser.number_from" style="width: 100%"></el-input-number>
            </el-form-item>
          </el-col>
          <el-col :span="4">
            <el-form-item label="End Number" prop="number_to" required>
              <el-input-number v-model="formGenerateUser.number_to" style="width: 100%"></el-input-number>
            </el-form-item>
          </el-col>
          <el-col :span="4">
            <el-form-item label="Password Length" prop="password_length" required>
              <el-input v-model="formGenerateUser.password_length"
                        placeholder="Password Length"></el-input>
            </el-form-item>
          </el-col>
        </el-row>

        <el-form-item>
          <el-button type="primary" @click="generateUser" icon="el-icon-fa-users" :loading="loadingGenerate">Generate & Export
          </el-button>
          <span class="userPreview" v-if="formGenerateUser.number_from && formGenerateUser.number_to &&
                                          formGenerateUser.number_from <= formGenerateUser.number_to">
            The usernames will be {{formGenerateUser.prefix + formGenerateUser.number_from + formGenerateUser.suffix}},
            <span v-if="formGenerateUser.number_from + 1 < formGenerateUser.number_to">
              {{formGenerateUser.prefix + (formGenerateUser.number_from + 1) + formGenerateUser.suffix + '...'}}
            </span>
            <span v-if="formGenerateUser.number_from + 1 <= formGenerateUser.number_to">
              {{formGenerateUser.prefix + formGenerateUser.number_to + formGenerateUser.suffix}}
            </span>
          </span>
        </el-form-item>
      </el-form>
    </Panel>
    <!-- 新建教师对话框 -->
    <el-dialog title="新建教师账号" :visible.sync="teacherDialogVisible" width="440px" :close-on-click-modal="false">
      <el-form :model="teacherForm" label-width="90px">
        <el-form-item label="用户名" required>
          <el-input v-model="teacherForm.username" placeholder="请输入用户名"></el-input>
        </el-form-item>
        <el-form-item label="姓名">
          <el-input v-model="teacherForm.real_name" placeholder="真实姓名（选填）"></el-input>
        </el-form-item>
        <el-form-item label="初始密码" required>
          <el-input v-model="teacherForm.password" type="password" placeholder="至少6位"></el-input>
        </el-form-item>
        <el-form-item label="邮箱">
          <el-input v-model="teacherForm.email" placeholder="邮箱（选填）"></el-input>
        </el-form-item>
      </el-form>
      <div slot="footer">
        <el-button @click="teacherDialogVisible=false">取消</el-button>
        <el-button type="primary" @click="createTeacher" :loading="creatingTeacher">创建</el-button>
      </div>
    </el-dialog>

    <!-- 批量升年级对话框 -->
    <el-dialog title="批量升年级" :visible.sync="upgradeDialogVisible" width="440px" :close-on-click-modal="false">
      <div style="font-size:14px;color:#606266;line-height:1.8;margin-bottom:16px;">
        此操作将所有在校学生的年级 <b>+1</b>，每学年执行一次。<br/>
        可设置最高年级，超过后该学生账号将被自动禁用（视为毕业）。
      </div>
      <el-form label-width="100px">
        <el-form-item label="最高年级">
          <el-input-number v-model="maxGrade" :min="0" :max="8" placeholder="0=不限制" style="width:160px;"></el-input-number>
          <span style="color:#909399;font-size:12px;margin-left:10px;">0 = 不自动禁用</span>
        </el-form-item>
      </el-form>
      <div slot="footer">
        <el-button @click="upgradeDialogVisible=false">取消</el-button>
        <el-button type="warning" @click="batchUpgrade" :loading="upgrading">执行升年级</el-button>
      </div>
    </el-dialog>

    <!--对话框-->
    <el-dialog :title="$t('m.User_Info')" :visible.sync="showUserDialog" :close-on-click-modal="false">
      <el-form :model="user" label-width="120px" label-position="left">
        <el-row :gutter="20">
          <el-col :span="12">
            <el-form-item :label="$t('m.User_Username')" required>
              <el-input v-model="user.username"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="$t('m.User_Real_Name')" required>
              <el-input v-model="user.real_name"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="$t('m.User_Email')" required>
              <el-input v-model="user.email"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="$t('m.User_New_Password')">
              <el-input v-model="user.password"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="$t('m.User_Type')">
              <el-select v-model="user.admin_type">
                <el-option label="Regular User" value="Regular User"></el-option>
                <el-option label="Admin" value="Admin"></el-option>
                <el-option label="Super Admin" value="Super Admin"></el-option>
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="$t('m.Problem_Permission')">
              <el-select v-model="user.problem_permission" :disabled="user.admin_type!=='Admin'">
                <el-option label="None" value="None"></el-option>
                <el-option label="Own" value="Own"></el-option>
                <el-option label="All" value="All"></el-option>
              </el-select>
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item :label="$t('m.Two_Factor_Auth')">
              <el-switch
                v-model="user.two_factor_auth"
                :disabled="!user.real_tfa"
                active-color="#13ce66"
                inactive-color="#ff4949">
              </el-switch>
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item label="Open Api">
              <el-switch
                v-model="user.open_api"
                active-color="#13ce66"
                inactive-color="#ff4949">
              </el-switch>
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item :label="$t('m.Is_Disabled')">
              <el-switch
                v-model="user.is_disabled">
              </el-switch>
            </el-form-item>
          </el-col>
        </el-row>
      </el-form>
      <span slot="footer" class="dialog-footer">
        <cancel @click.native="showUserDialog = false">Cancel</cancel>
        <save @click.native="saveUser()"></save>
      </span>
    </el-dialog>
  </div>
</template>

<script>
  import papa from 'papaparse'
  import api from '../../api.js'
  import utils from '@/utils/utils'

  export default {
    name: 'User',
    data () {
      return {
        // 一页显示的用户数
        pageSize: 10,
        // 用户总数
        total: 0,
        // 用户列表
        userList: [],
        uploadUsers: [],
        uploadUsersPage: [],
        uploadUsersCurrentPage: 1,
        uploadUsersPageSize: 15,
        // 搜索关键字
        keyword: '',
        // 是否显示用户对话框
        showUserDialog: false,
        // 当前用户model
        user: {},
        loadingTable: false,
        loadingGenerate: false,
        // 当前页码
        currentPage: 0,
        selectedUsers: [],
        filterIdentity: '',
        filterGrade: '',
        teacherDialogVisible: false,
        creatingTeacher: false,
        teacherForm: { username: '', real_name: '', password: '', email: '' },
        upgradeDialogVisible: false,
        upgrading: false,
        maxGrade: 0,
        formGenerateUser: {
          prefix: '',
          suffix: '',
          number_from: 0,
          number_to: 0,
          password_length: 8
        }
      }
    },
    mounted () {
      this.getUserList(1)
    },
    methods: {
      // 切换页码回调
      currentChange (page) {
        this.currentPage = page
        this.getUserList(page)
      },
      // 提交修改用户的信息
      saveUser () {
        api.editUser(this.user).then(res => {
          // 更新列表
          this.getUserList(this.currentPage)
        }).then(() => {
          this.showUserDialog = false
        }).catch(() => {
        })
      },
      // 打开用户对话框
      openUserDialog (id) {
        this.showUserDialog = true
        api.getUser(id).then(res => {
          this.user = res.data.data
          this.user.password = ''
          this.user.real_tfa = this.user.two_factor_auth
        })
      },
      // 获取用户列表
      getUserList (page) {
        this.loadingTable = true
        api.getUserList((page - 1) * this.pageSize, this.pageSize, this.keyword).then(res => {
          this.loadingTable = false
          this.total = res.data.data.total
          let list = res.data.data.results
          if (this.filterIdentity) {
            list = list.filter(u => u.identity === this.filterIdentity)
          }
          if (this.filterGrade) {
            list = list.filter(u => u.grade === this.filterGrade)
          }
          this.userList = list
        }, res => {
          this.loadingTable = false
        })
      },
      createTeacher () {
        if (!this.teacherForm.username || !this.teacherForm.password) {
          this.$error('用户名和密码为必填项')
          return
        }
        this.creatingTeacher = true
        api.createTeacher(this.teacherForm).then(() => {
          this.$success('教师账号创建成功')
          this.teacherDialogVisible = false
          this.teacherForm = { username: '', real_name: '', password: '', email: '' }
          this.getUserList(1)
        }).catch(() => {
          this.$error('创建失败，用户名可能已存在')
        }).finally(() => { this.creatingTeacher = false })
      },
      batchUpgrade () {
        this.$confirm(
          `确定执行批量升年级？所有在校学生年级+1${this.maxGrade ? `，达到${this.maxGrade}年级的学生将被禁用` : ''}。`,
          '确认操作', { type: 'warning' }
        ).then(() => {
          this.upgrading = true
          api.batchUpgradeGrade(this.maxGrade || null).then(res => {
            const { upgraded, graduated } = res.data.data
            this.$success(`升年级完成：${upgraded}人升级，${graduated}人已禁用（毕业）`)
            this.upgradeDialogVisible = false
            this.getUserList(1)
          }).catch(() => {
            this.$error('操作失败')
          }).finally(() => { this.upgrading = false })
        }).catch(() => {})
      },
      deleteUsers (ids) {
        this.$confirm('Sure to delete the user? The associated resources created by this user will be deleted as well, like problem, contest, announcement, etc.', 'confirm', {
          type: 'warning'
        }).then(() => {
          api.deleteUsers(ids.join(',')).then(res => {
            this.getUserList(this.currentPage)
          }).catch(() => {
            this.getUserList(this.currentPage)
          })
        }, () => {
        })
      },
      handleSelectionChange (val) {
        this.selectedUsers = val
      },
      generateUser () {
        this.$refs['formGenerateUser'].validate((valid) => {
          if (!valid) {
            this.$error('Please validate the error fields')
            return
          }
          this.loadingGenerate = true
          let data = Object.assign({}, this.formGenerateUser)
          api.generateUser(data).then(res => {
            this.loadingGenerate = false
            let url = '/admin/generate_user?file_id=' + res.data.data.file_id
            utils.downloadFile(url).then(() => {
              this.$alert('All users created successfully, the users sheets have downloaded to your disk.', 'Notice')
            })
            this.getUserList(1)
          }).catch(() => {
            this.loadingGenerate = false
          })
        })
      },
      handleUsersCSV (file) {
        papa.parse(file, {
          complete: (results) => {
            let data = results.data.filter(user => {
              return user[0] && user[1] && user[2] && user[3]
            })
            let delta = results.data.length - data.length
            if (delta > 0) {
              this.$warning(delta + ' users have been filtered due to empty value')
            }
            this.uploadUsersCurrentPage = 1
            this.uploadUsers = data
            this.uploadUsersPage = data.slice(0, this.uploadUsersPageSize)
          },
          error: (error) => {
            this.$error(error)
          }
        })
      },
      handleUsersUpload () {
        api.importUsers(this.uploadUsers).then(res => {
          this.getUserList(1)
          this.handleResetData()
        }).catch(() => {
        })
      },
      handleResetData () {
        this.uploadUsers = []
      }
    },
    computed: {
      selectedUserIDs () {
        let ids = []
        for (let user of this.selectedUsers) {
          ids.push(user.id)
        }
        return ids
      }
    },
    watch: {
      'keyword' () {
        this.currentChange(1)
      },
      'user.admin_type' () {
        if (this.user.admin_type === 'Super Admin') {
          this.user.problem_permission = 'All'
        } else if (this.user.admin_type === 'Regular User') {
          this.user.problem_permission = 'None'
        }
      },
      'uploadUsersCurrentPage' (page) {
        this.uploadUsersPage = this.uploadUsers.slice((page - 1) * this.uploadUsersPageSize, page * this.uploadUsersPageSize)
      }
    }
  }
</script>

<style scoped lang="less">
  .import-user-icon {
    color: #555555;
    margin-left: 4px;
  }

  .userPreview {
    padding-left: 10px;
  }

  .notification {
    p {
      margin: 0;
      text-align: left;
    }
  }
</style>
