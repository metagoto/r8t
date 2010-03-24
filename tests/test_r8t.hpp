#pragma once

#include "test_base.hpp"
#include "../include/r8t/r8t.hpp"


namespace r8t { namespace test { namespace r8t
{

using namespace ::r8t;


template<typename T, typename Context>
inline void test(T& tpl, Context const& ctx, std::string const& input, std::string const& expected, char const* filename = 0)
{
    std::string output = tpl.render(input, ctx);

    BOOST_CHECK(output == expected);

    if (output != expected)
    {
        fail_format(input, expected, output, ".r8t.out", filename);
    }
}


} } } // ns
