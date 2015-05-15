#include <node.h>
#include <nan.h>

#if defined _WIN32 || defined _WIN64
   int pipe(int descf[2]) { return -1; }
#else
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
   extern int errno;
#endif

#define ERRMSG "Could not create pipe"


using namespace v8;


NAN_METHOD(syspipe) {
    NanScope();

    int desc[2];
    char errmsg[128];

    if (pipe(desc)) {
#if defined _WIN32 || defined _WIN64
        return NanThrowError(ERRMSG);
#else
        snprintf(errmsg, sizeof(errmsg), "%s (%s)", ERRMSG, strerror(errno));
        return NanThrowError(errmsg);
#endif
   }

   Local<Object> obj = NanNew<Object>();
   obj->Set(NanNew<String>("read"), NanNew<Integer>(desc[0]));
   obj->Set(NanNew<String>("write"), NanNew<Integer>(desc[1]));
   NanReturnValue(obj);
}

void init(Handle<Object> exports) {
    exports->Set(NanNew<String>("pipe"),
            NanNew<FunctionTemplate>(syspipe)->GetFunction());
}

NODE_MODULE(syspipe, init)

