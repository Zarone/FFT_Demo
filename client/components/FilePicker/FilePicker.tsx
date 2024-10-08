import { ChangeEvent, ChangeEventHandler, useState } from "react";
import Chart from "../Chart/Chart";

function getInterpolation(arr: Uint8Array, maxOutputLength: number) {
  // I want to make this effectively shrink the dataset
  // by averaging the datapoints such that the output
  // has length < maxOutputLength

  // for now though I'll just do this
  return Array.from(arr.subarray(0, maxOutputLength));

}

export default function FilePicker() {

  const [fileData, updateFileData] = useState<Uint8Array>();

  const handleFileChange: ChangeEventHandler<HTMLInputElement> = async (event: ChangeEvent<HTMLInputElement>) => {
    // If for some reason the file is not found
    if (event.target == null || event.target.files == null || event.target.files.length == 0) {
      alert("No file selected");
      return;
    }

    const file: File = event.target.files[0];

    // If file is not a proper audio file
    if (file.type != "audio/mpeg") {
      alert("Please select a .mp3 audio file");
      return;
    }

    const buffer = await file.arrayBuffer()
    const uint8 = new Uint8Array(buffer)

    updateFileData(uint8);
  }

  return (
    <div>
      <input type="file" accept=".mp3" onChange={handleFileChange}/>
      <Chart data={fileData ? getInterpolation(fileData, 100) : []} />  
    </div>
  );
}
