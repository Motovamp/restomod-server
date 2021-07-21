const express = require('express')
const fs = require('fs')
const app = express()
const server = require('http').createServer(app)
const port = 8090
const io = require('socket.io')(server)
const valve = 125

const product_mode = false
exports.pMode = product_mode

const raspi = require('raspi')
const Serial = require('raspi-serial').Serial
const gpio = require('raspi-gpio')
var serial = false
var dispalyState = false

function sProcess(iface, socc) {
	console.log("test2")
	iface.open(() => {
	    let values = ["A", "B", "C", "D", "E", "Q", "R", "S", "T", "U", "P", "M", "I", "O", "W"]
	    let value = -1
	    iface.on('data', (data) => {
			let val = data.toString().replace(/[^ABCDEQRSTUPMIOW]/g, '')
			value = values.indexOf(val)
			if(value !== -1) {
				
		    	console.log('command ' + val)
				switch(val) {
					case "I":
						// console.log("Info")
						if(!dispalyState) { // включаем подсветку дисплея и отправляем команду на запуск картинкия заставки
							hscreen.write(1)
							dispalyState = true
							setTimeout(() => socc.emit('command', 'Start'), 30)
						}  
					break 
					case "W":
						if(!dispalyState) { // включаем подсветку дисплея и отправляем команду на запуск картинкия заставки
							hscreen.write(1)
							dispalyState = true
							setTimeout(() => socc.emit('command', 'WStart'), 30)
						}  
					break 
					case "O": 
						if(dispalyState) { // зажигание выключено, отрубаем дисплей
							hscreen.write(0)
							dispalyState = false
							socc.emit('command', 'Off')
						}
					break 
					// default: socc.emit('command', val)
				}
				setTimeout(() => socc.emit('command', val), 10)
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

    const brake = new gpio.DigitalInput({ // концевик капота (бывший ручной тормоз)
		pin: 'GPIO23',
		pullResistor: gpio.PULL_UP
    })
    const check = new gpio.DigitalInput({ // концевик двери (бывший чек)
		pin: 'GPIO24',
		pullResistor: gpio.PULL_UP
    })
    const malf = new gpio.DigitalInput({ // неисправность блока реле 
		pin: 'GPIO26',
		pullResistor: gpio.PULL_DOWN
    })

    const working = new gpio.DigitalOutput('GPIO12')
    const turns = new gpio.DigitalOutput('GPIO16')
    const boost1 = new gpio.DigitalOutput('GPIO20')
    const boost2 = new gpio.DigitalOutput('GPIO21')

    const hscreen = new gpio.DigitalOutput('GPIO18') // разрыв подсветки экрана (сразу после UART)

	boost1.write(0) 
	boost2.write(0) 
	turns.write(0)
	hscreen.write(0)  // Как только загрузились, гасим дисплей

function digitalRead(socket) {
	let bv = brake.read()
	socket.emit('brake', bv ? '0' : '1')
		
	let cv = check.read()
	socket.emit('check', cv ? '0' : '1')

	let mv = malf.read()
	socket.emit('malf', mv ? '1' : '0')
	
	// console.log('reading', bv, cv)
}

function boost() {
	return parseInt(fs.readFileSync('./exhst.info'))
}

function boostSet(value) {
	fs.writeFileSync('./exhst.info', value)
}

function boostOn() {
	boost1.write(0)
	if(!boost()) {
		boost2.write(1)
		setTimeout(() => {
			boost2.write(0)
		}, valve)
	}
	boostSet(1)
}

function boostOff() {
	boost2.write(0)
	if(boost()) {
		boost1.write(1)
		setTimeout(() => boost1.write(0), valve)
	}
	boostSet(0)
}


raspi.init(() => {
    // serial = new Serial({portId: "/dev/serial0"})
    serial = new Serial({portId: "/dev/ttyUSB0"})
    sProcess(serial, io.sockets)

    io.sockets.on('connection', socket => {
		console.log('connected')
			setTimeout(() => working.write(1), 200)		
			socket.emit('sconnect', 'success')
			// hscreen.write(1)

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
				turns.write(0)
			} else {
				turns.write(1)
			}
		})
    })

    
})


