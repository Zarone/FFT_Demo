'use client';

import LineGraph from "./LineGraph";

export default function Chart({data} : {data: number[]}) {
  return <div className="text-center" >
    {data.length == 0 ? "" : <LineGraph data={data} height={300} width={700}/>}
  </div>

}
