#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>

#include <v8.h>

namespace r8t { namespace conv
{

using namespace v8;



template<typename F>
inline Handle<Value> from(F const& val)
{
    return Null();
}


template<>
inline Handle<Value> from<std::string>(const std::string& val)
{
    return String::New(val.c_str(), val.size());
}


template<>
inline Handle<Value> from<const char*>(const char* const& val)
{
    return String::New(val, std::strlen(val));
}


template<>
inline Handle<Value> from<int>(const int& val)
{
    return Int32::New(val);
}


template<>
inline Handle<Value> from<double>(const double& val)
{
    return Number::New(val);
}


template<>
inline Handle<Value> from<std::time_t>(const std::time_t& val)
{
    return Integer::New(static_cast<uint32_t>(val));
}


template<>
inline Handle<Value> from<bool>(const bool& val)
{
    return Boolean::New(val);
}


///////////////////////////////


template<typename T>
inline Handle<Value> from/*<std::vector<T> >*/(std::vector<T> const& val)
{
    Local<Array> arr = Array::New(val.size());
    typename std::vector<T>::const_iterator i(val.begin()), e(val.end());
    for (int k = 0; i != e; ++i) {
        arr->Set(Integer::New(k++), from(*i));
    }
    return arr;
}

template<typename T>
inline Handle<Value> from/*<std::map<std::string, T> >*/(std::map<std::string, T> const& val)
{
    Local<Object> obj = Object::New();
    for (typename std::map<std::string, T>::const_iterator i(val.begin()), e(val.end()); i != e; ++i) {
        obj->ForceSet(String::New(i->first.c_str()), from(i->second));
    }
    return obj;
}



} } // ns
