<template>
  <div class="home">
    <h2 class="subtitle">Mode</h2>
    <div class="buttons">
      <b-button type="is-primary" @click="sendMode('Artnet')">ArtNet</b-button>
      <b-button type="is-primary" @click="sendMode('Artnet')">Animation</b-button>
    </div>
    <h2 class="subtitle">Animations</h2>
    <pre>
      Available animations
      - test
      - ledblink - blinks the LED built into the ESP32 controller
      - noise - sets every pixel on the board to a random color
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
      LED type GRB
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
      sendMode: function (mode) {
          fetch('http://' + window.location.host + '/set?mode=' + mode, { //play endpoint
              method: 'GET',
              headers: {
              },
          }).then(
              () => {
              }
          ).catch(
              error => {
                  this.isError = true;
                  this.errorMsg = error;
              }
          );
      },
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
