#include <iostream>
#include <fstream>
#include <string>

#define BOOST_TEST_MODULE r8t tests suite
#ifdef  BOOST_TEST_STATIC_LINK
#include <boost/test/unit_test.hpp>
#else   // build boost test executor in place
#include <boost/test/included/unit_test.hpp>
#endif

#include <boost/foreach.hpp>

#include "tests/test_parser.hpp"
#include "tests/test_context.hpp"


BOOST_AUTO_TEST_SUITE(standard_parser)

BOOST_AUTO_TEST_CASE(string_parsing)
{
    using r8t::test::parser::test;

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
}

BOOST_AUTO_TEST_CASE(file_parsing)
{
    using r8t::test::parser::test;
    using r8t::test::parser::from_file;

    std::string input;
    std::string expected;

    const char* files[] = {
        "tests/parser/basic_1.txt"
       ,"tests/parser/basic_2.txt"
       ,"tests/parser/basic_3.txt"
       ,"tests/parser/basic_4.txt"
    };

    BOOST_FOREACH(const char* f, files)
    {
        if (from_file(f, input, expected))
            test(input, expected, f);
    }

}

BOOST_AUTO_TEST_SUITE_END() // standard_parser


///////////////////////////////////////////////////////


BOOST_AUTO_TEST_SUITE(standard_context)

BOOST_AUTO_TEST_CASE(new_context)
{
    using r8t::test::context::test_variable_basic;

    r8t::standard_engine e;
    r8t::standard_context ctx = e.new_context();

    test_variable_basic(ctx, e);

}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    using r8t::test::context::test_variable_basic;

    r8t::standard_engine e;
    r8t::standard_context ctx = e.new_context();

    test_variable_basic(ctx, e);

    r8t::standard_context ctx2 = ctx;

    test_variable_basic(ctx2, e, false);

    ctx.set("test", false);
    BOOST_CHECK_EQUAL(e.run("__p(test)",ctx), std::string("false"));
    BOOST_CHECK_EQUAL(e.run("__p(test)",ctx2), std::string("string"));

    ctx2.set("test", "nop");
    BOOST_CHECK_EQUAL(e.run("__p(test)",ctx), std::string("false"));
    BOOST_CHECK_EQUAL(e.run("__p(test)",ctx2), std::string("nop"));

}

BOOST_AUTO_TEST_CASE(assignment_operator)
{
    using r8t::test::context::test_variable_basic;

    r8t::standard_engine e;
    r8t::standard_context ctx = e.new_context();

    test_variable_basic(ctx, e);

    r8t::standard_context ctx2 = e.new_context();

    test_variable_basic(ctx2, e);

    r8t::standard_context ctx3 = e.new_context();
    ctx3 = ctx2;

    test_variable_basic(ctx3, e, false);

}

BOOST_AUTO_TEST_SUITE_END()


///////////////////////////////////////////////////////


