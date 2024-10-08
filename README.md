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
  - Setup front end to get audio file data
  - Send audio file data to backend and establish socket
  - Process audio data in backend
    - Split audio into its components
  - Return audio components to front end
    - Render audio components on front end once returned from backend
  - Remerge selected audio components by sending to backend
- Ideal Additions
  - After front end recieves audio file, render the data from the audio file 
    - Get data locally instead of sending it to backend if I can
