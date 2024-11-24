/*
  *
  *
  * https://codeburst.io/simple-data-visualization-with-react-js-svg-line-chart-tutorial-df12e5843ce
  * The code here was inspired by Brandon Morelli's code posted in the above link.
  * 
  */

'use client';

import { useCallback, useEffect, useRef } from "react";
import styles from "./LineGraph.module.css"
import Path, { PathParameters } from "./Path"

export default function LineGraph(
  {data, height, width, colorPlayed, colorUnplayed, timeStamp, scale, setScale}: PathParameters
) {
  const listenerRef = useRef<SVGSVGElement>(null);

  const scroll = useCallback((e: WheelEvent) => {
    if (!setScale) return;
    setScale(scale=>Math.max(0.5, scale*(1+e.deltaY/500)));
    e.preventDefault();
    e.stopPropagation();
  }, [setScale]);

  useEffect(() => {
    if (!listenerRef.current) return;
    const el = listenerRef.current;
    el.addEventListener('wheel', scroll, {passive: false});
    return () => {
      el.removeEventListener('wheel', scroll);
    };
  }, [listenerRef, scroll]);

  return <div>
    <svg ref={listenerRef} height={height} width={width} className={styles["linechart-svg"]}>
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
