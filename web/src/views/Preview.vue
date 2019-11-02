<template>
  <div class="home">
    <h2 class="subtitle">Preview</h2>
    <div class="previewGrid">
      <div class="cellRow" v-for="x in 15" :key="x">
        <div class="cell"
             v-for="y in 20"
             :key="y"
             :style="'background: ' + hexArray[(x%2===1?(x-x%2) * 20  + y:(x-x%2) * 20  - y + 1)-1]">
            {{ x%2===1?(x-x%2) * 20  + y:(x-x%2) * 20  - y + 1}}
        </div>
      </div>
    </div>


  </div>
</template>

<script>

export default {
  name: 'preview',
  data() {
      return {
          hexArray: [],
          statusMsg: '',
          isSuccess: false,
          isError: false,
          errorMsg: 'Something went wrong.',
      }
  },
    methods: {
      status: function() {
          this.$socket.send('some data')
      }
  },
    mounted: function() {
      this.$connect();
      let _self = this;
      this.$options.sockets.onmessage = (msg) => {
          new Response(msg.data).arrayBuffer()
              .then((ab)=>{
                  let view = new Uint8Array(ab);
                  let hexArray = [];
                  let r,g,b = '';
                  for(let i = 0; i < view.length; i++ ) {
                      switch (i%3) {
                          case 0:
                              r = ('0' + view[i].toString(16)).substr(-2);
                              break;
                          case 1:
                              g = ('0' + view[i].toString(16)).substr(-2);
                              break;
                          case 2:
                              b = ('0' + view[i].toString(16)).substr(-2);
                              hexArray.push('#' + r + g + b);
                              break;
                      }
                  }
                  _self.hexArray = hexArray;
              });
      }
    }
}
</script>
<style>
  .cellRow {
    display: flex;
  }
  .cell {
    text-align: center;
    flex: 0 0 40px;
    height: 40px;
    width: 40px;
    border: 1px solid black;
  }
</style>