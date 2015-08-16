#include "node-s2nconnection.h"
#include "node-s2nconfig.h"

#include <node.h>
#include <s2n.h>

using namespace v8;

Nan::Persistent<Function> S2NConnection::constructor;

S2NConnection::S2NConnection(s2n_mode mode) {
  s2n_connection *s2nconnection = s2n_connection_new(mode);
  if(!s2nconnection) {
    Nan::ThrowError("Error getting new S2N connection");
  }
}

S2NConnection::~S2NConnection() {
  s2n_connection_free(s2nconnection);
  free(&mode);
  free(&serverName);
}

void S2NConnection::Init(Handle<Object> exports) {
  Nan::HandleScope scope;

  // Prepare constructor template
  Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("S2NConnection").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  Nan::SetPrototypeMethod(tpl, "setConfig", SetConfig);
  Nan::SetPrototypeMethod(tpl, "setServerName", SetServerName);
  Nan::SetPrototypeMethod(tpl, "getServerName", GetServerName);
  Nan::SetPrototypeMethod(tpl, "setFD", SetFD);
  Nan::SetPrototypeMethod(tpl, "setReadFD", SetReadFD);
  Nan::SetPrototypeMethod(tpl, "setWriteFD", SetWriteFD);
  Nan::SetPrototypeMethod(tpl, "setBlinding", SetBlinding);
  Nan::SetPrototypeMethod(tpl, "getApplicationProtocol", GetApplicationProtocol);
  Nan::SetPrototypeMethod(tpl, "getOCSPResponse", GetOCSPResponse);
  Nan::SetPrototypeMethod(tpl, "getAlert", GetAlert);
  Nan::SetPrototypeMethod(tpl, "getCipher", GetCipher);
  Nan::SetPrototypeMethod(tpl, "wipe", Wipe);

  constructor.Reset(tpl->GetFunction());

  Nan::Set(exports, Nan::New("S2NConnection").ToLocalChecked(), tpl->GetFunction());

}

NAN_METHOD(S2NConnection::New) {
  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  s2n_mode mode = static_cast<s2n_mode>(info[0]->Int32Value());

  if (info.IsConstructCall()) {
    S2NConnection* obj = new S2NConnection(mode);
    obj->mode = mode;
    obj->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { info[0] };
    Local<Function> cons = Nan::New<Function>(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(S2NConnection::SetConfig) {

  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsObject()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  // TODO: check info[0] is an instance of S2NConfig

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  S2NConfig* config = ObjectWrap::Unwrap<S2NConfig>(info[0]->ToObject());

  int result = s2n_connection_set_config(self->s2nconnection, config->s2nconfig);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection configuration: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  self->config = config;

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConnection::SetServerName) {

  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsString()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  std::string arg0 = *Nan::Utf8String(info[0]);

  self->serverName = arg0.c_str();

  int result = s2n_set_server_name(self->s2nconnection, self->serverName);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection server name: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConnection::GetServerName) {

  Nan::HandleScope scope;

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  const char* name = s2n_get_server_name(self->s2nconnection);

  info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());

}

NAN_METHOD(S2NConnection::SetFD) {

  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  int fd = info[0]->IntegerValue();

  int result = s2n_connection_set_fd(self->s2nconnection, fd);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection fd: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConnection::SetReadFD) {

  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  int readFD = info[0]->IntegerValue();

  int result = s2n_connection_set_read_fd(self->s2nconnection, readFD);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection read fd: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConnection::SetWriteFD) {

  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  int writeFD = info[0]->IntegerValue();

  int result = s2n_connection_set_write_fd(self->s2nconnection, writeFD);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection write fd: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConnection::SetBlinding) {

  Nan::HandleScope scope;

  if(info.Length() < 1 || !info[0]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");
    return;
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  s2n_blinding blinding = static_cast<s2n_blinding>(info[0]->Int32Value());

  int result = s2n_connection_set_blinding(self->s2nconnection, blinding);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection blinding: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}

NAN_METHOD(S2NConnection::GetApplicationProtocol) {

  Nan::HandleScope scope;

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  const char* name = s2n_get_application_protocol(self->s2nconnection);

  info.GetReturnValue().Set(Nan::New(name).ToLocalChecked());

}

NAN_METHOD(S2NConnection::GetOCSPResponse) {

  Nan::HandleScope scope;

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  uint32_t length;

  const uint8_t* response = s2n_connection_get_ocsp_response(self->s2nconnection, &length);

  std::string tmp = std::string(reinterpret_cast<const char*>(response));

  v8::Local<v8::String> t = Nan::New<v8::String>(tmp).ToLocalChecked();

  info.GetReturnValue().Set(t);

}

NAN_METHOD(S2NConnection::GetAlert) {

  Nan::HandleScope scope;

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  int code = s2n_connection_get_alert(self->s2nconnection);

  info.GetReturnValue().Set(Nan::New(code));

}

NAN_METHOD(S2NConnection::GetCipher) {

  Nan::HandleScope scope;

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  const char* cipher = s2n_connection_get_cipher(self->s2nconnection);

  info.GetReturnValue().Set(Nan::New(cipher).ToLocalChecked());

}

NAN_METHOD(S2NConnection::Wipe) {

  Nan::HandleScope scope;

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(info.Holder());

  int result = s2n_connection_wipe(self->s2nconnection);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("wiping connection: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  info.GetReturnValue().Set(Nan::True());

}
