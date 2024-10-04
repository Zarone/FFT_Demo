#include <node.h>

namespace demo {
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;

    void Method(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = args.GetIsolate();
        Local<String> returnValue = String::NewFromUtf8(isolate, "Hello from GFG").ToLocalChecked();
        args.GetReturnValue().Set(returnValue);
    }

    void Initialize(Local<Object> exports) {
        NODE_SET_METHOD(exports, "method", Method);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
}
