### Overview

This project implements several types of fast fourier transform algorithms and renders them using a custom react component. There's also a small cli program to make sound files for testing. To use it, you need to setup the server and the client using the below commands (the server can be setup using npm directly or docker).

### Run Server with Docker
```bash
cd server
docker build -t my_image .
docker run -it -p 3001:3001 --name my_container my_image
```

### Run Client
```bash
cd client
npm i
npm run dev
```
