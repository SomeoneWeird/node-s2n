#ifndef S2N_H
#define S2N_H

#include <node.h>

class S2N : public node::ObjectWrap {
public:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static void Init(v8::Handle<v8::Object> target);

protected:
    static NAN_METHOD(New);

};
