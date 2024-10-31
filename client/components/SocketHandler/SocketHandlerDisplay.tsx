import { useState, useEffect } from 'react';
import { SocketHandler } from "./SocketHandler";

interface SocketHandlerDisplayParams {
  socketHandler: SocketHandler
}

export default function SocketHandlerDisplay({socketHandler}: SocketHandlerDisplayParams) {
  const [isConnected, setIsConnected] = useState(socketHandler.isConnected());

  useEffect(() => {
    setIsConnected(socketHandler.isConnected());
    return socketHandler.setConnectionHandler(setIsConnected);
  }, [socketHandler]);

  return <div>
    <p className="text-center"> Connected: {isConnected ? "Connected" : "Not Connected"}</p>
  </div>
}
