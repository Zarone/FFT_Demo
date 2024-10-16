import styles from "./LineGraph.module.css"
import { useEffect, useState, useRef, Dispatch, SetStateAction } from 'react';

export interface PathParameters {
  data: number[];
  height: number;
  width: number;
  colorPlayed?: string;
  colorUnplayed?: string;
  timeStamp?: number;
  scale: number;
  setScale?: Dispatch<SetStateAction<number>>;
}

// default min/max has size limit
const customMin = (arr: number[]): number => {
  const len = arr.length;
  let min = Infinity;
  for (let i = 0; i < len; i++) {
    if (arr[i] < min) min = arr[i];
  }
  return min;
}
const customMax = (arr: number[]): number => {
  const len = arr.length;
  let max = -Infinity;
  for (let i = 0; i < len; i++) {
    if (arr[i] > max) max = arr[i];
  }
  return max;
}

const getSvgX = (x: number, length: number, width: number): number => {
  return (x / (length-1) * width);
}

const getSvgY = (y: number, dataMin: number, dataMax: number, height: number): number => {
  const padding = 0.5;
  const min = dataMin- dataMax*padding;
  const max = dataMax+ dataMax*padding;
  return height - ( (y-min) / (max-min) * height);
}

export default function Path({data, height, width, colorPlayed, colorUnplayed, timeStamp, scale}: PathParameters) {
  const dataMin = useRef(0);
  const dataMax = useRef(0);

  const [pathD, setPathD] = useState<string>();

  useEffect(() => {
    console.log("RECALCULATING PATH");

    if (!data || data.length == 0) return;

    dataMin.current = customMin(data);
    dataMax.current = customMax(data);
    const dataLength = data.length;

    const tempPathD = Array(1+data.length);
    tempPathD[0] = 
      "M " + 
        getSvgX(0, dataLength, scale*width) + 
        " " + 
        getSvgY(data[0], dataMin.current, dataMax.current, height) + 
        " ";

    for (let i = 1; i < data.length; i++) {
      tempPathD[i] = 
        "L " + 
          getSvgX(i, dataLength, scale*width) + 
          " " + 
          getSvgY(data[i], dataMin.current, dataMax.current, height) + 
          " ";
    }

    setPathD(tempPathD.join());
  }, [data, height, width, scale])

  if (!pathD || pathD.length == 0) {
    return "";
  }

  timeStamp = timeStamp ?? 1;

  return (
    <>
      <defs>
        <clipPath id="clipPath">
          <rect x="0" y="0" width={timeStamp*scale*width} height={height}/>
        </clipPath>
      </defs>
      <path 
        className={styles["linechart-path"]} 
        d={pathD} 
        fill="none"
        style={{stroke: colorUnplayed || "blue"}} 
      />
      <path 
        className={styles["linechart-path"]} 
        d={pathD} 
        fill="none"
        clipPath="url(#clipPath)"
        style={{stroke: colorPlayed || "red"}} 
      />
    </>
  );

  
}
