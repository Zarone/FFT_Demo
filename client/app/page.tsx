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
  const [recievedData, changeRecievedData] = useState<ArrayBuffer[]>([]);

  // changes how all of the charts render
  const [rendererScale, setRendererScale] = useState<number>(1);

  const sendData = async (buffer: ArrayBuffer) => {
    console.log(`Emitted Data Transfer with Buffer of Size ${buffer.byteLength}`);
    socket.emit("dataTransfer", new Uint8Array(buffer)); 
    console.group("Printing Header Info");
    console.log(new Uint8Array(buffer));
    console.groupEnd();
  }

  return (
    <div className={styles.container}>
      <FilePicker updateFileData={updateFileData} onPick={sendData}/>
      <Chart 
        data={fileData} 
        rendererScale={rendererScale} 
        setRendererScale={setRendererScale}
      />
      <SocketHandler 
        socket={socket} 
        changeReceivedData={changeRecievedData}
      />
      { 
        recievedData.map((arrData: ArrayBuffer, index: number)=>{
          console.log(index);
          console.log(arrData);
          return <Chart 
            key={index}
            data={arrData} 
            rendererScale={rendererScale} 
            setRendererScale={setRendererScale}
          />
        }) 
      }
    </div>
  );
}
