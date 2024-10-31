import { useState, useEffect } from 'react';
import { SocketHandler } from "./SocketHandler";

interface SocketHandlerDisplayParams {
  socketHandler: SocketHandler
}

export default function SocketHandlerDisplay({socketHandler}: SocketHandlerDisplayParams) {
  const [isConnected, setIsConnected] = useState(socketHandler.isConnected());

  useEffect(() => {
    const callback: ()=>void = socketHandler.setConnectionHandler(setIsConnected);
    return () => {
      callback();
    };
  }, [socketHandler]);

  return <div>
    <p className="text-center"> Connected: {isConnected ? "Connected" : "Not Connected"}</p>
  </div>
}
