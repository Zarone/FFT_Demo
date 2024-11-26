"use client"
import { useRef, useState } from "react";
import FilePicker from "@/components/FilePicker/FilePicker";
import styles from "./page.module.css"
import Chart from "../components/Chart/Chart";
import Toggle from "../components/Toggle/Toggle";

import SocketHandlerDisplay from "@/components/SocketHandler/SocketHandlerDisplay";
import { SocketHandler } from "@/components/SocketHandler/SocketHandler";

export default function Home() {
  const [fileData, updateFileData] = useState<ArrayBuffer>(new ArrayBuffer(0));
  const [receivedData, changeReceivedData] = useState<ArrayBuffer[]>([]);
  const [fastEnabled, setFastEnabled] = useState<boolean>(true);

  // changes how all of the charts render
  const [rendererScale, setRendererScale] = useState<number>(1);

  const handlerRef = useRef<SocketHandler>();
  if (!handlerRef.current) {
    handlerRef.current = new SocketHandler(changeReceivedData);
  }

  return (
    <div className={styles.container}>
      <Toggle state={fastEnabled} setState={setFastEnabled}/>

      <SocketHandlerDisplay
        socketHandler={handlerRef.current}
      />

      <FilePicker updateFileData={updateFileData} handler={handlerRef.current} fastEnabled={fastEnabled}/>

      { fileData?.byteLength == 0 ? "" : <header className="m-5 mt-20 text-center text-xl">Algorithm Input: </header> }

      <Chart 
        data={fileData} 
        rendererScale={rendererScale} 
        setRendererScale={setRendererScale}
      />

      { /*receivedData.length == 0 ? "" : <header className="m-5 mt-20 text-center text-xl">Algorithm Return Value: </header>*/ }

      { 
        /*receivedData.map((arrData: ArrayBuffer, index: number)=>{
          return <Chart 
            key={index}
            data={arrData} 
            rendererScale={rendererScale} 
            setRendererScale={setRendererScale}
          />
        }) */
      }

      { receivedData.length == 0 ? "" : 
        <div>
          <header className="m-5 mt-20 text-center text-xl">Frequency Data</header>
          <Chart 
            data={receivedData[0]} 
            rendererScale={rendererScale} 
            setRendererScale={setRendererScale}
          />
          <header className="m-5 mt-20 text-center text-xl">Reconstruction from Inverse Fourier Transform </header>
          <Chart 
            data={receivedData[1]} 
            rendererScale={rendererScale} 
            setRendererScale={setRendererScale}
          />
        </div>
      }
    </div>
  );
}
