let data_module = require('../modules/main')

exports.index = (req, res) => {
    let base = data_module.md()
    res.render('index/index', base)
}