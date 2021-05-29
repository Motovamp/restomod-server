const express = require('express')
const app = express()
const server = require('http').createServer(app)
const port = 8090
const io = require('socket.io')(server)

const product_mode = false
exports.pMode = product_mode

// const i2c1 = require('i2c-bus')

const raspi = require('raspi')
const Serial = require('raspi-serial').Serial
 
raspi.init(() => {
  var serial = new Serial()
  serial.open(() => {
    serial.on('data', (data) => {
      console.log(data)
    });
    console.log('Hello from raspi-serial');
  });
});

//Шаблонизатор и статика
app.set('views', './templates')
app.set('view engine', 'pug')
app.use(express.static(__dirname + '/public'))
app.use('/img', express.static(__dirname + '/public/jsvue/img'))

const indexRouter = require('./routes/index')
app.use('/', indexRouter)

server.listen(port, () => {
    console.log(`IO started at port ${port}`)
})

io.sockets.on('connection', socket => {
    console.log('connected')
    socket.on('test', data => { 
        console.log(data, socket)
        setTimeout(() => {
            socket.emit('response', 'success')
        }, 1000)
    })
})