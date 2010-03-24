#pragma once

#include "test_base.hpp"
#include "../include/r8t/grammar/standard_unicode.hpp"
#include "../include/r8t/standard_parser.hpp"


namespace r8t { namespace test { namespace parser
{


inline void test(std::string const& input, std::string const& expected, char const* filename = 0)
{
    using boost::algorithm::replace_all_copy;

    typedef boost::u8_to_u32_iterator<std::string::const_iterator> iterator_type;
    typedef r8t::standard_parser<iterator_type, r8t::grammar::standard_unicode> parser_type;


    std::string output;

    parser_type p;

    BOOST_CHECK(p.parse(input, output));
    BOOST_CHECK(output == expected);

    if (output != expected)
    {
        fail_format(input, expected, output, ".parser.out", filename);
    }
}


} } } // ns
