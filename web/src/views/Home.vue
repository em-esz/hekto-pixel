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

            <div class="columns" style="margin-top: 1rem;">
                <div class="column">
                    <b-field label="Text to show" >
                        <b-input v-model="msg"></b-input>
                    </b-field>
                </div>
                <div class="column is-1 has-text-centered">
                    <b-field label="Color" class="has-text-centered">
                        <swatches v-model="textColor" colors="text-advanced"></swatches>
                    </b-field>
                </div>
                <div class="column">
                    <b-field label="Interval">
                        <b-slider size="is-large" :min="30" :max="300" v-model="interval" style="margin-top:1.4rem;"></b-slider>
                    </b-field>
                </div>
            </div>

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

        <b-notification has-icon style="margin-top: 1rem;" type="is-danger" aria-close-label="Close"
                        :active.sync="isError">
            {{errorMsg}}
        </b-notification>
    </div>
</template>

<script>
    import Swatches from 'vue-swatches'
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
                textColor: '#FF0000',
                interval: 50,
                isSuccess: false,
                isError: false,
                errorMsg: 'Something went wrong.',
            }
        },
        methods: {
            hexToRGB(h) {
                let hex = h.replace('#','');
                let r = parseInt(hex.substring(0,2), 16);
                let g = parseInt(hex.substring(2,4), 16);
                let b = parseInt(hex.substring(4,6), 16);

                return {r,g,b};
            },
            sendPlay: function () {
                let animation = this.animationSelection;
                let jsonPayload = {};
                if (this.animationSelection === 'text') {
                    jsonPayload['msg'] = this.msg;
                    jsonPayload['color'] = this.hexToRGB(this.textColor);
                    jsonPayload['interval'] = this.interval;
                }
                if (this.animationSelection === 'other') animation = this.animationName;
                fetch('http://' + window.location.host + '/animation/play/' + animation, { //play endpoint
                    method: 'POST',
                    headers: {},
                    body: JSON.stringify(jsonPayload)
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
