"use client"
import { useState } from "react";
import FilePicker from "@/components/FilePicker/FilePicker";
import styles from "./page.module.css"
import Chart from "../components/Chart/Chart";

// A socket is made automatically here
import { socket } from "../components/Socket/Socket";
import SocketHandler from "@/components/SocketHandler/SocketHandler";

export default function Home() {
  const [fileData, updateFileData] = useState<ArrayBuffer>();

  // changes how all of the charts render
  const [rendererScale, setRendererScale] = useState<number>(1);

  const sendData = async (buffer: ArrayBuffer) => {
    socket.emit("dataTransfer", buffer); 
  }

  return (
    <div className={styles.container}>
      <FilePicker updateFileData={updateFileData} onPick={sendData}/>
      <Chart 
        data={fileData} 
        rendererScale={rendererScale} 
        setRendererScale={setRendererScale}
      />  
      <SocketHandler socket={socket} />
    </div>
  );
}
