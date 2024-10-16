let {method, zeroArrayBuffer, transformWavFileBuffer} = require("./engine/engine");
console.log(method([1, 2, 3]));
console.log(zeroArrayBuffer(
  new Uint16Array([1, 2, 3, 4, 5, 6, 7]).buffer
));
console.log(transformWavFileBuffer(
  new Uint16Array([1, 2, 3, 4, 5, 6, 7]).buffer
));

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
});

// whenever a user connects on port 3000 via
// a websocket, log that a user has connected
io.on("connection", (socket: any) => {
  console.log("a user connected");
  socket.on("dataTransfer", (data: ArrayBuffer)=>{
    console.log("dataTransfer", data);
  })
});

http.listen(3001, function() {
  console.log("listening on *:3001");
});
