<template>
  <el-menu class="vertical_menu"
           :router="true" :default-active="currentPath">
    <div class="logo">
      <img src="../../../assets/logo.svg" alt="oj admin"/>
    </div>
    <el-menu-item index="/"><i class="el-icon-fa-dashboard"></i>{{$t('m.Dashboard')}}</el-menu-item>
    <el-submenu v-if="isSuperAdmin" index="general">
      <template slot="title"><i class="el-icon-menu"></i>{{$t('m.General')}}</template>
      <el-menu-item index="/user">{{$t('m.User')}}</el-menu-item>
      <el-menu-item index="/announcement">{{$t('m.Announcement')}}</el-menu-item>
      <el-menu-item index="/conf">{{$t('m.System_Config')}}</el-menu-item>
      <el-menu-item index="/judge-server">{{$t('m.Judge_Server')}}</el-menu-item>
      <el-menu-item index="/prune-test-case">{{$t('m.Prune_Test_Case')}}</el-menu-item>
      <el-menu-item index="/ai/config">AI 诊断配置</el-menu-item>
    </el-submenu>
    <el-submenu index="problem" v-if="hasProblemPermission">
      <template slot="title"><i class="el-icon-fa-bars"></i>{{$t('m.Problem')}}</template>
      <el-menu-item index="/problems">{{$t('m.Problem_List')}}</el-menu-item>
      <el-menu-item index="/problem/create">{{$t('m.Create_Problem')}}</el-menu-item>
      <el-menu-item index="/problem/batch_ops">{{$t('m.Export_Import_Problem')}}</el-menu-item>
      <el-menu-item index="/problem/chapters">章节管理</el-menu-item>
      <el-menu-item index="/problem/class_session">课堂管理</el-menu-item>
      <el-menu-item index="/ai/diagnosis">AI 诊断记录</el-menu-item>

    </el-submenu>
    <el-submenu index="contest">
      <template slot="title"><i class="el-icon-fa-trophy"></i>{{$t('m.Contest')}}</template>
      <el-menu-item index="/contest">{{$t('m.Contest_List')}}</el-menu-item>
      <el-menu-item index="/contest/create">{{$t('m.Create_Contest')}}</el-menu-item>
    </el-submenu>
  </el-menu>
</template>

<script>
  import {mapGetters} from 'vuex'

  export default {
    name: 'SideMenu',
    data () {
      return {
        currentPath: ''
      }
    },
    mounted () {
      this.currentPath = this.$route.path
    },
    computed: {
      ...mapGetters(['user', 'isSuperAdmin', 'hasProblemPermission'])
    }
  }
</script>

<style scoped lang="less">
  .vertical_menu {
    overflow: auto;
    width: 215px;
    height: 100%;
    position: fixed !important;
    z-index: 100;
    top: 0;
    bottom: 0;
    left: 0;
    background: linear-gradient(180deg, #1a237e 0%, #1565c0 100%) !important;
    border-right: none !important;
    box-shadow: 2px 0 8px rgba(0,0,0,0.15);

    .logo {
      margin: 24px 0 16px;
      text-align: center;
      img {
        background-color: rgba(255,255,255,0.15);
        border-radius: 50%;
        border: 3px solid rgba(255,255,255,0.4);
        width: 72px;
        height: 72px;
      }
    }

    /deep/ .el-menu {
      background: transparent !important;
      border-right: none !important;
    }
    /deep/ .el-menu-item {
      color: rgba(255,255,255,0.85) !important;
      font-size: 14px;
      height: 46px;
      line-height: 46px;
      &:hover {
        background: rgba(255,255,255,0.12) !important;
        color: #fff !important;
      }
      &.is-active {
        background: rgba(255,255,255,0.2) !important;
        color: #fff !important;
        border-right: 3px solid #40c4ff;
      }
      i { color: rgba(255,255,255,0.7) !important; margin-right: 8px; }
    }
    /deep/ .el-submenu__title {
      color: rgba(255,255,255,0.85) !important;
      font-size: 14px;
      height: 46px;
      line-height: 46px;
      &:hover {
        background: rgba(255,255,255,0.12) !important;
        color: #fff !important;
      }
      i { color: rgba(255,255,255,0.7) !important; }
    }
    /deep/ .el-submenu .el-menu {
      background: rgba(0,0,0,0.15) !important;
      .el-menu-item {
        height: 40px;
        line-height: 40px;
        padding-left: 48px !important;
        font-size: 13px;
      }
    }
    /deep/ .el-submenu__icon-arrow {
      color: rgba(255,255,255,0.6) !important;
    }
  }
</style>
