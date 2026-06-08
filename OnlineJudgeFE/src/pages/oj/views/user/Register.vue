<template>
<div>
  <Form ref="formRegister" :model="formRegister" :rules="ruleRegister">
    <FormItem prop="username">
      <Input type="text" v-model="formRegister.username"
             placeholder="用户名" size="large" @on-enter="handleRegister">
        <Icon type="ios-person-outline" slot="prepend"></Icon>
      </Input>
    </FormItem>
    <FormItem prop="grade">
      <Select v-model="formRegister.grade" placeholder="年级" size="large">
        <Option v-for="g in gradeOptions" :key="g.value" :value="g.value">{{ g.label }}</Option>
      </Select>
    </FormItem>
    <FormItem prop="class_name">
      <Input type="text" v-model="formRegister.class_name"
             placeholder="班级（如：计算机2301班）" size="large" @on-enter="handleRegister">
        <Icon type="ios-school-outline" slot="prepend"></Icon>
      </Input>
    </FormItem>
    <FormItem prop="password">
      <Input type="password" v-model="formRegister.password"
             placeholder="密码（至少6位）" size="large" @on-enter="handleRegister">
        <Icon type="ios-locked-outline" slot="prepend"></Icon>
      </Input>
    </FormItem>
    <FormItem prop="passwordAgain">
      <Input type="password" v-model="formRegister.passwordAgain"
             placeholder="再次输入密码" size="large" @on-enter="handleRegister">
        <Icon type="ios-locked-outline" slot="prepend"></Icon>
      </Input>
    </FormItem>
    <FormItem prop="captcha" style="margin-bottom:10px">
      <div class="oj-captcha">
        <div class="oj-captcha-code">
          <Input v-model="formRegister.captcha" placeholder="验证码" size="large" @on-enter="handleRegister">
            <Icon type="ios-lightbulb-outline" slot="prepend"></Icon>
          </Input>
        </div>
        <div class="oj-captcha-img">
          <Tooltip content="点击刷新" placement="top">
            <img :src="captchaSrc" @click="getCaptchaSrc"/>
          </Tooltip>
        </div>
      </div>
    </FormItem>
  </Form>
  <div class="footer">
    <Button type="primary" @click="handleRegister" class="btn" long :loading="btnRegisterLoading">
      注册
    </Button>
    <Button type="ghost" @click="switchMode('login')" class="btn" long>
      已有账号，去登录
    </Button>
  </div>
</div>
</template>

<script>
  import { mapGetters, mapActions } from 'vuex'
  import api from '@oj/api'
  import { FormMixin } from '@oj/components/mixins'

  export default {
    mixins: [FormMixin],
    mounted () {
      this.getCaptchaSrc()
    },
    data () {
      const CheckUsernameNotExist = (rule, value, callback) => {
        api.checkUsernameOrEmail(value, undefined).then(res => {
          if (res.data.data.username === true) {
            callback(new Error('该用户名已存在'))
          } else {
            callback()
          }
        }, _ => callback())
      }
      const CheckPassword = (rule, value, callback) => {
        if (this.formRegister.password !== '') {
          this.$refs.formRegister.validateField('passwordAgain')
        }
        callback()
      }
      const CheckAgainPassword = (rule, value, callback) => {
        if (value !== this.formRegister.password) {
          callback(new Error('两次密码不一致'))
        }
        callback()
      }

      return {
        btnRegisterLoading: false,
        gradeOptions: [
          { value: 1, label: '大一 / 一年级' },
          { value: 2, label: '大二 / 二年级' },
          { value: 3, label: '大三 / 三年级' },
          { value: 4, label: '大四 / 四年级' },
          { value: 5, label: '五年级' },
          { value: 6, label: '六年级' },
          { value: 7, label: '七年级' },
          { value: 8, label: '八年级' }
        ],
        formRegister: {
          username: '',
          password: '',
          passwordAgain: '',
          grade: '',
          class_name: '',
          captcha: ''
        },
        ruleRegister: {
          username: [
            { required: true, message: '请输入用户名', trigger: 'blur' },
            { validator: CheckUsernameNotExist, trigger: 'blur' }
          ],
          grade: [
            { required: true, type: 'number', message: '请选择年级', trigger: 'change' }
          ],
          class_name: [
            { required: true, message: '请输入班级名称', trigger: 'blur' }
          ],
          password: [
            { required: true, trigger: 'blur', min: 6, max: 20 },
            { validator: CheckPassword, trigger: 'blur' }
          ],
          passwordAgain: [
            { required: true, validator: CheckAgainPassword, trigger: 'change' }
          ],
          captcha: [
            { required: true, trigger: 'blur', min: 1, max: 10 }
          ]
        }
      }
    },
    methods: {
      ...mapActions(['changeModalStatus', 'getProfile']),
      switchMode (mode) {
        this.changeModalStatus({ mode, visible: true })
      },
      handleRegister () {
        this.validateForm('formRegister').then(valid => {
          let formData = Object.assign({}, this.formRegister)
          delete formData['passwordAgain']
          this.btnRegisterLoading = true
          api.register(formData).then(res => {
            this.$success('注册成功，请登录')
            this.switchMode('login')
            this.btnRegisterLoading = false
          }, _ => {
            this.getCaptchaSrc()
            this.formRegister.captcha = ''
            this.btnRegisterLoading = false
          })
        })
      }
    },
    computed: {
      ...mapGetters(['website', 'modalStatus'])
    }
  }
</script>

<style scoped lang="less">
  .footer {
    overflow: auto;
    margin-top: 20px;
    margin-bottom: -15px;
    text-align: left;
    .btn {
      margin: 0 0 15px 0;
      &:last-child { margin: 0; }
    }
  }
</style>
