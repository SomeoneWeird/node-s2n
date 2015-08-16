#include "node-s2nconfig.h"

#include <node.h>
#include <s2n.h>

using namespace v8;

Nan::Persistent<Function> S2NConfig::constructor;

S2NConfig::S2NConfig() {
  s2n_config *s2nconfig = s2n_config_new();
  if(!s2nconfig) {
    Nan::ThrowError("Error getting new S2N config");
  }
}

S2NConfig::~S2NConfig() {
  s2n_config_free(s2nconfig);
  free(&version);
  free(&certificate);
  free(&privateKey);
  free(&dhparams);
}

void S2NConfig::Init(Handle<Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("S2NConfig").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "setStatusRequestType", SetStatusRequestType);
  Nan::SetPrototypeMethod(tpl, "setCipherPreferences", SetCipherPreferences);
  Nan::SetPrototypeMethod(tpl, "addCertChainAndKey", AddCertChainAndKey);
  Nan::SetPrototypeMethod(tpl, "addDhParams", AddDhParams);

  constructor.Reset(tpl->GetFunction());

  Nan::Set(exports, Nan::New("S2NConfig").ToLocalChecked(), tpl->GetFunction());

}

NAN_METHOD(S2NConfig::New) {
  Nan::HandleScope scope;

  if (info.IsConstructCall()) {
    S2NConfig* obj = new S2NConfig();
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { info[0] };
    Local<Function> cons = Nan::New<Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(S2NConfig::SetStatusRequestType) {
  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  // TODO: check type is valid
  s2n_status_request_type type = static_cast<s2n_status_request_type>(info[0]->IntegerValue());

  S2NConfig* self = ObjectWrap::Unwrap<S2NConfig>(info.Holder());

  int result = s2n_config_set_status_request_type(self->s2nconfig, type);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting config status request type: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConfig::SetCipherPreferences) {
  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsString()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  S2NConfig* self = ObjectWrap::Unwrap<S2NConfig>(info.Holder());

  std::string arg0 = *Nan::Utf8String(info[0]);

  self->version = arg0.c_str();

  int result = s2n_config_set_cipher_preferences(self->s2nconfig, self->version);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting config cipher preferences: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConfig::AddCertChainAndKey) {
  Nan::HandleScope scope;

  if(info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if(!info[0]->IsString() || !info[1]->IsString()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  S2NConfig* self = ObjectWrap::Unwrap<S2NConfig>(info.Holder());

  std::string arg0 = *Nan::Utf8String(info[0]);
  std::string arg1 = *Nan::Utf8String(info[1]);

  self->certificate = strdup(arg0.c_str());
  self->privateKey  = strdup(arg1.c_str());

  int result = s2n_config_add_cert_chain_and_key(self->s2nconfig, self->certificate, self->privateKey);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("adding certificate chain and key: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConfig::AddDhParams) {
  Nan::HandleScope scope;

  if(info.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if(!info[0]->IsString()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  S2NConfig* self = ObjectWrap::Unwrap<S2NConfig>(info.Holder());

  std::string arg0 = *Nan::Utf8String(info[0]);

  self->dhparams = strdup(arg0.c_str());

  int result = s2n_config_add_dhparams(self->s2nconfig, self->dhparams);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("adding DH params: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}
