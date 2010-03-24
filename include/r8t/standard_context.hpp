#pragma once

#include <string>
#include <v8.h>

#include "conv/from.hpp"
#include "engine_base.hpp"

namespace r8t
{

using namespace v8;

struct standard_context
{

    ~standard_context();

    standard_context(standard_context const& other);

    standard_context& operator=(standard_context const& other);

    standard_context& set(std::string const& name, const char* val);

    template<typename T>
    standard_context& set(std::string const& name, T const& val);


private:

    standard_context();

    standard_context(Handle<ObjectTemplate> const& obj_template);

    void clone_from(Persistent<Context> const& other);

    void dispose();

    template<typename T>
    void dispose(Persistent<T>& persistent);

    Persistent<Context> const& internal_context() const;

    template<class T>
    friend class engine_base;

    Persistent<ObjectTemplate> global_template;
    Persistent<Context>        intern_context;

};


///////////////////////


inline standard_context::~standard_context()
{
    dispose();
}


inline standard_context::standard_context(Handle<ObjectTemplate> const& obj_template)
    : global_template(Persistent<ObjectTemplate>::New(obj_template))
    , intern_context(Persistent<Context>::New(Context::New(0, global_template)))
{

}


inline standard_context::standard_context(standard_context const& other)
    : global_template(Persistent<ObjectTemplate>::New(other.global_template))
    , intern_context(Persistent<Context>::New(Context::New()))
{

    HandleScope hs;

    dispose(intern_context);

    intern_context = Persistent<Context>::New(Context::New(0, global_template));

    clone_from(other.intern_context);
}


inline standard_context& standard_context::operator=(standard_context const& other)
{
    if (this != &other)
    {
        HandleScope hs;

        dispose();

        global_template = Persistent<ObjectTemplate>::New(other.global_template);
        intern_context  = Persistent<Context>::New(Context::New(0, global_template));

        clone_from(other.intern_context);
    }
    return *this;
}


inline standard_context& standard_context::set(std::string const& name, const char* val)
{
    HandleScope hs;
    Context::Scope scope(intern_context);
    intern_context->Global()->Set(String::New(name.c_str(), name.size()), conv::from<const char*>(val));
    return *this;
}


template<typename T>
inline standard_context& standard_context::set(std::string const& name, T const& val)
{
    HandleScope hs;
    Context::Scope scope(intern_context);
    intern_context->Global()->Set(String::New(name.c_str(), name.size()), conv::from(val));
    return *this;
}


inline void standard_context::clone_from(Persistent<Context> const& other)
{
    Context::Scope scope(other);

    Handle<Object> from   = other->Global();
    Handle<Array>  keys   = from->GetPropertyNames();
    Handle<Object> holder = Object::New();

    uint32_t n = keys->Length();
    for (uint32_t i = 0; i != n; ++i)
    {
        Handle<Value> key = keys->Get(i);
        holder->Set(key, from->Get(key));
    }

    Handle<Object> clone = holder->Clone();
    for (uint32_t i = 0; i != n; ++i)
    {
        Handle<Value> key = keys->Get(i);
        intern_context->Enter();
        intern_context->Global()->Set(key, clone->Get(key));
        intern_context->Exit();
    }
}


inline void standard_context::dispose()
{
    dispose(intern_context);
    dispose(global_template);
}


template<typename T>
inline void standard_context::dispose(Persistent<T>& persistent)
{
    persistent.Clear();
    persistent.Dispose();
}


inline Persistent<Context> const& standard_context::internal_context() const
{
    return intern_context;
}


} // ns

