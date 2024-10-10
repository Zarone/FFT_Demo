import { useState, useEffect, useRef, Dispatch, SetStateAction } from 'react';
import styles from "./AudioElement.module.css"

interface AudioElementParams {
  data: ArrayBuffer;
  setTimeStamp: Dispatch<SetStateAction<number>>;
  timeStamp: number;
}

export default function AudioElement({data, setTimeStamp, timeStamp}: AudioElementParams) {

  const [isPlaying, setIsPlaying] = useState(false);
  const audioSrc = useRef<string|undefined>("");
  const audioRef = useRef<HTMLAudioElement>(null);

  useEffect(()=>{
    const blob: Blob|undefined = data ? new Blob([data], { type: "audio/wav" }) : undefined;
    audioSrc.current = blob ? URL.createObjectURL(blob) : undefined;
  }, [data])

  useEffect(()=>{

    if (!audioRef.current) return;
    if (isPlaying) audioRef.current.play();
    else { audioRef.current.pause(); }

    const timeUpdateFunction = () => {
      if ( audioRef.current?.duration == undefined || isNaN(audioRef.current?.duration) ) {
        if (isPlaying) requestAnimationFrame(timeUpdateFunction);
        return;
      }

      const duration = audioRef.current?.duration;
      const newTimeStamp = audioRef.current?.currentTime;
      const newVal = newTimeStamp/duration;

      if (newVal >= 0.999) {
        setTimeStamp(0);
        setIsPlaying(false);
        requestAnimationFrame(timeUpdateFunction);
      } else if (newVal <= 0.01 || newVal > timeStamp) {
        setTimeStamp(newVal);
        requestAnimationFrame(timeUpdateFunction);
      }

    }

    const frame = requestAnimationFrame(timeUpdateFunction);

    return () => {
      cancelAnimationFrame(frame);
    }
  }, [isPlaying]);

  return <>
    <audio src={audioSrc.current} controls ref={audioRef}/>
    <div className={styles["proxy-audio-player"]} onClick={()=>{setIsPlaying(!isPlaying);}}>
      <svg viewBox="0 0 24 24" preserveAspectRatio="xMidYMid meet" className={styles["play-button"]}>
        {isPlaying ? <path d="M6 19h4V5H6zm8-14v14h4V5z" fill="lightgray"/> : <path d="M8 5v14l11-7z" fill="lightgray"/> }
      </svg>
    </div>
  </>
}
