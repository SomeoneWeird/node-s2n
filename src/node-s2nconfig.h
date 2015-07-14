#ifndef S2NConfig_H
#define S2NConfig_H

#include <nan.h>

class S2NConfig : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit S2NConfig();
  ~S2NConfig();

  static NAN_METHOD(New);
  static v8::Persistent<v8::Function> constructor;
};

#endif