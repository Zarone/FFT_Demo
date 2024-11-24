import { ChangeEvent, ChangeEventHandler, useState, Dispatch, SetStateAction } from "react";
import styles from "./FilePicker.module.css";
import { DataHandler } from "../DataHandler/DataHandler";

interface Args {
  handler: DataHandler;
  updateFileData: Dispatch<SetStateAction<ArrayBuffer>>;
}

export default function FilePicker( {handler, updateFileData}: Args ) {

  const handleFileChange: ChangeEventHandler<HTMLInputElement> = async (event: ChangeEvent<HTMLInputElement>) => {
    // If for some reason the file is not found
    if (event.target == null || event.target.files == null || event.target.files.length == 0) {
      alert("No file selected");
      return;
    }

    const file: File = event.target.files[0];

    // If file is not a proper audio file
    if (file.type != "audio/x-wav" && file.type != "audio/wav") {
      console.error(`File was ${file.type}`);
      alert("Please select a .wav audio file");
      return;
    }
    
    const buffer: ArrayBuffer = await file.arrayBuffer()

    console.log("Update File Data");
    console.group("Printing Header Info");
    console.log(new Uint16Array(buffer.slice(0, 44)));
    console.groupEnd();

    updateFileData(buffer);

    console.error("TODO: ADD LOADING ANIMATION");

    handler.sendData(buffer);
  }

  return (
    <div className="text-center">
      <div className={styles["f-input-container"]}>
        <input type="file" accept=".wav" onChange={handleFileChange} className={styles["f-input"]}/>
      </div>
    </div>
  );
}
