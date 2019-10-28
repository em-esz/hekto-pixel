import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../views/Home.vue'
import Update from '../views/Update.vue'

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
  }
]

const router = new VueRouter({
  routes
})

export default router
