'use client';
import FilePicker from "@/components/FilePicker/FilePicker";
import styles from "./page.module.css"

function timeout(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

async function sleep(ms) {
    await timeout(ms);
}

export default function Home() {
  const testFunction = async () => {
    await sleep(3000);
    console.log("FINISHED ASYNC FUNCTION");
  }

  return (
    <div className={styles.container}>
      <FilePicker onPick={testFunction}/>
    </div>
  );
}
