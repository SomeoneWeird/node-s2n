#include "node-s2nconfig.h"

#include <node.h>
#include <s2n.h>

using namespace v8;

Persistent<Function> S2NConfig::constructor;

S2NConfig::S2NConfig() {
  s2n_config *s2nconfig = s2n_config_new();
  if(!s2nconfig) {
    NanThrowError("Error getting new S2N config");
  }
}

S2NConfig::~S2NConfig() {
  s2n_config_free(s2nconfig);
  free(&version);
  free(&certificate);
  free(&privateKey);
}

void S2NConfig::Init(Handle<Object> exports) {
  NanScope();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("S2NConfig"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "setStatusRequestType", SetStatusRequestType);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setCipherPreferences", SetCipherPreferences);
  NODE_SET_PROTOTYPE_METHOD(tpl, "addCertChainAndKey", AddCertChainAndKey);

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

NAN_METHOD(S2NConfig::SetStatusRequestType) {

  NanScope();

  if(args.Length() < 1 || !args[0]->IsNumber()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  // TODO: check type is valid
  s2n_status_request_type type = static_cast<s2n_status_request_type>(args[0]->IntegerValue());

  S2NConfig* self = ObjectWrap::Unwrap<S2NConfig>(args.Holder());

  int result = s2n_config_set_status_request_type(self->s2nconfig, type);

  if(result < 0) {
    NanThrowError("Error setting config status request type");
  }

  NanReturnValue(NanNew(true));

}

NAN_METHOD(S2NConfig::SetCipherPreferences) {

  NanScope();

  if(args.Length() < 1 || !args[0]->IsString()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  S2NConfig* self = ObjectWrap::Unwrap<S2NConfig>(args.Holder());

  std::string arg0 = *NanUtf8String(args[0]);

  self->version = arg0.c_str();

  int result = s2n_config_set_cipher_preferences(self->s2nconfig, self->version);

  if(result < 0) {
    NanThrowError("Error setting config cipher preferences");
  }

  NanReturnValue(NanNew(true));

}

NAN_METHOD(S2NConfig::AddCertChainAndKey) {

  NanScope();

  if(args.Length() < 1) {
    NanThrowTypeError("Wrong number of arguments");
    NanReturnUndefined();
  }

  if(!args[0]->IsString()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  S2NConfig* self = ObjectWrap::Unwrap<S2NConfig>(args.Holder());

  std::string arg0 = *NanUtf8String(args[0]);
  std::string arg1 = *NanUtf8String(args[1]);

  self->certificate = strdup(arg0.c_str());
  self->privateKey  = strdup(arg1.c_str());

  int result = s2n_config_add_cert_chain_and_key(self->s2nconfig, self->certificate, self->privateKey);

  if(result < 0) {
    NanThrowError("Error adding certificate chain and key");
    NanReturnUndefined();
  }

  NanReturnValue(NanNew(true));

}
