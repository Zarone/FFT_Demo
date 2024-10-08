import styles from "./LineGraph.module.css"
import { useEffect, useState } from 'react';

export interface PathParameters {
  data: number[];
  height: number;
  width: number;
  color?: string;
}

export default function Path({data, height, width, color}: PathParameters) {
  const getSvgX = (x: number): number => {
    return (x / data.length * width);
  }
  
  const getSvgY = (y: number): number => {
    return height - (y / Math.max(...data) * height);
  }

  const [pathD, setPathD] = useState<string>("");

  useEffect(() => {
    let tempPathD = "M " + getSvgX(0) + " " + getSvgY(data[0]) + " ";
    tempPathD+= data.map((num, i) => {
      return "L " + getSvgX(i) + " " + getSvgY(num) + " ";
    });
    setPathD(tempPathD);
  }, [data])

  return (
    <path className={styles["linechart_path"]} d={pathD} style={{stroke: color || "blue"}} />
  );

  
}
