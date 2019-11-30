// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import router from './router'
import axios from 'axios'

//Vue.use(axios);
Vue.prototype.$axios = axios
Vue.config.productionTip = false

/* eslint-disable no-new */
new Vue({
  //  为实例提供挂载的元素，此处为index.html
  el: '#app',
  //  指向router里index.js中的routes:[]
  router,
  //  注册组件，在文件顶部import
  components: { App },
  // 替换挂载元素的模版组件，用template替换index.html中的<div id = "app"></div>
  template: '<App/>'
})
