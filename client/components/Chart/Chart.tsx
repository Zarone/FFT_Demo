'use client';

import AudioElement from "../AudioElement/AudioElement";
import LineGraph from "./LineGraph";
import { useEffect, useRef, useState } from "react";

const convertRawBufferToWaveform = (wavData: ArrayBuffer): number[] => {
  // Create a DataView to read the byte data
  const dataView = new DataView(wavData);
  
  const bitsPerSample: number = dataView.getInt16(34, true);
  const bytesPerSample: number = bitsPerSample/8
  if (bitsPerSample != 16) throw new Error(`Whoops! Didn't expect ${bitsPerSample} bits per sample`);

  //const sampleRate = dataView.getUint32(24, true); // Sample rate at byte offset 24

  // Number of samples at byte offset 40 (divided by 2 for 16-bit)
  const numSamples: number = dataView.getUint32(40, true) / (bytesPerSample); 
  const waveform: number[] = Array(numSamples); // Array to hold normalized amplitude values

  const startOffset = 44; // 44 bits at start of wav file are header data

  for (let i = 0; i < numSamples; i++) {

    // Get the 16-bit signed integer value (2 bytes)
    const sample: number = dataView.getInt16(startOffset+bytesPerSample*i, true);

    // Normalize to range -1.0 to 1.0
    waveform[i] = sample / Math.pow(2, bitsPerSample-1);
  }
  
  return waveform;
}

export default function Chart({data} : {data: ArrayBuffer | undefined}) {
  const [timeStamp, setTimeStamp] = useState(0);

  return <div>
    {data==undefined ? "" : 
      <div>
        <div className="text-center flex align-middle">
          <AudioElement data={data} timeStamp={timeStamp} setTimeStamp={setTimeStamp} />
          <LineGraph timeStamp={timeStamp} data={convertRawBufferToWaveform(data)} height={300} width={700}/>
        </div>
      </div>
    }
  </div>
}
