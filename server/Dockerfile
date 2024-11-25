# Use an official Node.js runtime as the base image
FROM node:23

# Set the working directory in the container
WORKDIR /app

# Copy package.json and package-lock.json into the working directory
COPY package*.json ./

ENV CXX g++

# Install dependencies
RUN npm install

# Copy the rest of the application’s source code to the container
COPY . .

# Expose the application’s port (assuming it listens on port 3001)
EXPOSE 3001

# Run build step
RUN npm run build

# Remove dev dependencies after building
RUN npm prune --production

CMD ["npm", "run", "start"]


# DEBUGGING STUFF
#RUN apt-get update && apt-get install -y gdb
#CMD ["gdb", "--args", "node", "./build/index.js"]

