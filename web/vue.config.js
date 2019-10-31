module.exports = {
    productionSourceMap: false,
    configureWebpack: config => {
        config.output.filename = '[name].js'
        config.output.chunkFilename = '[name].js'
    },
    css: {
        extract: {
            filename: '[name].css',
        },
    }
};