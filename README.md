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

### TODO
- Send audio file data to backend and establish socket
- Process audio data in backend
  - Split audio into its components
- Return audio components to front end
  - Render audio components on front end once returned from backend
- Remerge selected audio components by sending to backend
- Ideally:
  - Time the amount of time it takes to run FFT
  - Time the amount of time it takes to run DFT
