import styles from "./LineGraph.module.css"
import { useEffect, useState, useRef } from 'react';

export interface PathParameters {
  data: number[];
  height: number;
  width: number;
  colorPlayed?: string;
  colorUnplayed?: string;
  timeStamp?: number;
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

export default function Path({data, height, width, colorPlayed, colorUnplayed, timeStamp}: PathParameters) {
  const dataMin = useRef(0);
  const dataMax = useRef(0);

  const getSvgX = (x: number): number => {
    return (x / (data.length-1) * width);
  }
  
  const getSvgY = (y: number): number => {
    const padding = 0.5;
    const min = dataMin.current - dataMax.current*padding;
    const max = dataMax.current + dataMax.current*padding;
    return height - ( (y-min) / (max-min) * height);
  }

  const [pathD, setPathD] = useState<string[]>([]);

  useEffect(() => {
    dataMin.current = customMin(data);
    dataMax.current = customMax(data);

    const tempPathD = Array(1+data.length);
    tempPathD[0] = "M " + getSvgX(0) + " " + getSvgY(data[0]) + " ";
    for (let i = 1; i < data.length; i++) {
      tempPathD[i] = "L " + getSvgX(i) + " " + getSvgY(data[i]) + " ";
    }
    setPathD(tempPathD);
  }, [data])

  if (pathD.length == 0) {
    return "";
  }

  timeStamp = timeStamp ?? 1;
  const playedLength = timeStamp*Math.round(data.length);

  let path1 = ""; // played
  let path2 = ""; //unplayed
  if (playedLength == 0) { // if just started
    path2 = pathD.slice(playedLength).join();
    path2 = "M"+path2.slice(1);
  } else if (playedLength >= data.length-1){ // if at end
    path1 = pathD.slice(0, playedLength).join();
  } else {
    path1 = pathD.slice(0, playedLength).join();
    path2 = pathD.slice(playedLength).join();
    path2 = "M"+path2.slice(1);
  }

  return (
    <>
      <path className={styles["linechart-path"]} d={path1} style={{stroke: colorPlayed || "red"}} />
      <path className={styles["linechart-path"]} d={path2} style={{stroke: colorUnplayed || "blue"}} />
    </>
  );

  
}
