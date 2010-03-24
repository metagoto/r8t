#pragma once

#include <iostream>
#include <fstream>
#include <string>

#ifdef  BOOST_TEST_STATIC_LINK
#include <boost/test/unit_test.hpp>
#else
#include <boost/test/included/unit_test.hpp>
#endif

#include <boost/algorithm/string/replace.hpp>


namespace r8t { namespace test
{


inline bool load_file(char const* filename, std::string& content)
{
    std::ifstream in(filename, std::ios_base::in);
    if (!in)
    {
        return false;
    }

    in.unsetf(std::ios::skipws); // keep white space
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(content));

    return true;
}


inline bool from_file(char const* filename, std::string& input, std::string& expected, std::string const& exp_suffix = ".exp")
{
    input.clear();
    expected.clear();

    std::string filename_exp = std::string(filename) + exp_suffix;

    if (!load_file(filename, input))
    {
        BOOST_TEST_MESSAGE("error: could not open file: " << filename);
        return false;
    }

    if (!load_file(filename_exp.c_str(), expected))
    {
        BOOST_TEST_MESSAGE("error: could not open file: " << filename_exp);
        return false;
    }

    return true;
}


inline void fail_format(std::string const& input
                       ,std::string const& expected
                       ,std::string const& output
                       ,std::string const& suffix
                       ,char const* filename = 0)
{

    using boost::algorithm::replace_all_copy;

    if (filename)
    {
        std::string filename_out = std::string(filename) + suffix;
        BOOST_TEST_MESSAGE( "file: " << filename << " -> " << filename_out);

        std::ofstream out(filename_out.c_str(), std::ios_base::out);
        if (!out)
            BOOST_TEST_MESSAGE("error: could not write to file: " << filename_out);
        else
            out << output;
    } else {
        BOOST_TEST_MESSAGE(    "input: " << replace_all_copy(input, "\n", "\\n") << "\n"
                            << "  exp: " << replace_all_copy(expected, "\n", "\\n") << "\n"
                            << "  out: " << replace_all_copy(output, "\n", "\\n") << "\n"
                            << "  siz: " << expected.size() << " " << output.size());
    }
}

} } // ns
