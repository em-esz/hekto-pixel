module.exports = {
    productionSourceMap: false,
    configureWebpack: config => {
        config.output.filename = 'js/[name].js'
        config.output.chunkFilename = 'js/[name].js'
    },
    css: {
        extract: {
            filename: 'css/[name].css',
            chunkFilename: 'css/[name].css'
        },
    }
};