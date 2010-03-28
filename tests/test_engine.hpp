#pragma once

#include "test_base.hpp"
#include "../include/r8t/standard_engine.hpp"
#include "../include/r8t/standard_context.hpp"


namespace r8t { namespace tests { namespace engine
{

using namespace r8t;


template<typename Context, typename Engine>
inline void test(Context& ctx, Engine& e, std::string const& input, std::string const& expected, char const* filename = 0)
{
    std::string output = e.eval(input, ctx);

    BOOST_CHECK(output == expected);

    if (output != expected)
    {
        fail_format(input, expected, output, ".engine.out", filename);
    }
}


} } } // ns
