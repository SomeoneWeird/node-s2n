#include <nan.h>

#include <s2n.h>

#include "node-s2nconfig.h"
#include "node-s2nconnection.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  NanScope();

  int s2n_init_err = s2n_init();

  if(s2n_init_err == -1) {
    return NanThrowError("S2N failed to init");
  }

  S2NConfig::Init(exports);
  S2NConnection::Init(exports);
  exports->Set(NanNew("S2N_SSLv2"), NanNew(S2N_SSLv2));
  exports->Set(NanNew("S2N_SSLv3"), NanNew(S2N_SSLv3));
  exports->Set(NanNew("S2N_TLS10"), NanNew(S2N_TLS10));
  exports->Set(NanNew("S2N_TLS11"), NanNew(S2N_TLS11));
  exports->Set(NanNew("S2N_TLS12"), NanNew(S2N_TLS12));

  /* enums */
  Local<Object> s2n_modes = NanNew<Object>();
  s2n_modes->Set(NanNew("S2N_SERVER"), NanNew(S2N_SERVER));
  s2n_modes->Set(NanNew("S2N_CLIENT"), NanNew(S2N_CLIENT));

  Local<Object> s2n_blindings = NanNew<Object>();
  s2n_blindings->Set(NanNew("S2N_BUILT_IN_BLINDING"), NanNew(S2N_BUILT_IN_BLINDING));
  s2n_blindings->Set(NanNew("S2N_SELF_SERVICE_BLINDING"), NanNew(S2N_SELF_SERVICE_BLINDING));

  Local<Object> s2n_status_request_types = NanNew<Object>();
  s2n_status_request_types->Set(NanNew("S2N_STATUS_REQUEST_NONE"), NanNew(S2N_STATUS_REQUEST_NONE));
  s2n_status_request_types->Set(NanNew("S2N_STATUS_REQUEST_OCSP"), NanNew(S2N_STATUS_REQUEST_OCSP));

  exports->Set(NanNew("S2N_MODE"), s2n_modes);
  exports->Set(NanNew("S2N_BLINDING"), s2n_blindings);
  exports->Set(NanNew("S2N_STATUS_REQUEST_TYPE"), s2n_status_request_types);
}

NODE_MODULE(s2n, InitAll)
