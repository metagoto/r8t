#pragma once

namespace r8t
{

template<typename T>
struct engine_base
{
protected:

    template<typename U>
    T create_context(U const& global_obj)
    {
        T ctx(global_obj);
        return ctx;
    }

    template<typename U>
    U const& internal_context(T const& ctx)
    {
        return ctx.internal_context();
    }

};

} // ns
