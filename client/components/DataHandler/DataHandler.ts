export interface DataHandler {
  sendData: (buf: ArrayBuffer) => void;
}
