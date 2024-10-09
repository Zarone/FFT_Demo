/*
  *
  *
  * https://codeburst.io/simple-data-visualization-with-react-js-svg-line-chart-tutorial-df12e5843ce
  * The code here was inspired by Brandon Morelli's code posted in the above link.
  * 
  */

'use client';

import styles from "./LineGraph.module.css"
import Path, { PathParameters } from "./Path"

export default function LineGraph({data, height, width, color}: PathParameters) {

  return <div>
    <svg height={height} width={width} className={styles["linechart-svg"]}>
      <Path data={data} height={height} width={width} color={color}/>
    </svg>
  </div>
}
