const WebSocketServer = require('ws').Server
const bodyParser = require('body-parser')
const express = require('express')
const coap = require('coap')
const app = express()

const port = 3000

app.use(bodyParser.urlencoded({extended: false}))

// all handler:
app.all('/*', function (req, res) {
	console.log('Got a ' + req.method + ' request')
	// the parameters of a GET request are passed in
	// request.body. Pass that to formatResponse()
	// for formatting:
	console.log(req.headers)
	if (req.method == 'GET') {
		console.log(req.query)
	} else {
		console.log(req.body)
	}

	// send the response:
	res.send('OK')
	res.end()
})

// start the http server:
const server = app.listen(port, () => {
  console.log(`Server listening on port ${port}`)
})

// create a WebSocket server and attach it to the server
var wss = new WebSocketServer({server: server})

wss.on('connection', function connection(ws) {
	// new connection, add message listener
	ws.on('message', function incoming(message) {
		// received a message
		console.log('received: %s', message)
		// echo it back
		ws.send(message)
	})
})

// create a CoAP server
const CoapServer = coap.createServer()

CoapServer.on('request', (req, res) => {
	console.log('Got a ' + req.method + ' request')
	// the parameters of a GET request are passed in
	// request.body. Pass that to formatResponse()
	// for formatting:
	console.log(req.headers)
	if (req.method == 'GET') {
		console.log(req.query)
	} else {
		console.log(req.body)
	}

	// send the response:
	res.send('OK')
	res.end()
})

// the default CoAP port is 5683
CoapServer.listen(() => {
  const req = coap.request(`coap://0.0.0.0:${port}/arduino`)

  req.on('response', (res) => {
    res.pipe(process.stdout)
    res.on('end', () => {
      process.exit(0)
    })
  })

  req.end()
})
