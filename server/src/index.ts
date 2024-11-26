let {transformWavFileBufferDFT, transformWavFileBufferFFT} = require("./engine/engine");

let express = require("express");
const app = express();
const cors = require("cors");

app.set("port", 3001);

const whitelist = ["http://localhost:3000"]
const corsOptions = {
  origin: (origin: string, callback: (err: Error | null, origin?: string|boolean) => void) => {
    if (!origin || whitelist.indexOf(origin) !== -1) {
      callback(null, true)
    } else {
      callback(new Error("Not allowed by CORS"))
    }
  },
  credentials: true,
}
app.use(cors(corsOptions))

let http = require("http").Server(app);
let io = require("socket.io")(http, {
  origin: 'http://localhost:3000', // Your React frontend URL
  methods: ['GET', 'POST'],
  credentials: true,
  transports: ['websocket'],
  pingTimeout: 500000,
  pingInterval: 25000,
  maxHttpBufferSize: 1e8
});

const handleDataReceived = (socket: any, data: Uint8Array, fast: boolean) => {
  console.log("Data Received");

  // This is necessary because the actual buffer can start earlier than the uint8array
  const cutBuffer: ArrayBuffer = data.buffer.slice(data.byteOffset, data.byteOffset+data.byteLength);

  //console.group("Receive: Printing Header Info");
  //console.log(cutBuffer.slice(0, 44));
  //console.groupEnd();

  console.log("Processing...");
  //console.log("Printing Header Info");

  let buf: ArrayBuffer[];
  if (fast) {
     buf = transformWavFileBufferFFT(cutBuffer);
  } else {
     buf = transformWavFileBufferDFT(cutBuffer);
  }
  //console.log(buf[0]?.slice(0,44));

  socket.emit("decomposedTransfer", buf);
}

// whenever a user connects on port 3000 via
// a websocket, log that a user has connected
io.on("connection", (socket: any) => {
  socket.on("dataTransfer", (data: Uint8Array)=>{
    handleDataReceived(socket, data, false);
  })
  socket.on("dataTransferFast", (data: Uint8Array)=>{
    handleDataReceived(socket, data, true);
  })
});

http.listen(3001, function() {
  console.log("listening on *:3001");
});
