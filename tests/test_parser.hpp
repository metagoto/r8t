#pragma once

#ifdef  BOOST_TEST_STATIC_LINK
#include <boost/test/unit_test.hpp>
#else
#include <boost/test/included/unit_test.hpp>
#endif

#include <boost/algorithm/string/replace.hpp>

#include "../include/r8t/parser/standard_unicode.hpp"


namespace r8t { namespace test { namespace parser
{


inline bool from_file(char const* filename, std::string& input, std::string& expected)
{
    input.clear();
    expected.clear();

    std::string filename_exp = std::string(filename) + ".exp";

    std::ifstream in(filename, std::ios_base::in);
    if (!in)
    {
        std::cout << "error: could not open file: " << filename << std::endl;
        return false;
    }

    std::ifstream in_exp(filename_exp.c_str(), std::ios_base::in);
    if (!in_exp)
    {
        std::cout << "error: could not open file: " << filename_exp << std::endl;
        return false;
    }

    in.unsetf(std::ios::skipws); // keep white space
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(input));

    in_exp.unsetf(std::ios::skipws);
    std::copy(
        std::istream_iterator<char>(in_exp),
        std::istream_iterator<char>(),
        std::back_inserter(expected));

    return true;
}


inline void test(std::string const& input, std::string const& expected, char const* filename = 0)
{
    using boost::algorithm::replace_all_copy;

    typedef boost::u8_to_u32_iterator<std::string::const_iterator> iterator_type;

    iterator_type first = input.begin();
    iterator_type last = input.end();

    std::string result;
    r8t::parser::standard_unicode<iterator_type> p;

    bool b = boost::spirit::qi::parse(first, last, p, result);

    BOOST_CHECK(b);
    BOOST_CHECK(first == last);
    BOOST_CHECK(result == expected);

    if (result != expected)
    {
        if (filename)
        {
            std::string filename_out = std::string(filename) + ".out";
            BOOST_TEST_MESSAGE( "file: " << filename << " -> " << filename_out);

            std::ofstream out(filename_out.c_str(), std::ios_base::out);
            if (!out)
                BOOST_TEST_MESSAGE("error: could not write to file: " << filename_out);
            else
                out << result;
        } else {
            BOOST_TEST_MESSAGE(    "input: " << replace_all_copy(input, "\n", "\\n") << "\n"
                                << "  exp: " << replace_all_copy(expected, "\n", "\\n") << "\n"
                                << "  out: " << replace_all_copy(result, "\n", "\\n") << "\n"
                                << "  siz: " << expected.size() << " " << result.size());
        }
    }
}


} } } // ns
