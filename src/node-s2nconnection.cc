#include "node-s2nconnection.h"
#include "node-s2nconfig.h"

#include <node.h>
#include <s2n.h>

using namespace v8;

Persistent<Function> S2NConnection::constructor;

S2NConnection::S2NConnection(s2n_mode mode) {
  s2n_connection *s2nconnection = s2n_connection_new(mode);
  if(!s2nconnection) {
    NanThrowError("Error getting new S2N connection");
  }
}

S2NConnection::~S2NConnection() {
  s2n_connection_free(s2nconnection);
  free(&mode);
  free(&serverName);
}

void S2NConnection::Init(Handle<Object> exports) {
  NanScope();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = NanNew<FunctionTemplate>(New);
  tpl->SetClassName(NanNew("S2NConnection"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(tpl, "setConfig", SetConfig);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setServerName", SetServerName);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getServerName", GetServerName);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setFD", SetFD);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setReadFD", SetReadFD);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setWriteFD", SetWriteFD);
  NODE_SET_PROTOTYPE_METHOD(tpl, "setBlinding", SetBlinding);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getApplicationProtocol", GetApplicationProtocol);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getOCSPResponse", GetOCSPResponse);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getAlert", GetAlert);
  NODE_SET_PROTOTYPE_METHOD(tpl, "getCipher", GetCipher);
  NODE_SET_PROTOTYPE_METHOD(tpl, "wipe", Wipe);

  NanAssignPersistent(constructor, tpl->GetFunction());
  exports->Set(NanNew("S2NConnection"), tpl->GetFunction());

}

NAN_METHOD(S2NConnection::New) {
  NanScope();

  if(args.Length() < 1 || !args[0]->IsNumber()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  s2n_mode mode = static_cast<s2n_mode>(args[0]->Int32Value());

  if (args.IsConstructCall()) {
    S2NConnection* obj = new S2NConnection(mode);
    obj->mode = mode;
    obj->Wrap(args.This());
    NanReturnValue(args.This());
  } else {
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Function> cons = NanNew<Function>(constructor);
    NanReturnValue(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(S2NConnection::SetConfig) {

  NanScope();

  if(args.Length() < 1 || !args[0]->IsObject()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  // TODO: check args[0] is an instance of S2NConfig

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  S2NConfig* config = ObjectWrap::Unwrap<S2NConfig>(args[0]->ToObject());

  int result = s2n_connection_set_config(self->s2nconnection, config->s2nconfig);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection configuration: ");
    error.append(err);
    NanThrowError(error.c_str());
    NanReturnUndefined();
  }

  self->config = config;

  NanReturnValue(NanNew(true));

}

NAN_METHOD(S2NConnection::SetServerName) {

  NanScope();

  if(args.Length() < 1 || !args[0]->IsString()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  std::string arg0 = *NanUtf8String(args[0]);

  self->serverName = arg0.c_str();

  int result = s2n_set_server_name(self->s2nconnection, self->serverName);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection server name: ");
    error.append(err);
    NanThrowError(error.c_str());
    NanReturnUndefined();
  }

  NanReturnValue(NanNew(true));

}

NAN_METHOD(S2NConnection::GetServerName) {

  NanScope();

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  const char* name = s2n_get_server_name(self->s2nconnection);

  NanReturnValue(NanNew(name));

}

NAN_METHOD(S2NConnection::SetFD) {

  NanScope();

  if(args.Length() < 1 || !args[0]->IsNumber()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  int fd = args[0]->IntegerValue();

  int result = s2n_connection_set_fd(self->s2nconnection, fd);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection fd: ");
    error.append(err);
    NanThrowError(error.c_str());
    NanReturnUndefined();
  }

  NanReturnValue(NanNew(true));

}

NAN_METHOD(S2NConnection::SetReadFD) {

  NanScope();

  if(args.Length() < 1 || !args[0]->IsNumber()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  int readFD = args[0]->IntegerValue();

  int result = s2n_connection_set_read_fd(self->s2nconnection, readFD);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection read fd: ");
    error.append(err);
    NanThrowError(error.c_str());
    NanReturnUndefined();
  }

  NanReturnValue(NanNew(true));

}

NAN_METHOD(S2NConnection::SetWriteFD) {

  NanScope();

  if(args.Length() < 1 || !args[0]->IsNumber()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  int writeFD = args[0]->IntegerValue();

  int result = s2n_connection_set_write_fd(self->s2nconnection, writeFD);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection write fd: ");
    error.append(err);
    NanThrowError(error.c_str());
    NanReturnUndefined();
  }

  NanReturnValue(NanNew(true));

}

NAN_METHOD(S2NConnection::SetBlinding) {

  NanScope();

  if(args.Length() < 1 || !args[0]->IsNumber()) {
    NanThrowTypeError("Wrong arguments");
    NanReturnUndefined();
  }

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  s2n_blinding blinding = static_cast<s2n_blinding>(args[0]->Int32Value());

  int result = s2n_connection_set_blinding(self->s2nconnection, blinding);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("setting connection blinding: ");
    error.append(err);
    NanThrowError(error.c_str());
    NanReturnUndefined();
  }

  NanReturnValue(NanNew(true));

}

NAN_METHOD(S2NConnection::GetApplicationProtocol) {

  NanScope();

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  const char* name = s2n_get_application_protocol(self->s2nconnection);

  NanReturnValue(NanNew(name));

}

NAN_METHOD(S2NConnection::GetOCSPResponse) {

  NanScope();

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  uint32_t length;

  const uint8_t* response = s2n_connection_get_ocsp_response(self->s2nconnection, &length);

  NanReturnValue(NanNew(response));

}

NAN_METHOD(S2NConnection::GetAlert) {

  NanScope();

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  int code = s2n_connection_get_alert(self->s2nconnection);

  NanReturnValue(NanNew(code));

}

NAN_METHOD(S2NConnection::GetCipher) {

  NanScope();

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  const char* cipher = s2n_connection_get_cipher(self->s2nconnection);

  NanReturnValue(NanNew(cipher));

}

NAN_METHOD(S2NConnection::Wipe) {

  NanScope();

  S2NConnection* self = ObjectWrap::Unwrap<S2NConnection>(args.Holder());

  int result = s2n_connection_wipe(self->s2nconnection);

  if(result < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("wiping connection: ");
    error.append(err);
    NanThrowError(error.c_str());
    NanReturnUndefined();
  }

  NanReturnValue(NanNew(true));

}
