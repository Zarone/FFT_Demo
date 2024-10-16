let {method} = require("./engine/engine");
console.log(method([1, 2, 3]));

let express = require("express");
const app = express();
const cors = require("cors");

app.set("port", 3001);

const whitelist = ["http://localhost:3000"]
const corsOptions = {
  origin: function (origin: any, callback: Function) {
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

// simple '/' endpoint sending a Hello World
// response
app.get("/", (_: any, res: any) => {
  res.send("hello world");
});

// whenever a user connects on port 3000 via
// a websocket, log that a user has connected
io.on("connection", function(_: any) {
  console.log("a user connected");
});

const server = http.listen(3001, function() {
  console.log("listening on *:3001");
});
