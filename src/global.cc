#include "v8.h"
#include "global.h"

using namespace v8;
using namespace ejsr;

void log(const FunctionCallbackInfo<Value>& args) {
  String::Utf8Value str(args.GetIsolate(), args[0]);
  printf("%s", *str);
  // args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), "A VALUE"));
}

void ejsr::attach_console(Isolate *isolate) {
  Local<ObjectTemplate> console = ObjectTemplate::New(isolate);
  console->Set(String::NewFromUtf8(isolate, "log"),
            FunctionTemplate::New(isolate, log));

  isolate -> GetCurrentContext() -> Global() -> Set(
    String::NewFromUtf8(isolate, "console"),
    console -> NewInstance()
  );
  /**
   * Not sure if required. By all means should be.
   * Apparently my context doesn't die fast enought?
   */
  Eternal<ObjectTemplate> whatever_i_want(isolate, console);
}