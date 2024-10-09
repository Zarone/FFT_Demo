import { useState, useEffect, useRef } from 'react';
import styles from "./AudioElement.module.css"

export default function AudioElement({data}) {

  const [isPlaying, setIsPlaying] = useState(false);
  const audioSrc = useRef<string|undefined>("");
  const audioRef = useRef<HTMLAudioElement>(null);

  useEffect(()=>{
    const blob: Blob|undefined = data ? new Blob([data], { type: "audio/wav" }) : undefined;
    audioSrc.current = blob ? URL.createObjectURL(blob) : undefined;
  }, [data])

  useEffect(()=>{
    console.log("isPlaying changed");
    if (!audioRef.current) return;
    if (isPlaying) audioRef.current.play();
    else { audioRef.current.pause(); }
  }, [isPlaying]);

  return <>
    <audio hidden src={audioSrc.current} controls ref={audioRef}/>
    <div className={styles["proxy-audio-player"]} onClick={()=>{console.log("onclick");setIsPlaying(!isPlaying);}}>
      <svg viewBox="0 0 24 24" preserveAspectRatio="xMidYMid meet" className={styles["play-button"]}>
        {isPlaying ? <path d="M6 19h4V5H6zm8-14v14h4V5z" fill="lightgray"/> : <path d="M8 5v14l11-7z" fill="lightgray"/> }
      </svg>
    </div>
  </>
}
