#include <nan.h>

#include <s2n.h>

#include "node-s2nconfig.h"
#include "node-s2nconnection.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  Nan::HandleScope scope;

  int s2n_init_err = s2n_init();

  if(s2n_init_err < 0) {
    std::string err(s2n_strerror(s2n_errno, "EN"));
    std::string error("S2N failed to init: ");
    error.append(err);
    Nan::ThrowError(error.c_str());
    return;
  }

  S2NConfig::Init(exports);
  S2NConnection::Init(exports);

  Nan::Set(exports, Nan::New("S2N_SSLv2").ToLocalChecked(), Nan::New(S2N_SSLv2));
  Nan::Set(exports, Nan::New("S2N_SSLv3").ToLocalChecked(), Nan::New(S2N_SSLv3));
  Nan::Set(exports, Nan::New("S2N_TLS10").ToLocalChecked(), Nan::New(S2N_TLS10));
  Nan::Set(exports, Nan::New("S2N_TLS11").ToLocalChecked(), Nan::New(S2N_TLS11));
  Nan::Set(exports, Nan::New("S2N_TLS12").ToLocalChecked(), Nan::New(S2N_TLS12));

  Local<Object> s2n_modes = Nan::New<Object>();
  Nan::Set(s2n_modes, Nan::New("S2N_SERVER").ToLocalChecked(), Nan::New(S2N_SERVER));
  Nan::Set(s2n_modes, Nan::New("S2N_CLIENT").ToLocalChecked(), Nan::New(S2N_CLIENT));

  Local<Object> s2n_blindings = Nan::New<Object>();
  Nan::Set(s2n_blindings, Nan::New("S2N_BUILT_IN_BLINDING").ToLocalChecked(), Nan::New(S2N_BUILT_IN_BLINDING));
  Nan::Set(s2n_blindings, Nan::New("S2N_SELF_SERVICE_BLINDING").ToLocalChecked(), Nan::New(S2N_SELF_SERVICE_BLINDING));

  Local<Object> s2n_status_request_types = Nan::New<Object>();
  Nan::Set(s2n_status_request_types, Nan::New("S2N_STATUS_REQUEST_NONE").ToLocalChecked(), Nan::New(S2N_STATUS_REQUEST_NONE));
  Nan::Set(s2n_status_request_types, Nan::New("S2N_STATUS_REQUEST_OCSP").ToLocalChecked(), Nan::New(S2N_STATUS_REQUEST_OCSP));

  Nan::Set(exports, Nan::New("S2N_MODE").ToLocalChecked(), s2n_modes);
  Nan::Set(exports, Nan::New("S2N_BLINDING").ToLocalChecked(), s2n_blindings);
  Nan::Set(exports, Nan::New("S2N_STATUS_REQUEST_TYPE").ToLocalChecked(), s2n_status_request_types);
}

NODE_MODULE(s2n, InitAll)
