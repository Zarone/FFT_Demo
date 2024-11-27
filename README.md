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
