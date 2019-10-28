<template>
  <div class="home">
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
          fetch('http://' + window.location.host + '/update', { //play endpoint
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
