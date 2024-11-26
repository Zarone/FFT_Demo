import { Dispatch, SetStateAction } from "react";
import styles from "./Toggle.module.css";

interface ToggleParams {
  state: boolean;
  setState: Dispatch<SetStateAction<boolean>>;
}

export default function Toggle({state, setState}: ToggleParams) {
  return (
    <div className={styles["toggle-container"]}>
      <label>
        <input type="checkbox" checked={state} onChange={()=>setState(state=>!state)}/>
        <span>Use Fast Fourier Transform (as opposed to Discrete Fourier Transform)</span>
      </label>
    </div>
  )
}
