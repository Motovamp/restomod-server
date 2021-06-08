const express = require('express')
const app = express()
const server = require('http').createServer(app)
const port = 8090
const io = require('socket.io')(server)

const product_mode = false
exports.pMode = product_mode

// const i2c = require('i2c-bus')
// const DEVICE_ADDR = 0x04

// const i2c1 = i2c.open(1)
// var buf = new Buffer(4)
// const rawData = i2c1.read(DEVICE_ADDR, 0x04, buf, (err, succ) => {
//    console.log(err, succ)
// })


const raspi = require('raspi')
const Serial = require('raspi-serial').Serial
var serial = false

function sProcess(iface, socc) {
	console.log("test2")
	iface.open(() => {
	    let values = ["AA", "AB", "AC", "AD", "AE", "CA", "CB", "CC", "CD", "CE"]
	    let value = -1
	    iface.on('data', (data) => {
		let val = data.toString().replace(/[^ABCD]/g, '')
		value = values.indexOf(val)
		if(value !== -1) {
		    console.log(val)
		    socc.emit('command', val)
		    //iface.close(() => sProcess(iface))
		    // raspiInit()
		    iface.flush()
		}
		// console.log("Clear", data, data.toString())
		process.stdout.write(data)
	    })
	    console.log('raspi-serial opened')
	})
    
}



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


raspi.init(() => {
    serial = new Serial({portId: "/dev/serial0"})
    io.sockets.on('connection', socket => {
	console.log('connected')

	sProcess(serial, socket)

        socket.on('test', data => { 
            console.log(data, socket)
	    setTimeout(() => {
    	        socket.emit('response', 'success')
            }, 1000)
	})
    })
})


/* io.sockets.on('connection', socket => {
    console.log('connected')
    socket.on('test', data => { 
        console.log(data, socket)
        setTimeout(() => {
            socket.emit('response', 'success')
        }, 1000)
    })
}) */