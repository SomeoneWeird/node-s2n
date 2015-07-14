#include <nan.h>

#include <s2n.h>

#include "node-s2nconfig.h"
#include "node-s2nconnection.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  NanScope();
  S2NConfig::Init(exports);
  exports->Set(NanNew("S2N_SSLv2"), NanNew(S2N_SSLv2));
  exports->Set(NanNew("S2N_SSLv3"), NanNew(S2N_SSLv3));
  exports->Set(NanNew("S2N_TLS10"), NanNew(S2N_TLS10));
  exports->Set(NanNew("S2N_TLS11"), NanNew(S2N_TLS11));
  exports->Set(NanNew("S2N_TLS12"), NanNew(S2N_TLS12));
}

NODE_MODULE(s2n, InitAll)
