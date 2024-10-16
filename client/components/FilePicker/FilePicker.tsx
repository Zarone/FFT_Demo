import { ChangeEvent, ChangeEventHandler, useState, Dispatch, SetStateAction } from "react";
import styles from "./FilePicker.module.css"

interface Args {
  onPick: (x: ArrayBuffer) => void;
  updateFileData: Dispatch<SetStateAction<ArrayBuffer|undefined>>;
}

export default function FilePicker( {onPick, updateFileData}: Args ) {

  const handleFileChange: ChangeEventHandler<HTMLInputElement> = async (event: ChangeEvent<HTMLInputElement>) => {
    // If for some reason the file is not found
    if (event.target == null || event.target.files == null || event.target.files.length == 0) {
      alert("No file selected");
      return;
    }

    const file: File = event.target.files[0];

    // If file is not a proper audio file
    if (file.type != "audio/x-wav") {
      console.error(`File was ${file.type}`);
      alert("Please select a .wav audio file");
      return;
    }
    
    const buffer = await file.arrayBuffer()

    console.log("Update File Data");
    updateFileData(buffer);

    console.error("NO LOADING ANIMATION");

    onPick(buffer);

    console.error("NO LOADING ANIMATION");
  }

  return (
    <div className="text-center">
      <div className={styles["f-input-container"]}>
        <input type="file" accept=".wav" onChange={handleFileChange} className={styles["f-input"]}/>
      </div>
    </div>
  );
}
