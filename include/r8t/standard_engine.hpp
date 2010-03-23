#pragma once

#include <string>
#include <exception>
#include <v8.h>

#include "standard_context.hpp"


namespace r8t
{

using namespace v8;


struct standard_engine
{

    typedef standard_engine self_type;

    standard_engine();
    ~standard_engine();

    std::string run(std::string const& src, standard_context const& context);


    standard_context new_context();


private:

    static Handle<Value> __pr(Arguments const& args);
    static Handle<Value> __p(Arguments const& args);

    static self_type* unwrap(Arguments const& args);


    std::string buffer;

};


//////////////////////////////////////////////////////


inline standard_engine::standard_engine()
{

}


inline standard_engine::~standard_engine()
{

}


inline std::string standard_engine::run(std::string const& src, standard_context const& contx)
{
    HandleScope hs;
    Context::Scope scope(contx.context);

    buffer.clear();

    {
        TryCatch trycatch;
        Handle<String> source = String::New(src.c_str(), src.size());

        Handle<Script> script = Script::Compile(source);
        if (trycatch.HasCaught()) {
            Handle<Value> exception = trycatch.Exception();
            throw std::runtime_error(*String::AsciiValue(exception));
        }

        Handle<Value> result = script->Run();
        if (trycatch.HasCaught()) {
            Handle<Value> exception = trycatch.Exception();
            throw std::runtime_error(*String::AsciiValue(exception));
        }
    }

    return buffer;
}


inline standard_context standard_engine::new_context() // yes. return by val. RVO
{
    HandleScope hs;

    Handle<ObjectTemplate> global_obj = ObjectTemplate::New();
    global_obj->Set(String::New("__pr"), FunctionTemplate::New(__pr, Handle<External>(External::New(this))));
    global_obj->Set(String::New("__p"),  FunctionTemplate::New(__p,  Handle<External>(External::New(this))));

    standard_context ctx(global_obj);

    return ctx;
}



inline Handle<Value> standard_engine::__pr(Arguments const& args)
{
    if (!args.Length())
        return Undefined();

    self_type* self = unwrap(args);

    String::Utf8Value utf8(args[0]);
    self->buffer += std::string(*utf8, utf8.length());
    return Undefined();
}


inline Handle<Value> standard_engine::__p(Arguments const& args)
{
    if (!args.Length())
        return Undefined();

    self_type* self = unwrap(args);

    String::Utf8Value utf8(args[0]);
    self->buffer += std::string(*utf8, utf8.length());
    return Undefined();
}


inline standard_engine::self_type* standard_engine::unwrap(Arguments const& args)
{
    Handle<External> field = Handle<External>::Cast(args.Data());

    assert(!field.IsEmpty());
    assert(field->Value() != (void*)0);

    return static_cast<self_type*>(field->Value());
}


} // ns


