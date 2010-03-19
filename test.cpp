#include <iostream>
#include <fstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "include/r8t/parser/standard_unicode.hpp"



bool test_file(char const* filename, std::string& input, std::string& expected)
{
    input.clear();
    expected.clear();

    std::string filename_exp = std::string(filename) + ".exp";

    std::ifstream in(filename, std::ios_base::in);
    if (!in) {
        std::cout << "error: could not open file: " << filename << std::endl;
        return false;
    }

    std::ifstream in_exp(filename_exp.c_str(), std::ios_base::in);
    if (!in_exp) {
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


bool test(std::string const& input, std::string const& expected, char const* filename = 0)
{
    using boost::algorithm::replace_all_copy;

    typedef boost::u8_to_u32_iterator<std::string::const_iterator> iterator_type;

    iterator_type first = input.begin();
    iterator_type last = input.end();

    r8t::parser::standard_unicode<iterator_type> p;

    std::string result;

    if (boost::spirit::qi::parse(first, last, p, result) && first == last && result == expected) {
        std::cout << " PASS "
                  << (filename ? filename : replace_all_copy(input, "\n", "\\n"));
    }
    else {
        std::cout << "-FAIL ";
        if (filename) {
            std::string filename_out = std::string(filename) + ".out";
            std::cout << filename << " -> " << filename_out;
            std::ofstream out(filename_out.c_str(), std::ios_base::out);
            if (!out) {
                std::cout << "error: could not write to file: " << filename_out << std::endl;
            }
            else {
                out << result << "\n";
            }
        }
        else {
            std::cout << replace_all_copy(input, "\n", "\\n") << "\n"
                      << "  exp " << replace_all_copy(expected, "\n", "\\n") << "\n"
                      << "  out " << replace_all_copy(result, "\n", "\\n") << "\n"
                      << "  siz " << result.size() << " " << expected.size();
        }
    }
    std::cout << std::endl;
}


int main()
{

    test(""
        ,""
    );
    test("a"
        ,"__pr('a');"
    );
    test("ab\nc"
        ,"__pr('ab\\nc');"
    );
    test("{% azf = 42 %}"
        ,"azf = 42;"
    );
    test("{# azf #}"
        ,""
    );
    test("abc{%for(i in a):%}{%end%}"
        ,"__pr('abc');for(i in a){};"
    );
    test("abc{%for(i in a):%}\nraw{#com#}war{{sa}}{%end%}"
        ,"__pr('abc');for(i in a){__pr('raw');__pr('war');__p(sa);};"
    );

    std::string input;
    std::string expected;

    const char* files[] = {
        "tests/parser/basic_1.txt"
       ,"tests/parser/basic_2.txt"
       ,"tests/parser/basic_3.txt"
    };

    BOOST_FOREACH( const char* f, files )
    {
        if (test_file(f, input, expected))
            test(input, expected, f);
    }


    return 0;
};
