import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../views/Home.vue'
import Update from '../views/Update.vue'
import Preview from '../views/Preview.vue'
import FS from '../views/FileSystem.vue'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'home',
    component: Home
  },
  {
    path: '/update',
    name: 'update',
    component: Update
  },
  {
      path: '/preview',
      name: 'preview',
      component: Preview
  },
  {
      path: '/fs',
      name: 'fs',
      component: FS
  }
]

const router = new VueRouter({
  routes
})

export default router
