import { useState, useEffect } from 'react';
import { Socket } from 'socket.io-client';
import {DefaultEventsMap} from "@socket.io/component-emitter";

interface SocketHandlerParams {
  socket: Socket<DefaultEventsMap, DefaultEventsMap>;
}

export default function SocketHandler({socket}: SocketHandlerParams) {
  const [isConnected, setIsConnected] = useState(socket.connected);
  //const [fooEvents, setFooEvents] = useState([]);

  useEffect(() => {
    function onConnect() {
      setIsConnected(true);
    }

    function onDisconnect() {
      setIsConnected(false);
    }

    //function onFooEvent(value) {
      //setFooEvents(previous => [...previous, value]);
    //}

    socket.on('connect', onConnect);
    socket.on('disconnect', onDisconnect);
    //socket.on('foo', onFooEvent);

    return () => {
      socket.off('connect', onConnect);
      socket.off('disconnect', onDisconnect);
      //socket.off('foo', onFooEvent);
    };
  }, []);

  return <div>
    <button onClick={()=>{socket.connect();}}>Connect</button>
    <p>Connected: {isConnected ? "Connected" : "Not Connected"}</p>
  </div>
}
