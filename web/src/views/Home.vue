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
    <div class="has-text-centered">
      <canvas id="sketchpad"></canvas>
      <canvas id="hektopixel"></canvas>
    </div>
    <b-button @click="clear">clear</b-button>
    <b-button @click="animate">animate</b-button>
    <b-button @click="cancelAnimation">cancel animation</b-button>
    <b-button @click="reset">reset animation</b-button>
    <b-button @click="undo">undo</b-button>
    <b-button @click="redo">redo</b-button>
  </div>
</template>

<script>
import Sketchpad from '../sketchpad'
import _ from 'lodash'

export default {
  name: 'home',
    data() {
      return {
          sketchpad: null,
          animationName: '',
          isSuccess: false,
          isError: false,
          errorMsg: 'Something went wrong.',
      }
  },
    mounted: function () {
        this.$connect();
        this.sketchpad = new Sketchpad({
            element: '#sketchpad',
            destination: '#hektopixel',
            width: 400,
            height: 300,
        });
        // Change color
        this.sketchpad.color = '#FF0000';

        // Change stroke size
        this.sketchpad.penSize = 10;
        this.sketchpad.on('show', this.sendData)
    },
    methods: {
        sendData: _.throttle(function(data) {
            if (this.$socket)
                this.$socket.send(data.buffer);
        }, 200).bind(this),
      sendData2(data) {
          console.log(data);
          if (this.$socket)
            this.$socket.send(data);
      },
        clear: function () {
            this.sketchpad.clear();
        },
      cancelAnimation: function () {
          this.sketchpad.cancelAnimation();
      },
      animate: function () {
        this.sketchpad.animate(25,true);
      },
      reset: function () {
        this.sketchpad.reset();
      },
        undo: function () {
          this.sketchpad.undo();
        },
        redo: function () {
            this.sketchpad.redo();
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
<style>
canvas {
  border: 1px solid black;
}
#tempimage {
  width: 400px;
  height: 300px;
}
  #hektopixel {
    width: 400px;
    height: 300px;
    image-rendering: -moz-crisp-edges;
    image-rendering: -o-crisp-edges;
    image-rendering: -webkit-optimize-contrast;
    -ms-interpolation-mode: nearest-neighbor;
    image-rendering: pixelated;
  }
</style>