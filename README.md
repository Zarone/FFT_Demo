### Run Server with Docker
```bash
cd server
docker build -t my_image .
docker run -d -p 3000:3000 --name my_container my_image
```

### Run Client
```bash
cd client
npm i
npm run dev
```

### TODO
- Minimum Viable Product
  - Setup React app
  - Setup front end to get audio file data
  - Send audio file data to backend and establish socket
  - Process audio data in backend
    - Split audio into its components
  - Return audio components to front end
  - Remerge selected audio components
- Ideal Additions
  - After socket established, and backend processed data, have socket send back the data from the file, and render audio file 
  - Render audio components on front end
