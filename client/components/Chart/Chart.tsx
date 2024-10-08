'use client';

import LineGraph from "./LineGraph";

export default function Chart({y} : {y: number[]}) {
  return <div>
    <LineGraph y={y} />
  </div>

}
