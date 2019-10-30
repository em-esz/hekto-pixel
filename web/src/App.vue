<template>
  <div id="app" class="container" style="margin-top: 2rem">
    <h1 class="title">hekto Pixel GUI</h1>
    <b-tabs type="is-boxed" class="block" v-model="activeTab">
      <b-tab-item>
        <router-link tag="div" :to="{ name: 'home' }" slot="header">Home</router-link>
      </b-tab-item>
      <b-tab-item>
        <router-link tag="div" :to="{ name: 'preview' }" slot="header">Preview</router-link>
      </b-tab-item>
      <b-tab-item>
        <router-link tag="div" :to="{ name: 'update' }" slot="header">Update</router-link>
      </b-tab-item>
    </b-tabs>
    <router-view/>
  </div>
</template>

<script>
    export default {
        data() {
            return {
                activeTab: null
            }
        },
        created() {
            let x = 0
            switch (this.$router.currentRoute.name) {
                case 'update':
                    x = 2
                    break
                case 'preview':
                    x = 1
                    break
                default:
                    x = 0
                    break
            }

            this.activeTab = x
        }
    }
</script>

<style lang="scss">
.tabs a {
  padding: 0 !important;

  & div {
    padding: .5rem 1rem;
  }
}
</style>
