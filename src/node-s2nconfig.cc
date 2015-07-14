#include "node-s2nconfig.h"

using namespace v8;

Persistent<Function> S2NConfig::constructor;

S2NConfig::S2NConfig() {
}

S2NConfig::~S2NConfig() {
}

void S2NConfig::Init(Handle<Object> exports) {
  NanScope();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("S2NConfig"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NanAssignPersistent(constructor, tpl->GetFunction());
  exports->Set(NanNew("S2NConfig"), tpl->GetFunction());

}

NAN_METHOD(S2NConfig::New) {
  NanScope();

  if (args.IsConstructCall()) {
    S2NConfig* obj = new S2NConfig();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = NanNew<Function>(constructor);
    NanReturnValue(cons->NewInstance(argc, argv));
  }
}
