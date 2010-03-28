#pragma once

#include "test_base.hpp"
#include "../include/r8t/grammar/standard_unicode.hpp"


namespace r8t { namespace tests { namespace grammar
{


inline void test(std::string const& input, std::string const& expected, char const* filename = 0)
{

    typedef boost::u8_to_u32_iterator<std::string::const_iterator> iterator_type;

    iterator_type first = input.begin();
    iterator_type last = input.end();

    std::string result;
    r8t::grammar::standard_unicode<iterator_type> p;

    bool b = boost::spirit::qi::parse(first, last, p, result);

    BOOST_CHECK(b);
    BOOST_CHECK(first == last);
    BOOST_CHECK(result == expected);

    if (result != expected)
    {
        fail_format(input, expected, result, ".out", filename);
    }
}


} } } // ns
