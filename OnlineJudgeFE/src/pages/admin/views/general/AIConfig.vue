<template>
  <div>
    <panel title="AI 诊断配置">
      <el-form ref="form" :model="config" label-width="160px" style="max-width:680px;">
        <el-form-item label="启用 AI 诊断">
          <el-switch v-model="config.enabled"></el-switch>
          <span class="form-hint">关闭后学生端不显示 AI 诊断按钮</span>
        </el-form-item>
        <el-form-item label="每人每日次数上限">
          <el-input-number v-model="config.daily_limit" :min="0" :max="999"></el-input-number>
          <span class="form-hint">管理员/教师账号不受此限制</span>
        </el-form-item>
        <el-form-item label="开放诊断的判题状态">
          <el-checkbox-group v-model="selectedGroups">
            <el-checkbox v-for="g in statusGroups" :key="g.key" :label="g.key">{{g.label}}</el-checkbox>
          </el-checkbox-group>
          <span class="form-hint">不勾选的状态学生端不显示按钮（建议关闭"编译错误"，避免在语法错误上浪费 token）</span>
        </el-form-item>
        <el-form-item label="API Base">
          <el-input v-model="config.api_base" placeholder="如 https://api.deepseek.com/v1"></el-input>
          <span class="form-hint">OpenAI 兼容接口地址，结尾不含 /chat/completions</span>
        </el-form-item>
        <el-form-item label="模型名称">
          <el-input v-model="config.model" placeholder="如 deepseek-chat / qwen-plus / gpt-4o-mini"></el-input>
        </el-form-item>
        <el-form-item label="API Key">
          <el-input v-model="config.api_key" type="password"
                    :placeholder="config.api_key_set ? '已配置，留空表示不修改' : '请输入 API Key'"></el-input>
        </el-form-item>
        <el-form-item>
          <el-button type="primary" :loading="loading" @click="save">保存</el-button>
        </el-form-item>
      </el-form>
    </panel>
  </div>
</template>

<script>
  import api from '../../api'

  const STATUS_GROUPS = [
    { key: 'ce', label: '编译错误', codes: [-2] },
    { key: 'wa', label: '答案错误', codes: [-1] },
    { key: 'tle', label: '运行超时', codes: [1, 2] },
    { key: 'mle', label: '内存超限', codes: [3] },
    { key: 're', label: '运行错误', codes: [4] },
    { key: 'pa', label: '部分正确', codes: [8] }
  ]

  export default {
    name: 'AIConfig',
    data () {
      return {
        loading: false,
        statusGroups: STATUS_GROUPS,
        selectedGroups: [],
        config: {
          enabled: false,
          daily_limit: 5,
          api_base: '',
          model: '',
          api_key: '',
          api_key_set: false
        }
      }
    },
    mounted () {
      this.getConfig()
    },
    methods: {
      getConfig () {
        api.getAIDiagnosisConfig().then(res => {
          let d = res.data.data
          this.config = {
            enabled: d.enabled,
            daily_limit: d.daily_limit,
            api_base: d.api_base,
            model: d.model,
            api_key: '',
            api_key_set: d.api_key_set
          }
          let allowed = d.allowed_results || []
          // 某状态组只要有任一 code 在允许列表里，就视为勾选
          this.selectedGroups = STATUS_GROUPS
            .filter(g => g.codes.some(c => allowed.includes(c)))
            .map(g => g.key)
        })
      },
      save () {
        this.loading = true
        // 把勾选的状态组展开成判题结果码列表
        let allowedResults = []
        STATUS_GROUPS.forEach(g => {
          if (this.selectedGroups.includes(g.key)) {
            allowedResults = allowedResults.concat(g.codes)
          }
        })
        let data = {
          enabled: this.config.enabled,
          daily_limit: this.config.daily_limit,
          api_base: this.config.api_base,
          model: this.config.model,
          allowed_results: allowedResults
        }
        if (this.config.api_key) {
          data.api_key = this.config.api_key
        }
        api.setAIDiagnosisConfig(data).then(() => {
          this.loading = false
          this.getConfig()
        }, () => {
          this.loading = false
        })
      }
    }
  }
</script>

<style scoped lang="less">
  .form-hint {
    margin-left: 12px;
    font-size: 12px;
    color: #999;
  }
</style>
