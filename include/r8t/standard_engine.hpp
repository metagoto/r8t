#pragma once

#include <string>
#include <stdexcept>
#include <v8.h>

#include "engine_base.hpp"


namespace r8t
{

using namespace v8;


template<typename Context_T>
struct standard_engine : public engine_base<Context_T>
{

    typedef Context_T                     context_type;
    typedef engine_base<context_type>     base_type;
    typedef standard_engine<context_type> self_type;

    typedef Persistent<Script> compiled_type;


    standard_engine();
    ~standard_engine();

    std::string eval(std::string const& src, context_type const& ctx);

    compiled_type compile(std::string const& src, context_type const& ctx);

    std::string run(compiled_type const& src, context_type const& ctx);

    context_type new_context();

    static void release(compiled_type& item);

private:

    static Handle<Value> __pr(Arguments const& args);
    static Handle<Value> __p(Arguments const& args);

    static self_type* unwrap(Arguments const& args);

    std::string buffer;
};


//////////////////////////////////////////////////////


template<typename Context_T>
inline standard_engine<Context_T>::standard_engine()
{

}


template<typename Context_T>
inline standard_engine<Context_T>::~standard_engine()
{

}


template<typename Context_T>
inline std::string standard_engine<Context_T>::eval(std::string const& src, context_type const& ctx)
{
    buffer.clear();
    HandleScope hs;
    Context::Scope scope(base_type::template internal_context<Persistent<Context> >(ctx));
    TryCatch trycatch;

    Handle<String> source = String::New(src.c_str(), src.size());

    Handle<Script> script = Script::Compile(source);
    if (script.IsEmpty() || trycatch.HasCaught())
    {
        String::Utf8Value exception(trycatch.Exception());
        throw std::runtime_error(*exception);
    }
    else
    {
        Handle<Value> result = script->Run();
        if (result.IsEmpty() || trycatch.HasCaught())
        {
            String::Utf8Value exception(trycatch.Exception());
            throw std::runtime_error(*exception);
        }
    }
    return buffer;
}


template<typename Context_T>
inline typename standard_engine<Context_T>::compiled_type standard_engine<Context_T>::compile(std::string const& src, context_type const& ctx)
{
    HandleScope hs;
    Context::Scope scope(base_type::template internal_context<Persistent<Context> >(ctx));
    TryCatch trycatch;

    Handle<String> source = String::New(src.c_str(), src.size());

    Handle<Script> script = Script::New(source);
    if (script.IsEmpty() || trycatch.HasCaught())
    {
        String::Utf8Value exception(trycatch.Exception());
        throw std::runtime_error(*exception);
    }
    return Persistent<Script>::New(script);
}


template<typename Context_T>
inline std::string standard_engine<Context_T>::run(compiled_type const& src, context_type const& ctx)
{
    buffer.clear();
    HandleScope hs;
    Context::Scope scope(base_type::template internal_context<Persistent<Context> >(ctx));
    TryCatch trycatch;

    Handle<Value> result = src->Run();
    if (result.IsEmpty() || trycatch.HasCaught())
    {
        String::Utf8Value exception(trycatch.Exception());
        throw std::runtime_error(*exception);
    }

    return buffer;
}


template<typename Context_T>
inline Context_T standard_engine<Context_T>::new_context() // yes. return by val. RVO
{
    HandleScope hs;

    Handle<ObjectTemplate> global_obj = ObjectTemplate::New();
    global_obj->Set(String::New("__pr"), FunctionTemplate::New(__pr, Handle<External>(External::New(this))));
    global_obj->Set(String::New("__p"),  FunctionTemplate::New(__p,  Handle<External>(External::New(this))));

    return base_type::template create_context<Handle<ObjectTemplate> >(global_obj);
}


template<typename Context_T>
inline Handle<Value> standard_engine<Context_T>::__pr(Arguments const& args)
{
    if (!args.Length())
        return Undefined();

    self_type* self = unwrap(args);

    String::Utf8Value utf8(args[0]);
    self->buffer += std::string(*utf8, utf8.length());
    return Undefined();
}


template<typename Context_T>
inline Handle<Value> standard_engine<Context_T>::__p(Arguments const& args)
{
    if (!args.Length())
        return Undefined();

    self_type* self = unwrap(args);

    String::Utf8Value utf8(args[0]);
    self->buffer += std::string(*utf8, utf8.length());
    return Undefined();
}


template<typename Context_T>
inline typename standard_engine<Context_T>::self_type* standard_engine<Context_T>::unwrap(Arguments const& args)
{
    Handle<External> field = Handle<External>::Cast(args.Data());

    assert(!field.IsEmpty());
    assert(field->Value() != (void*)0);

    return static_cast<self_type*>(field->Value());
}


template<typename Context_T>
void standard_engine<Context_T>::release(compiled_type& item)
{
    item.Clear();
    item.Dispose();
}


} // ns


