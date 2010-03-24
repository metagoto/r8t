#include <string>


namespace custom
{

struct person
{
    std::string name;
    int age;
};


} // custom ns


namespace r8t { namespace conv
{

template<>
inline Handle<Value> from<demo>(demo const& val)
{
    Local<Object> obj = Object::New();
    obj->Set(String::New("name"), from(val.name));
    obj->Set(String::New("age"), from(val.age + 100));
    return obj;
}


} } // ns
