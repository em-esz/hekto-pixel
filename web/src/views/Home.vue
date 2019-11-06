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


        <pre style="margin-top:2rem;">
      Artnet settings:
      LED type RGB
      Universe 1: 20 x 8, LED starting top - left, snakewise ( only 0 - 480 channels are used )
      Universe 2: 20 x 7, LED starting top - left, snakewise ( only 0 - 420 channels are used )
    </pre>
        <div class="has-text-centered is-flex">
            <div style="flex:1" class="is-flex">
                <canvas id="sketchpad"></canvas>
                <swatches v-model="sketchpadColor" colors="text-advanced" @input="changeColor"></swatches>
            </div>

            <canvas id="hektopixel"></canvas>

        </div>
        <b-button class="is-primary" @click="clear">clear</b-button>
        <b-button class="is-primary" @click="animate">animate</b-button>
        <b-button class="is-primary" @click="cancelAnimation">cancel animation</b-button>
        <b-button class="is-primary" @click="reset">reset animation</b-button>
        <b-button class="is-primary" @click="undo">undo</b-button>
        <b-button class="is-primary" @click="redo">redo</b-button>

        <b-notification has-icon style="margin-top: 1rem;" type="is-danger" aria-close-label="Close"
                        :active.sync="isError">
            {{errorMsg}}
        </b-notification>
    </div>
</template>

<script>
    import Sketchpad from '../sketchpad'
    import Swatches from 'vue-swatches'
    import _ from 'lodash'

    import "vue-swatches/dist/vue-swatches.min.css"

    export default {
        name: 'home',
        components: {Swatches},
        data() {
            return {
                animationNames: ['text', 'plasma', 'noise', 'artnet', 'other'],
                animationSelection: 'text',
                animationName: '',
                msg: 'hello :)',
                sketchpad: null,
                sketchpadColor: '#2cffd4',
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
            this.changeColor();

            // Change stroke size
            this.sketchpad.penSize = 10;
            this.sketchpad.on('show', this.sendData)
        },
        methods: {
            changeColor: function () {
                this.sketchpad.color = this.sketchpadColor;
            },
            sendData: _.throttle(function (data) {
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
                this.sketchpad.animate(25, true);
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
                let formData = new FormData();
                let animation = this.animationSelection;
                if (this.animationSelection === 'text') formData.append('msg', this.msg);
                if (this.animationSelection === 'other') animation = this.animationName;
                fetch('http://' + window.location.host + '/animation/play/' + animation, { //play endpoint
                    method: 'POST',
                    headers: {},
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
<style>
    #sketchpad {
        cursor: crosshair;
    }
    #hektopixel {
        cursor: not-allowed;
        width: 400px;
        height: 300px;
        image-rendering: -moz-crisp-edges;
        image-rendering: -o-crisp-edges;
        image-rendering: -webkit-optimize-contrast;
        -ms-interpolation-mode: nearest-neighbor;
        image-rendering: pixelated;
    }
</style>