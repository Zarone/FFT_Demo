"use client"
import { useState } from "react";
import FilePicker from "@/components/FilePicker/FilePicker";
import styles from "./page.module.css"
import Chart from "../components/Chart/Chart";

// A socket is made automatically here
import { socket } from "../components/Socket/Socket";
import SocketHandler, {sendDataToSocket} from "@/components/SocketHandler/SocketHandler";

export default function Home() {
  const [fileData, updateFileData] = useState<ArrayBuffer>();
  const [recievedData, changeRecievedData] = useState<ArrayBuffer[]>([]);

  // changes how all of the charts render
  const [rendererScale, setRendererScale] = useState<number>(1);

  return (
    <div className={styles.container}>
      <SocketHandler 
        socket={socket} 
        changeReceivedData={changeRecievedData}
      />
      <FilePicker updateFileData={updateFileData} onPick={sendDataToSocket(socket)}/>
      <Chart 
        data={fileData} 
        rendererScale={rendererScale} 
        setRendererScale={setRendererScale}
      />
      { 
        recievedData.map((arrData: ArrayBuffer, index: number)=>{
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
