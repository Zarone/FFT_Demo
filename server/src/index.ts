let {method} = require("./engine/engine");
console.log(method([1, 2, 3]));

let express = require("express");
const app = express();

app.set("port", 3001);

let http = require("http").Server(app);
let io = require("socket.io")(http);

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
