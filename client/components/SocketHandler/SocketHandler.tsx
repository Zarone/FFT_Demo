import { useState, useEffect, Dispatch, SetStateAction } from 'react';
import { Socket } from 'socket.io-client';
import {DefaultEventsMap} from "@socket.io/component-emitter";

interface SocketHandlerParams {
  socket: Socket<DefaultEventsMap, DefaultEventsMap>;
  changeReceivedData: Dispatch<SetStateAction<ArrayBuffer[]>>;
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
    <button onClick={()=>{socket.connect();}}>Connect</button>
    <p>Connected: {isConnected ? "Connected" : "Not Connected"}</p>
  </div>
}
