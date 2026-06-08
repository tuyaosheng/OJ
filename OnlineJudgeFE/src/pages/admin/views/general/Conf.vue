<template>
  <div class="view">
    <Panel :title="$t('m.SMTP_Config')">
      <el-form label-position="left" label-width="70px" :model="smtp">
        <el-row :gutter="20">
          <el-col :span="12">
            <el-form-item :label="$t('m.Server')" required>
              <el-input v-model="smtp.server" placeholder="SMTP Server Address"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="$t('m.Port')" required>
              <el-input type="number" v-model="smtp.port" placeholder="SMTP Server Port"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="$t('m.Email')" required>
              <el-input v-model="smtp.email" placeholder="Account Used To Send Email"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="12">
            <el-form-item :label="$t('m.Password')" label-width="90px" required>
              <el-input v-model="smtp.password" type="password" placeholder="SMTP Server Password"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="24">
            <el-form-item label="TLS">
              <el-switch
                v-model="smtp.tls">
              </el-switch>
            </el-form-item>
          </el-col>
        </el-row>
      </el-form>
      <el-button type="primary" @click="saveSMTPConfig">Save</el-button>
      <el-button type="warning" @click="testSMTPConfig"
                 v-if="saved" :loading="loadingBtnTest">Send Test Email</el-button>
    </Panel>

    <Panel title="网站 Logo">
      <el-form label-position="left" label-width="100px">
        <el-form-item label="当前 Logo">
          <div v-if="websiteConfig.website_logo" style="margin-bottom:10px;">
            <img :src="websiteConfig.website_logo" style="max-height:60px;max-width:200px;border-radius:4px;border:1px solid #eee;padding:4px;background:#fff;" />
            <el-button type="danger" size="small" icon="el-icon-delete" style="margin-left:12px;" @click="deleteLogo" :loading="logoBtnLoading">删除 Logo</el-button>
          </div>
          <div v-else style="color:#909399;font-size:13px;margin-bottom:10px;">暂未设置 Logo</div>
          <el-upload
            action="/api/admin/website/logo"
            name="file"
            :show-file-list="false"
            accept="image/jpeg,image/png,image/gif,image/webp,image/svg+xml"
            :on-success="onLogoUploaded"
            :on-error="onLogoError"
            :before-upload="beforeLogoUpload">
            <el-button size="small" type="primary" icon="el-icon-upload">{{ websiteConfig.website_logo ? '更换 Logo' : '上传 Logo' }}</el-button>
            <span slot="tip" style="margin-left:10px;color:#909399;font-size:12px;">支持 jpg/png/gif/webp/svg，最大 5MB；同时作为浏览器标签图标</span>
          </el-upload>
        </el-form-item>
      </el-form>
    </Panel>

    <Panel :title="$t('m.Website_Config')">
      <el-form label-position="left" label-width="100px" ref="form" :model="websiteConfig">
        <el-row :gutter="20">
          <el-col :span="8">
            <el-form-item :label="$t('m.Base_Url')" required>
              <el-input v-model="websiteConfig.website_base_url" placeholder="Website Base Url"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item :label="$t('m.Name')" required>
              <el-input v-model="websiteConfig.website_name" placeholder="Website Name"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="8">
            <el-form-item :label="$t('m.Shortcut')" required>
              <el-input v-model="websiteConfig.website_name_shortcut" placeholder="Website Name Shortcut"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="24">
            <el-form-item :label="$t('m.Footer')" required>
              <el-input type="textarea" :autosize="{ minRows: 2, maxRows: 4}" v-model="websiteConfig.website_footer"
                        placeholder="Website Footer HTML"></el-input>
            </el-form-item>
          </el-col>
          <el-col :span="24">
            <el-col :span="12">
              <el-form-item :label="$t('m.Allow_Register')" label-width="200px">
                <el-switch
                  v-model="websiteConfig.allow_register"
                  active-color="#13ce66"
                  inactive-color="#ff4949">
                </el-switch>
              </el-form-item>
            </el-col>
            <el-col :span="12">
              <el-form-item :label="$t('m.Submission_List_Show_All')" label-width="200px">
                <el-switch
                  v-model="websiteConfig.submission_list_show_all"
                  active-color="#13ce66"
                  inactive-color="#ff4949">
                </el-switch>
              </el-form-item>
            </el-col>
          </el-col>
        </el-row>
      </el-form>
      <save @click.native="saveWebsiteConfig"></save>
    </Panel>
  </div>
</template>

<script>
  import api from '../../api.js'

  export default {
    name: 'Conf',
    data () {
      return {
        init: false,
        saved: false,
        loadingBtnTest: false,
        logoBtnLoading: false,
        smtp: {
          server: 'smtp.example.com',
          port: 25,
          password: '',
          email: 'email@example.com',
          tls: true
        },
        websiteConfig: {}
      }
    },
    mounted () {
      api.getSMTPConfig().then(res => {
        if (res.data.data) {
          this.smtp = res.data.data
        } else {
          this.init = true
          this.$warning('Please setup SMTP config at first')
        }
      })
      api.getWebsiteConfig().then(res => {
        this.websiteConfig = res.data.data
      }).catch(() => {
      })
    },
    methods: {
      saveSMTPConfig () {
        if (!this.init) {
          api.editSMTPConfig(this.smtp).then(() => {
            this.saved = true
          }, () => {
          })
        } else {
          api.createSMTPConfig(this.smtp).then(() => {
            this.saved = true
          }, () => {
          })
        }
      },
      testSMTPConfig () {
        this.$prompt('Please input your email', '', {
          inputPattern: /[\w!#$%&'*+/=?^_`{|}~-]+(?:\.[\w!#$%&'*+/=?^_`{|}~-]+)*@(?:[\w](?:[\w-]*[\w])?\.)+[\w](?:[\w-]*[\w])?/,
          inputErrorMessage: 'Error email format'
        }).then(({value}) => {
          this.loadingBtnTest = true
          api.testSMTPConfig(value).then(() => {
            this.loadingBtnTest = false
          }, () => {
            this.loadingBtnTest = false
          })
        }).catch(() => {
        })
      },
      saveWebsiteConfig () {
        api.editWebsiteConfig(this.websiteConfig).then(() => {
        }).catch(() => {
        })
      },
      beforeLogoUpload (file) {
        const allowed = ['image/jpeg', 'image/png', 'image/gif', 'image/webp', 'image/svg+xml']
        if (!allowed.includes(file.type)) {
          this.$error('仅支持 jpg/png/gif/webp/svg 格式')
          return false
        }
        if (file.size > 5 * 1024 * 1024) {
          this.$error('图片大小不能超过 5MB')
          return false
        }
        return true
      },
      onLogoUploaded (response) {
        if (response.error) {
          this.$error('上传失败：' + response.data)
        } else {
          this.$set(this.websiteConfig, 'website_logo', response.data.logo)
          this.$success('Logo 上传成功')
        }
      },
      onLogoError () {
        this.$error('Logo 上传失败，请重试')
      },
      deleteLogo () {
        this.$confirm('确定删除网站 Logo 吗？', '提示', { type: 'warning' }).then(() => {
          this.logoBtnLoading = true
          api.deleteSiteLogo().then(() => {
            this.$set(this.websiteConfig, 'website_logo', '')
            this.$success('Logo 已删除')
          }).catch(() => {
            this.$error('删除失败')
          }).finally(() => {
            this.logoBtnLoading = false
          })
        }).catch(() => {})
      }
    }
  }
</script>
