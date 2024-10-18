import { useState, useEffect, Dispatch, SetStateAction } from 'react';
import { Socket } from 'socket.io-client';
import {DefaultEventsMap} from "@socket.io/component-emitter";

interface SocketHandlerParams {
  socket: Socket<DefaultEventsMap, DefaultEventsMap>;
  changeReceivedData: Dispatch<SetStateAction<ArrayBuffer[]>>;
}


const sendData = async (socket: Socket, buffer: ArrayBuffer) => {
  console.log(`Emitted Data Transfer with Buffer of Size ${buffer.byteLength}`);
  socket.emit("dataTransfer", new Uint8Array(buffer)); 
  console.group("Printing Header Info");
  console.log(new Uint8Array(buffer));
  console.groupEnd();
}

export const sendDataToSocket = (socket: Socket) => {
  return (buf: ArrayBuffer) => sendData(socket, buf);
}

export default function SocketHandler({socket, changeReceivedData}: SocketHandlerParams) {
  const [isConnected, setIsConnected] = useState(socket.connected);

  useEffect(() => {
    function onConnect() {
      setIsConnected(true);
    }

    function onDisconnect() {
      setIsConnected(false);
    }

    function onDecomposedTransfer(value: ArrayBuffer[]) {
      console.log("Received value from onDecomposedTransfer");
      changeReceivedData(value);

      // CODE TO PRINT FREQUENCIES
      //const threshold = 1000;
      //const temp = (new Uint16Array(value[0]));
      //for (let i = 22; i < temp.length; i++) {
        //if (temp[i] > threshold)
          //console.log(`index: ${i-22}, value: ${temp[i]}, freq: ${(i-22)*8000/(temp.length-22)}`);
      //}

    }

    socket.on('connect', onConnect);
    socket.on('disconnect', onDisconnect);
    socket.on('decomposedTransfer', onDecomposedTransfer);

    return () => {
      socket.off('connect', onConnect);
      socket.off('disconnect', onDisconnect);
      socket.off('decomposedTransfer', onDecomposedTransfer);
    };
  }, []);

  return <div>
    <p className="text-center" >Connected: {isConnected ? "Connected" : "Not Connected"}</p>
  </div>
}
