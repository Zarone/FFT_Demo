import styles from "./LineGraph.module.css"
import { useEffect, useState, useRef } from 'react';

export interface PathParameters {
  data: number[];
  height: number;
  width: number;
  color?: string;
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

export default function Path({data, height, width, color}: PathParameters) {
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

  const [pathD, setPathD] = useState<string>("");

  useEffect(() => {
    dataMin.current = customMin(data);
    dataMax.current = customMax(data);

    let tempPathD = "M " + getSvgX(0) + " " + getSvgY(data[0]) + " ";
    tempPathD += data.map((num, i) => {
      return "L " + getSvgX(i) + " " + getSvgY(num) + " ";
    });
    setPathD(tempPathD);
  }, [data])

  return (
    <path className={styles["linechart-path"]} d={pathD} style={{stroke: color || "blue"}} />
  );

  
}
