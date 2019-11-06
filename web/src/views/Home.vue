<template>
  <div class="home">
    <h2 class="subtitle">Animations</h2>
    <b-field label="Animation">
      <div class="is-flex">
        <b-select placeholder="Select an animation" v-model="animationSelection">
          <option
                  v-for="(option,key) in animationNames"
                  :key="key">
            {{ option }}
          </option>
        </b-select>
        <b-button style="margin-left: 1rem;" type="is-primary" @click="sendPlay">Play</b-button>
      </div>

    </b-field>

    <div v-if="animationSelection === 'text'">
      <pre>Description: displays a scrolling text</pre>

      <b-field label="Text to show" style="margin-top: 1rem;">
        <b-input v-model="msg"></b-input>
      </b-field>
    </div>

    <div v-if="animationSelection === 'noise'">
      <pre>Description: sets every pixel on the board to a random color</pre>
    </div>

    <div v-if="animationSelection === 'artnet'">
      <pre>Description: acts as an ArtNet client

        Artnet settings: LED type RGB
        Universe 1: 20 x 8, LED starting top - left, snakewise ( only 0 - 480 channels are used )
        Universe 2: 20 x 7, LED starting top - left, snakewise ( only 0 - 420 channels are used )</pre>
    </div>

    <div v-if="animationSelection === 'other'">
      <pre>Description: you can type here a name of the new animation to be shown</pre>

      <b-field label="Animation to show" style="margin-top: 1rem;">
        <b-input v-model="animationName"></b-input>
      </b-field>
    </div>

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
          animationNames: ['text','plasma','noise','artnet','other'],
          animationSelection: 'text',
          animationName: '',
          msg: 'hello :)',
          isSuccess: false,
          isError: false,
          errorMsg: 'Something went wrong.',
      }
  },
    methods: {
      sendPlay: function () {
          let formData = new FormData();
          let animation = this.animationSelection;
          if (this.animationSelection === 'text') formData.append('msg', this.msg);
          if (this.animationSelection === 'other') animation = this.animationName;
          fetch('http://' + window.location.host + '/animation/play/' + animation, { //play endpoint
              method: 'POST',
              headers: {
              },
              body: formData
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
