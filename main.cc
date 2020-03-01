#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

#include "libplatform/libplatform.h"
#include "v8.h"

void log(const v8::FunctionCallbackInfo<v8::Value>& args) {
  bool isString = args[0] -> IsString();
  printf("Is argument string %d\n", isString);
  args.GetReturnValue().Set(v8::String::NewFromUtf8(args.GetIsolate(), "A VALUE"));
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("No JS provided\n");
    exit(1);
  }

  std::ifstream jsFile;
  jsFile.unsetf(std::ios_base::skipws);
  jsFile.open(argv[1]);

  if (!jsFile) {
    printf("No JS file opened\n");
    exit(1);
  }

  std::string js;
  while (true)
  {
    std::string partial;
    if (std::getline(jsFile,partial,'\n')) {
      js += "\n";
      js += partial;
    } else {
      break;
    }
  }
  
  jsFile.close();

  // Initialize V8.
  v8::V8::InitializeICUDefaultLocation(argv[0]);
  v8::V8::InitializeExternalStartupData(argv[0]);
  std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
  v8::V8::InitializePlatform(platform.get());
  v8::V8::Initialize();

  // Create a new Isolate and make it the current one.
  v8::Isolate::CreateParams create_params;
  create_params.array_buffer_allocator =
      v8::ArrayBuffer::Allocator::NewDefaultAllocator();
  v8::Isolate* isolate = v8::Isolate::New(create_params);
  {
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    // Create a new context.
    v8::Local<v8::Context> context = v8::Context::New(isolate);

    // Enter the context for compiling and running the hello world script.
    v8::Context::Scope context_scope(context);

    v8::Local<v8::FunctionTemplate> function_template = v8::FunctionTemplate::New(isolate, log);
    isolate -> GetCurrentContext() -> Global() -> Set(v8::String::NewFromUtf8(isolate, "log"), function_template -> GetFunction());

    {
      // Create a string containing the JavaScript source code.
      v8::Local<v8::String> source =
          v8::String::NewFromUtf8(isolate, js.c_str(),
                                  v8::NewStringType::kNormal)
              .ToLocalChecked();

      // Compile the source code.
      v8::Local<v8::Script> script =
          v8::Script::Compile(context, source).ToLocalChecked();

      // Run the script to get the result.
      v8::Local<v8::Value> result = script->Run(context).ToLocalChecked();
    }
  }

  // Dispose the isolate and tear down V8.
  isolate->Dispose();
  v8::V8::Dispose();
  v8::V8::ShutdownPlatform();
  delete create_params.array_buffer_allocator;
  return 0;
}
