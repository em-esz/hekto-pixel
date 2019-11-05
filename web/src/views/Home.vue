<template>
  <div class="home">
    <h2 class="subtitle">Animations</h2>
    <pre>
      Available animations
      - plasma
      - text - displays a scrolling text
      - noise - sets every pixel on the board to a random color
      - artnet - acts as an ArtNet client
    </pre>

    <b-field label="Animation name">
      <b-input v-model="animationName"></b-input>
    </b-field>
    <b-button type="is-primary" @click="sendPlay">Play</b-button>
    <b-notification has-icon style="margin-top: 1rem;" type="is-danger" aria-close-label="Close" :active.sync="isError">
      {{errorMsg}}
    </b-notification>
    <pre style="margin-top:2rem;">
      Artnet settings:
      LED type RGB
      Universe 1: 20 x 8, LED starting top - left, snakewise ( only 0 - 480 channels are used )
      Universe 2: 20 x 7, LED starting top - left, snakewise ( only 0 - 420 channels are used )
    </pre>
  </div>
</template>

<script>

export default {
  name: 'home',
  data() {
      return {
          animationName: '',
          isSuccess: false,
          isError: false,
          errorMsg: 'Something went wrong.',
      }
  },
    methods: {
      sendPlay: function () {
          fetch('http://' + window.location.host + '/animation/play/' + this.animationName, { //play endpoint
              method: 'POST',
              headers: {
              },
          }).then(
              () => {
                  this.isSuccess = true;
              }
          ).catch(
              error => {
                  this.isError = true;
                  this.errorMsg = error;
              }
          );
    }
  }
}
</script>
