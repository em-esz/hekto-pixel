<template>
  <div id="app" class="container">
    <h1 class="title">hekto Pixel GUI</h1>
    <div id="nav" class="buttons">
      <router-link to="/" v-slot="{ navigate, isExactActive  }">
        <b-button :type="isExactActive?'is-success':'is-link'" @click="navigate">Home</b-button>
      </router-link>
      <router-link to="/update" v-slot="{ navigate, isExactActive  }">
        <b-button :type="isExactActive?'is-success':'is-link'" @click="navigate">Update</b-button>
      </router-link>
    </div>
    <router-view/>
  </div>
</template>

<script>
    export default {
        name: 'App',
        data() {
            return {

            }
        },
        methods: {

        }
    }
</script>

<style>

</style>
