export interface DataHandler {
  sendData: (buf: ArrayBuffer, fastEnabled: boolean) => void;
}
