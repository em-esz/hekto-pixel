import Vue from 'vue'
import App from './App.vue'
import router from './router'
import VueNativeSock from 'vue-native-websocket'
import Buefy from 'buefy'
import 'buefy/dist/buefy.css'
Vue.use(Buefy)
Vue.use(VueNativeSock, 'ws://' + window.location.host + '/ws', {
    connectManually: true,
    reconnection: true // (Boolean) whether to reconnect automatically (false)
})

Vue.config.productionTip = false

new Vue({
  router,
  render: h => h(App)
}).$mount('#app')
