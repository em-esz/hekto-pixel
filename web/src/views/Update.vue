<template>
    <div class="">
        <h2 class="subtitle">Update</h2>
        <p style="margin-bottom:1rem;">Here you can update firmware by uploading bin file</p>
        <form method='POST' action='/update' enctype='multipart/form-data'>
            <b-field class="file">
                <b-upload v-model="file" :disabled="isUploading" @input="filesChange($event);">
                    <a class="button is-primary">
                        <b-icon icon="upload"></b-icon>
                        <span>Click to select file</span>
                    </a>
                </b-upload>
                <span class="file-name" v-if="file">
                    {{ file.name }}
                </span>
            </b-field>
            <b-button class="is-primary" :disabled="!isFile || isUploading" @click="submitFile">Submit</b-button>
            <b-notification has-icon style="margin-top: 1rem;" type="is-info" :closeable="false" v-show="isUploading">
                Wait for file to be uploaded...
            </b-notification>
            <b-notification has-icon style="margin-top: 1rem;" type="is-success" aria-close-label="Close" :active.sync="isSuccess">
                Firmware has been uploaded successfully
            </b-notification>
            <b-notification has-icon style="margin-top: 1rem;" type="is-danger" aria-close-label="Close" :active.sync="isError">
                {{errorMsg}}
            </b-notification>
        </form>
    </div>
</template>

<script>

    export default {
        name: 'update',
        data() {
            return {
                file: null,
                formData: null,
                isFile: false,
                isUploading: false,
                isSuccess: false,
                isError: false,
                errorMsg: 'Something went wrong.'
            }
        },
        methods: {
            filesChange: function(file) {
                this.formData = new FormData();
                this.isFile = true;
                this.formData.append('file', file);
            },
            submitFile: function () {
                this.isUploading = true;
                fetch('http://' + window.location.host + '/update', { //update endpoint
                    method: 'POST',
                    headers: {

                    },
                    body: this.formData // This is your file object
                }).then(
                    () => {
                        this.isSuccess = true;
                        this.isUploading = false;
                    } // Handle the success response object
                ).catch(
                    error => {
                        this.isError = true;
                        this.isUploading = false;
                        this.errorMsg = error;
                    } // Handle the error response object
                );

            }
        }
    }
</script>
