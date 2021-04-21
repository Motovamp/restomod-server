const fs = require('fs')
let imdl = require('../server')
let pMode = imdl.pMode

exports.md = function() {
    return {
        styles: pMode ? fs.readdirSync(__dirname + '/../public/jsvue/css').sort((a, b) => b.length - a.length) : [],
        scripts: fs.readdirSync(__dirname + '/../public/jsvue/js').filter(el => el.endsWith('.map') === false).sort((a, b) => b.length - a.length)
    }
}