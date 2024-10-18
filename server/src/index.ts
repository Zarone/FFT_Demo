let {method, zeroArrayBuffer, transformWavFileBuffer} = require("./engine/engine");

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
});

// whenever a user connects on port 3000 via
// a websocket, log that a user has connected
io.on("connection", (socket: any) => {
  socket.binaryType = 'arraybuffer';
  console.log("A user connected");
  socket.on("dataTransfer", (data: Uint8Array)=>{

    console.log("Data Transfer");

    console.group("Printing Data Info");
    console.log("Received Data");
    console.log(data);
    console.log(Object.prototype.toString.call(data));
    console.groupEnd();

    // This is necessary because the actual buffer can start earlier than the uint8array
    const cutBuffer = data.buffer.slice(data.byteOffset, data.byteOffset+data.byteLength);

    console.group("Printing Buffer Info");
    console.log(cutBuffer);
    console.groupEnd();

    console.log("Emitting Data");
    console.group("Printing Header Info");
    const buf: ArrayBuffer[] = transformWavFileBuffer(cutBuffer);
    console.log(buf[0]?.slice(0,44));
    console.groupEnd();

    socket.emit("decomposedTransfer", buf );
  })
});

http.listen(3001, function() {
  console.log("listening on *:3001");
});
