#pragma once

#include "test_base.hpp"
#include "../include/r8t/tpl.hpp"


namespace r8t { namespace tests { namespace tpl
{

using namespace ::r8t;


template<typename T, typename Context>
inline void test(T& t, Context const& ctx, std::string const& input, std::string const& expected, char const* filename = 0)
{
    std::string const& output = t.render_string(input, ctx);

    BOOST_CHECK(output == expected);

    if (output != expected)
    {
        fail_format(input, expected, output, ".r8t.out", filename);
    }
}


} } } // ns
