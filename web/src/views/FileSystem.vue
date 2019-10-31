<template>
  <div class="home">
    <h2 class="subtitle">File system</h2>
    <b-progress :type="percentageType" :value="usedPercentage" show-value size="is-large">
      using {{used}} bytes of {{total}} bytes total - {{usedPercentage}}%
    </b-progress>
    <div>
      <nav class="breadcrumb" aria-label="breadcrumbs">
        <ul>
          <template v-for="(item,key) in pathArray">
            <li :key="key">
              <b-icon icon="home" v-if="key === 0"></b-icon>
              <a v-if="key === 0" href="javascript:void(0);" @click="pathArray=['']">root</a>
              <a v-else href="javascript:void(0);">{{item}}</a>
            </li>
          </template>
        </ul>
      </nav>

      <ul class="filelist">
        <li v-if="pathArray.length > 1"><b-icon icon="folder"></b-icon><a href="javascript:void(0);" @click.prevent="pathArray.pop()">..</a></li>
        <li v-for="(item,index) in fileList" :key="index">
          <div>
            <b-icon :icon="item.type" style="margin-right:.5rem;"></b-icon>
            <a v-if="item.type === 'file'" :href="'/'+item.name">{{item.name}}</a>
            <a href="javascript:void(0);" v-if="item.type === 'folder'" @click.prevent="pathArray.push(item.name)">{{item.name}}</a>
          </div>
          <div>{{item.size}} bytes</div>
          <div>
              <b-button v-if="item.type === 'file'" size="is-small" type="is-danger" @click="deleteFile(item.name)">delete file</b-button>
          </div>

        </li>
      </ul>

      <h2 class="subtitle" style="margin-top: 6rem;">Upload a file to this folder</h2>
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
    </div>
  </div>
</template>

<script>

import BButton from "buefy/src/components/button/Button";
export default {
  name: 'filesystem',
    components: {BButton},
    data() {
      return {
          total: 0,
          used: 0,
          files: [],
          pathArray: [""],
          file: null,
          formData: null,
          isFile: false,
          isUploading: false,
          isSuccess: false,
          isError: false,
          errorMsg: 'Something went wrong.'
      }
    },
    computed: {
        percentageType: function() {
            if (this.usedPercentage>80) return 'is-danger';
            return 'is-primary';
        },
        fileList: function() {
            const files = [];
            const dirs = [];
            const tempDirs = [];
            for (const i in this.files) {
                const chunks = this.files[i].name.split("/");
                if (chunks.length - 1 === this.pathArray.length && chunks[this.pathArray.length - 1] === this.pathArray[this.pathArray.length - 1]) { //is a file on a current level
                    files.push({type:'file',name:chunks[this.pathArray.length],size:this.files[i].size})
                } else if (chunks[this.pathArray.length - 1] === this.pathArray[this.pathArray.length - 1]) { //is a file inside directory
                    const dir = chunks[this.pathArray.length];
                    if (typeof tempDirs[dir] === 'undefined') tempDirs[dir] = {filesNum:0,size:0};
                    tempDirs[dir].filesNum++;
                    tempDirs[dir].size += parseInt(this.files[i].size);
                }
            }
            for (const name in tempDirs) {
                dirs.push({type:'folder',name:name,size:tempDirs[name].size,filesNum:tempDirs[name].filesNum})
            }
            return dirs.concat(files);
        },
        usedPercentage: function() {
            if (this.total > 0) {
                return Math.round(this.used / this.total * 100);
            }
            return null;
        }

    },
    mounted: function() {
      this.getFiles();
    },
    methods: {
        getFiles:function () {
            fetch('http://' + window.location.host + '/spiffs', { //fs endpoint
                method: 'GET'
            }).then(
                (resp) => resp.json()
            ).then(
                (data) => {
                    if (data.total) this.total = data.total;
                    if (data.used) this.used = data.used;
                    if (data.files) this.files = data.files;
                }
            ).catch(
                error => {
                    this.isError = true;
                    this.errorMsg = error;
                }
            );
        },
        filesChange: function(file) {
            this.formData = new FormData();
            this.isFile = true;
            this.formData.append('file', file);
        },
        deleteFile: function (file) {
            let path = '?path=' + this.pathArray.join('/') + '/' + file;
            fetch('http://' + window.location.host + '/spiffs=' + encodeURI(path), { //delete endpoint
                method: 'DELETE',
                headers: {
                },
                body: this.formData // This is your file object
            }).then(
                () => {
                    this.getFiles();
                    this.isSuccess = true;
                    this.isUploading = false;
                }
            ).catch(
                error => {
                    this.isError = true;
                    this.errorMsg = error;
                }
            );
        },
        submitFile: function () {
            this.isUploading = true;
            let path = ''
            if (this.pathArray.length > 1) path = '?path=' + this.pathArray.join('/')+'/';
            fetch('http://' + window.location.host + '/spiffs' + encodeURI(path), { //update endpoint
                method: 'POST',
                headers: {
                },
                body: this.formData // This is your file object
            }).then(
                () => {
                    this.getFiles();
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
<style>
ul.filelist li {
  display: flex;
}

ul.filelist li div {
    padding: .2rem;
  flex: 1 1 33%;
}

</style>
