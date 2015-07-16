#ifndef S2NConfig_H
#define S2NConfig_H

#include <nan.h>

#include <s2n.h>

class S2NConfig : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  explicit S2NConfig();
  ~S2NConfig();

  static NAN_METHOD(New);
  static NAN_METHOD(SetStatusRequestType);
  static NAN_METHOD(SetCipherPreferences);
  static NAN_METHOD(AddCertChainAndKey);
  static NAN_METHOD(AddDhParams);
  static v8::Persistent<v8::Function> constructor;

  struct s2n_config *s2nconfig;

  const char *version;

  char *certificate;
  char *privateKey;
  char *dhparams;

};

#endif
