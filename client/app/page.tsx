"use client"
import { useState } from "react";
import FilePicker from "@/components/FilePicker/FilePicker";
import styles from "./page.module.css"
import Chart from "../components/Chart/Chart";

function timeout(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function sleep(ms) {
    await timeout(ms);
}

export default function Home() {
  const [fileData, updateFileData] = useState<ArrayBuffer>();

  // changes how all of the charts render
  const [rendererScale, setRendererScale] = useState<number>(2);

  const testFunction = async () => {
    await sleep(3000);
    console.log("FINISHED ASYNC FUNCTION");
  }

  return (
    <div className={styles.container}>
      <FilePicker updateFileData={updateFileData} onPick={testFunction}/>
      <Chart 
        data={fileData} 
        rendererScale={rendererScale} 
        setRendererScale={setRendererScale}
      />  
    </div>
  );
}
