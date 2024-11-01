import { io, Socket } from 'socket.io-client';
import { Dispatch, SetStateAction } from 'react';
import { DataHandler } from '../DataHandler/DataHandler';

// "undefined" means the URL will be computed from the `window.location` object
const URL = process.env.NODE_ENV === 'production' ? undefined : 'http://localhost:3001';

export class SocketHandler implements DataHandler {
  socket: Socket;

  constructor(changeReceivedData: Dispatch<SetStateAction<ArrayBuffer[]>>) {
    this.socket = io(URL, {
      withCredentials: true, // Ensure credentials are sent with CORS
      transports: ['websocket', 'polling'], // Use both websocket and polling transports
      timeout: 500000,
      ackTimeout: 500000,
    });

    this.setupSocket(changeReceivedData);
  }

  private setupSocket = (changeReceivedData: Dispatch<SetStateAction<ArrayBuffer[]>>) => {
    function onDecomposedTransfer(value: ArrayBuffer[]) {
      console.log("Received value from onDecomposedTransfer");
      changeReceivedData(value);

      console.log(new Int16Array(value[0]));
      console.log(new Int16Array(value[1]));
      // CODE TO PRINT FREQUENCIES
      //const threshold = 1000;
      //const temp = (new Uint16Array(value[0]));
      //for (let i = 22; i < temp.length; i++) {
        //if (temp[i] > threshold)
          //console.log(`index: ${i-22}, value: ${temp[i]}, freq: ${(i-22)*8000/(temp.length-22)}`);
      //}
    }

    this.socket.on('decomposedTransfer', onDecomposedTransfer);
  }

  public sendData = async (buffer: ArrayBuffer) => {
    console.log(`Emitted Data Transfer with Buffer of Size ${buffer.byteLength}`);
    this.socket.emit("dataTransfer", new Uint8Array(buffer)); 
    console.group("Printing Header Info");
    console.log(new Uint8Array(buffer));
    console.groupEnd();
  }
  
  public isConnected = (): boolean => {
    return this.socket.connected;
  }

  public setConnectionHandler = (setIsConnected: Dispatch<SetStateAction<boolean>>): (()=>void) => {
    function onConnect() {
      setIsConnected(true);
    }

    function onDisconnect() {
      setIsConnected(false);
    }

    this.socket.on('connect', onConnect);
    this.socket.on('disconnect', onDisconnect);

    return () => {
      this.socket.off('connect', onConnect);
      this.socket.off('disconnect', onDisconnect);
    }

  }

}
