#include <node.h>
#include <cstring>
#include "transform.h"

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Array;
using v8::ArrayBuffer;
using v8::Exception;
using std::vector;

// Helper function to convert a V8 ArrayBuffer to a C++ vector<int16_t>
vector<int16_t> v8ArrayToData(const Local<ArrayBuffer>& array) {
  size_t len = array->ByteLength()/sizeof(int16_t);
  int16_t* raw = static_cast<int16_t*>(array->GetBackingStore()->Data());
  return vector<int16_t>(raw, raw+len);
}


// Helper function to convert a C++ vector<int16_t> to a V8 array of buffers
Local<Array> arrToV8ArrayOfBuffers(Isolate* isolate, const vector<vector<int16_t>>& arr) {
  size_t arrLen = arr.size();
  Local<Array> v8Array = Array::New(isolate, arrLen);
  for (size_t i = 0; i < arrLen; ++i) {

    // Calculate the byte size of the int16_t array
    size_t byte_length = arr[i].size() * sizeof(int16_t);
    
    // Create an external backing store
    std::unique_ptr<v8::BackingStore> backing_store = v8::ArrayBuffer::NewBackingStore(
        std::malloc(byte_length), 
        byte_length, 
        [](void* data, size_t, void*) { std::free(data); }, 
        nullptr  // No user data
    );

    // Copy the data from arr[i].data() to the new backing store
    std::memcpy(backing_store->Data(), arr[i].data(), byte_length);

    Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, std::move(backing_store));
    v8Array->Set(isolate->GetCurrentContext(), i, buffer).FromJust();
  }
  return v8Array;
}

void TransformWavFileBuffer(const FunctionCallbackInfo<Value>& args, bool fast) {
  Isolate* isolate = args.GetIsolate();
  v8::HandleScope handleScope(isolate);

  if (!args[0]->IsArrayBuffer()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8Literal(isolate, "Argument must be an ArrayBuffer")));
    return;
  }
  
  // Get local v8 ArrayBuffer
  Local<ArrayBuffer> localArr = Local<ArrayBuffer>::Cast(args[0]);

  vector<int16_t> arr = v8ArrayToData(localArr);

  vector<vector<int16_t>> response = transformWAVData(arr, fast);
  
  args.GetReturnValue().Set(arrToV8ArrayOfBuffers(isolate, response));
}

void TransformWavFileBufferDFT(const FunctionCallbackInfo<Value>& args) {
  TransformWavFileBuffer(args, false);
}

void TransformWavFileBufferFFT(const FunctionCallbackInfo<Value>& args) {
  TransformWavFileBuffer(args, true);
}

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "transformWavFileBufferDFT", TransformWavFileBufferDFT);
  NODE_SET_METHOD(exports, "transformWavFileBufferFFT", TransformWavFileBufferFFT);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
