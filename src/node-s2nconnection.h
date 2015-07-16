#ifndef S2NConnection_H
#define S2NConnection_H

#include <nan.h>
#include <s2n.h>

#include "node-s2nconfig.h"

class S2NConnection : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit S2NConnection(s2n_mode mode);
  ~S2NConnection();

  static NAN_METHOD(New);
  static NAN_METHOD(SetConfig);
  static NAN_METHOD(SetServerName);

  static v8::Persistent<v8::Function> constructor;

  struct s2n_connection *s2nconnection;

  S2NConfig *config;

  s2n_mode mode;
  const char *serverName;

};

#endif