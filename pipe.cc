#include <node.h>

#if defined _WIN32 || defined _WIN64
   int pipe(int descf[2]) { return -1; }
#else
#  include <unistd.h>
#  include <fcntl.h>
#endif


using namespace v8;


void syspipe(const FunctionCallbackInfo<Value>& args) {

   int desc[2];
   Isolate* isolate = Isolate::GetCurrent();


   if (pipe(desc)) {
      isolate->ThrowException(Exception::Error(
        String::NewFromUtf8(isolate, "Could not create pipe")));
      return;
   }

   Local<Object> obj = Object::New(isolate);
   obj->Set(String::NewFromUtf8(isolate, "read"), v8::Integer::New(isolate, desc[0]));
   obj->Set(String::NewFromUtf8(isolate, "write"), v8::Integer::New(isolate, desc[1]));
   args.GetReturnValue().Set(obj);
}

void init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "pipe", syspipe);
}

NODE_MODULE(syspipe, init)

