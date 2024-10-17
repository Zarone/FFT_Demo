#include <node.h>
#include "transform.h"

namespace demo {
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::Array;
  using v8::ArrayBuffer;
  using v8::Integer;
  using v8::Exception;

  // Helper function to convert a V8 array to a C++ vector of integers
  std::vector<int> v8ArrayToVector(Local<Array> array) {
    std::vector<int> vec;
    for (uint32_t i = 0; i < array->Length(); ++i) {
      vec.push_back(array->Get(array->GetIsolate()->GetCurrentContext(), i).ToLocalChecked()->Int32Value(array->GetIsolate()->GetCurrentContext()).FromJust());
    }
    return vec;
  }

  // Helper function to convert a V8 ArrayBuffer to a C++ pointer
  int16_t* v8ArrayToData(Local<ArrayBuffer> array) {
    return (int16_t*)array->GetBackingStore()->Data();
  }

  // Helper function to convert a C++ vector of integers back to a V8 array
  Local<Array> vectorToV8Array(Isolate* isolate, const std::vector<int>& vec) {
    Local<Array> v8Array = Array::New(isolate, vec.size());
    for (size_t i = 0; i < vec.size(); ++i) {
      v8Array->Set(isolate->GetCurrentContext(), i, Integer::New(isolate, vec[i])).FromJust();
    }
    return v8Array;
  }

  // Helper function to convert a C++ int16 array to a V8 array
  Local<ArrayBuffer> arrToV8Array(Isolate* isolate, int16_t* arr, std::size_t len) {
    Local<ArrayBuffer> v8Array = ArrayBuffer::New(isolate, len);
    for (size_t i = 0; i < len; ++i) {
      v8Array->Set(isolate->GetCurrentContext(), i, Integer::New(isolate, arr[i])).FromJust();
    }
    return v8Array;
  }

  // Helper function to convert a C++ int16 array to a V8 array of buffers
  Local<Array> arrToV8ArrayOfBuffers(Isolate* isolate, int16_t** arr, std::size_t bufferLen, std::size_t arrLen) {
    Local<Array> v8Array = Array::New(isolate, arrLen);
    for (std::size_t i = 0; i < arrLen; ++i) {

      // Calculate the byte size of the int16_t array
      size_t byte_length = bufferLen * sizeof(int16_t);
      
      // Create an external backing store that doesn't copy the data
      std::unique_ptr<v8::BackingStore> backing_store = v8::ArrayBuffer::NewBackingStore(
          arr[i], 
          byte_length, 
          v8::BackingStore::EmptyDeleter,  // No deleter because we don't want to free the memory
          nullptr  // No user data
      );

      Local<ArrayBuffer> buffer = ArrayBuffer::New(isolate, std::move(backing_store));
      v8Array->Set(isolate->GetCurrentContext(), i, buffer).FromJust();
    }
    return v8Array;
  }

  // Function to increment each element of the array by 1
  void Method(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    v8::HandleScope handleScope(isolate);

    if (!args[0]->IsArray()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8Literal(isolate, "Argument must be an array")));
      return;
    }

    Local<Array> inputArray = Local<Array>::Cast(args[0]);
    std::vector<int> inputVec = v8ArrayToVector(inputArray);

    // Increment each element by 1
    for (int& num : inputVec) {
      ++num;
    }

    // Return the modified array
    args.GetReturnValue().Set(vectorToV8Array(isolate, inputVec));
  }

  void ZeroArrayBuffer(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    v8::HandleScope handleScope(isolate);

    if (!args[0]->IsArrayBuffer()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8Literal(isolate, "Argument must be an ArrayBuffer")));
      return;
    }
    
    Local<ArrayBuffer> localArr = Local<ArrayBuffer>::Cast(args[0]);
    int16_t* arr = v8ArrayToData(localArr);
    std::size_t len = localArr->ByteLength()/2; // divide by 2 because it's int16

    for (std::size_t i = 0; i < len; i++) {
      arr[i] = 0;
    }
    
    args.GetReturnValue().Set(arrToV8Array(isolate, arr, len));
    
  }

  void TransformWavFileBuffer(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    v8::HandleScope handleScope(isolate);

    if (!args[0]->IsArrayBuffer()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8Literal(isolate, "Argument must be an ArrayBuffer")));
      return;
    }
    
    Local<ArrayBuffer> localArr = Local<ArrayBuffer>::Cast(args[0]);
    int16_t* arr = v8ArrayToData(localArr);
    std::size_t len = localArr->ByteLength()/2; // divide by 2 because it's int16

    int16_t** response = transformWAVData(arr, len);
    
    args.GetReturnValue().Set(arrToV8ArrayOfBuffers(isolate, response, len, 1));
    
  }

  void Initialize(Local<Object> exports) {
    NODE_SET_METHOD(exports, "method", Method);
    NODE_SET_METHOD(exports, "zeroArrayBuffer", ZeroArrayBuffer);
    NODE_SET_METHOD(exports, "transformWavFileBuffer", TransformWavFileBuffer);
  }

  NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
}
