'use client';
export default function LineGraph({y} : {y: number[]}) {
  const height: number = 300;
  const width: number = 700;
  
  const getMinX = (): number => {
    return 0;
  }
  const getMaxX = (): number => {
    return y.length;
  }
  const getMinY = (): number => {
    return Math.min(...y);
  }

  const getMaxY = (): number => {
    return Math.max(...y);
  }
  const getSvgX = (x: number): number => {
    return (x / getMaxX() * width);
  }
  const getSvgY = (y: number): number => {
    return height - (y / getMaxY() * height);
  }

  const makePath = (): JSX.Element => {
    let pathD: string = "M " + getSvgX(0) + " " + getSvgY(y[0]) + " ";
    pathD += y.map((num, i) => {
      return "L " + getSvgX(i) + " " + getSvgY(num) + " ";
    });
    return (
      <path className="linechart_path" d={pathD} style={{stroke: "blueviolet"}} />
    );
  }

  const makeAxis = (): JSX.Element => {
    const minX = getMinX(), maxX = getMaxX();
    const minY = getMinY(), maxY = getMaxY();
    return (
      <g className="linechart_axis">
        <line
          x1={getSvgX(minX)} y1={getSvgY(minY)}
          x2={getSvgX(maxX)} y2={getSvgY(minY)} />
        <line
          x1={getSvgX(minX)} y1={getSvgY(minY)}
          x2={getSvgX(minX)} y2={getSvgY(maxY)} />
      </g>
    );
  }
  
  return <div>
    <svg color={"blue"} height={height} width={width}>
      {makePath()}
      {makeAxis()}
    </svg>
  </div>
}
