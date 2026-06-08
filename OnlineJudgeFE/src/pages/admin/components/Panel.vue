<template>
  <div class="panel" :class="{'small': small}">
    <header>
      <div class="title">
        <template v-if="$slots.title">
          <slot name="title"></slot>
        </template>
        <template v-else>
          {{title}}
        </template>
      </div>

      <div class="header_right">
        <slot name="header"></slot>
      </div>
    </header>

    <div class="body">
      <slot></slot>
    </div>
  </div>
</template>

<script>
  export default {
    name: 'Panel',
    props: {
      title: {
        type: String,
        required: false
      },
      small: {
        type: Boolean,
        default: false
      }
    }
  }
</script>
<style scoped lang="less">
  .panel {
    margin-bottom: 20px;
    background-color: #fff;
    border: none;
    border-radius: 10px;
    box-shadow: 0 2px 12px rgba(0,0,0,0.07);
    transition: box-shadow 0.3s;
    &:hover {
      box-shadow: 0 4px 20px rgba(0,0,0,0.11);
    }
    &.small {
      max-width: 830px;
      min-width: 700px;
      margin-left: 20px;
      margin-top: 10px;
    }
    header {
      position: relative;
      z-index: 10;
      > .title {
        margin: 0;
        color: #1a237e;
        font-size: 16px;
        font-weight: 600;
        height: 54px;
        line-height: 54px;
        padding: 0 20px;
        border-bottom: 1px solid #f0f0f0;
        border-top-left-radius: 10px;
        border-top-right-radius: 10px;
        background: linear-gradient(90deg, #e8f0fe 0%, #fff 100%);
        border-left: 4px solid #1565c0;
        letter-spacing: 0.3px;
      }
      > .header_right {
        position: absolute;
        top: 50%;
        right: 20px;
        transform: translate(0, -50%);
      }
    }
    .body {
      padding: 20px;
    }
  }
</style>
<style lang="less">
  .panel-options {
    background-color: transparent;
    position: relative;
    height: 50px;
    button {
      margin-top: 18px;
      margin-right: 10px;
    }
    > .page {
      position: absolute;
      right:20px;
      top: 20px;
    }
  }
</style>
