#ifndef S2NConnection_H
#define S2NConnection_H

#include <nan.h>

class S2NConnection : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit S2NConnection();
  ~S2NConnection();

  static NAN_METHOD(New);
  static v8::Persistent<v8::Function> constructor;
};

#endif