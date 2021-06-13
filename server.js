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
const gpio = require('raspi-gpio')
var serial = false





function sProcess(iface, socc) {
	console.log("test2")
	iface.open(() => {
	    let values = ["A", "B", "C", "D", "E", "Q", "R", "S", "T", "U", "P"]
	    let value = -1
	    iface.on('data', (data) => {
			let val = data.toString().replace(/[^ABCDEQRSTUP]/g, '')
			value = values.indexOf(val)
			if(value !== -1) {
		    	console.log('command ' + val)
				switch(val) {
					case "K": socc.emit('check', 1); break 
					case "L": socc.emit('check', 0); break 
					case "M": socc.emit('brake', 1); break 
					case "N": socc.emit('brake', 0); break
					default: socc.emit('command', val)
				}
		    	// iface.flush()
			}
			// console.log('value ' + val)
			process.stdout.write(data)
			// iface.flush()
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


// var brakeValue = 1,
// 	checkValue = 1

    const brake = new gpio.DigitalInput({
		pin: 'GPIO23',
		pullResistor: gpio.PULL_DOWN
    })
    const check = new gpio.DigitalInput({
		pin: 'GPIO24',
		pullResistor: gpio.PULL_DOWN
    })

    const turns = new gpio.DigitalOutput('GPIO16')
    const boost1 = new gpio.DigitalOutput('GPIO20')
    const boost2 = new gpio.DigitalOutput('GPIO21')
	boost1.write(0) 
	boost2.write(0) 
	turns.write(0)

function digitalRead(socket) {
	let bv = brake.read()
	socket.emit('brake', bv)
		
	let cv = check.read()
	socket.emit('check', cv)
	
	// console.log('reading', bv, cv)
}

function boostOn() {
	boost1.write(0)
	boost2.write(1)
	setTimeout(() => boost2.write(0), 1000)
}

function boostOff() {
	boost2.write(0)
	boost1.write(1)
	setTimeout(() => boost1.write(0), 1000)
}


raspi.init(() => {
    serial = new Serial({portId: "/dev/serial0"})
    sProcess(serial, io.sockets)

    io.sockets.on('connection', socket => {
		console.log('connected')
		socket.emit('sconnect', 'success')

        socket.on('read', () => { 
			digitalRead(socket)
		})

		socket.on('exhst', value => {
			if(value == 'boost') {
				boostOn()
			} else {
				boostOff()
			}
		})		
		socket.on('turn', value => {
			if(value == 'blink') {
				turns.write(1)
			} else {
				turns.write(0)
			}
		})
    })

    
})


