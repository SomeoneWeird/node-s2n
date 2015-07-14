#ifndef MODULENAME_HPP
#define MODULENAME_HPP

#include <node.h>

class S2NConfig : public node::ObjectWrap {
public:
    static v8::Persistent<v8::FunctionTemplate> constructor;
    static void Init(v8::Handle<v8::Object> target);

protected:
    S2NConfig(int val);

    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Handle<v8::Value> Value(const v8::Arguments& args);

    // Your own object variables here
    int value_;
};
