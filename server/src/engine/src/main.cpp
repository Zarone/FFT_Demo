#include <node.h>

namespace demo {
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;
    using v8::Array;
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

    // Helper function to convert a C++ vector of integers back to a V8 array
    Local<Array> vectorToV8Array(Isolate* isolate, const std::vector<int>& vec) {
        Local<Array> v8Array = Array::New(isolate, vec.size());
        for (size_t i = 0; i < vec.size(); ++i) {
            v8Array->Set(isolate->GetCurrentContext(), i, Integer::New(isolate, vec[i])).FromJust();
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

    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "method", Method);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
}
