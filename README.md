### Run Server with Docker
```
cd server
docker build -t my_image .
docker run -d -p 3000:3000 --name my_container my_image
```
