#include "node-s2nconnection.h"

using namespace v8;

Persistent<Function> S2NConnection::constructor;

S2NConnection::S2NConnection() {
}

S2NConnection::~S2NConnection() {
}

void S2NConnection::Init(Handle<Object> exports) {
  NanScope();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("S2NConnection"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NanAssignPersistent(constructor, tpl->GetFunction());
  exports->Set(NanNew("S2NConnection"), tpl->GetFunction());

}

NAN_METHOD(S2NConnection::New) {
  NanScope();

  if (args.IsConstructCall()) {
    S2NConnection* obj = new S2NConnection();
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = NanNew<Function>(constructor);
    NanReturnValue(cons->NewInstance(argc, argv));
  }
}
