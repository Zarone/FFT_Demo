/*
  *
  *
  * https://codeburst.io/simple-data-visualization-with-react-js-svg-line-chart-tutorial-df12e5843ce
  * The code here was inspired by Brandon Morelli's code posted in the above link.
  * 
  */

'use client';

import { WheelEvent } from "react";
import styles from "./LineGraph.module.css"
import Path, { PathParameters } from "./Path"

export default function LineGraph(
  {data, height, width, colorPlayed, colorUnplayed, timeStamp, scale, setScale, offset}: PathParameters
) {
  const scroll = (e: WheelEvent<SVGSVGElement>) => {
    if (!setScale) return;
    setScale(Math.max(0.5, scale*(1+e.deltaY/50)));
    e.preventDefault();
    e.stopPropagation();
  }

  return <div>
    <svg onWheel={scroll} height={height} width={width} className={styles["linechart-svg"]}>
      <Path 
        data={data} 
        height={height} 
        width={width} 
        colorPlayed={colorPlayed} 
        colorUnplayed={colorUnplayed} 
        timeStamp={timeStamp}
        scale={scale}
      />
    </svg>
  </div>
}
