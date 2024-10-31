"use client"
import { useRef, useState } from "react";
import FilePicker from "@/components/FilePicker/FilePicker";
import styles from "./page.module.css"
import Chart from "../components/Chart/Chart";

import SocketHandlerDisplay from "@/components/SocketHandler/SocketHandlerDisplay";
import { SocketHandler } from "@/components/SocketHandler/SocketHandler";

export default function Home() {
  const [fileData, updateFileData] = useState<ArrayBuffer>(new ArrayBuffer(0));
  const [receivedData, changeReceivedData] = useState<ArrayBuffer[]>([]);

  // changes how all of the charts render
  const [rendererScale, setRendererScale] = useState<number>(1);

  const handlerRef = useRef<SocketHandler>(new SocketHandler(changeReceivedData));

  return (
    <div className={styles.container}>
      <SocketHandlerDisplay
        socketHandler={handlerRef.current}
      />

      <FilePicker updateFileData={updateFileData} handler={handlerRef.current}/>

      { fileData?.byteLength == 0 ? "" : <header className="m-5 mt-20 text-center text-xl">Algorithm Input: </header> }

      <Chart 
        data={fileData} 
        rendererScale={rendererScale} 
        setRendererScale={setRendererScale}
      />

      { receivedData.length == 0 ? "" : <header className="m-5 mt-20 text-center text-xl">Algorithm Return Value: </header> }

      { 
        receivedData.map((arrData: ArrayBuffer, index: number)=>{
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
