#include <iostream>
#include <string>

#define BOOST_TEST_MODULE r8t tests suite
#ifdef  BOOST_TEST_STATIC_LINK
#include <boost/test/unit_test.hpp>
#else   // build boost test executor in place
#include <boost/test/included/unit_test.hpp>
#endif

#include <boost/foreach.hpp>

#include "tests/test_grammar.hpp"
#include "tests/test_parser.hpp"
#include "tests/test_context.hpp"
#include "tests/test_engine.hpp"
#include "tests/test_r8t.hpp"



BOOST_AUTO_TEST_SUITE(grammar_standard_unicode)

BOOST_AUTO_TEST_CASE(from_string)
{
    using r8t::test::grammar::test;

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

BOOST_AUTO_TEST_CASE(from_file)
{
    using r8t::test::from_file;
    using r8t::test::grammar::test;

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

BOOST_AUTO_TEST_SUITE_END() // grammar_standard_unicode


///////////////////////////////////////////////////////


BOOST_AUTO_TEST_SUITE(standard_parser)

BOOST_AUTO_TEST_CASE(from_string)
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

BOOST_AUTO_TEST_CASE(from_file)
{
    using r8t::test::from_file;
    using r8t::test::parser::test;

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

    typedef r8t::standard_engine<r8t::standard_context> engine_type;
    typedef engine_type::context_type context_type;

    engine_type e;
    context_type ctx = e.new_context();

    test_variable_basic(ctx, e);

}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
    using r8t::test::context::test_variable_basic;

    typedef r8t::standard_engine<r8t::standard_context> engine_type;
    typedef engine_type::context_type context_type;

    engine_type e;
    context_type ctx = e.new_context();

    test_variable_basic(ctx, e);

    context_type ctx2 = ctx;

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
    using r8t::test::context::test_variable_more;
    using r8t::test::context::fill_context;

    typedef r8t::standard_engine<r8t::standard_context> engine_type;
    typedef engine_type::context_type context_type;

    engine_type e;
    context_type ctx = e.new_context();
    test_variable_basic(ctx, e);

    context_type ctx2 = e.new_context();
    test_variable_basic(ctx2, e);

    {
        context_type ctx3 = e.new_context();
        ctx3 = ctx2;
        test_variable_basic(ctx3, e, false);
    }
    test_variable_basic(ctx2, e, false);

    fill_context(ctx2);
    test_variable_more(ctx2, e);

    {
        context_type ctx3 = e.new_context();
        ctx3 = ctx2;

        test_variable_more(ctx3, e);
    }
}

BOOST_AUTO_TEST_SUITE_END() // standard_context


///////////////////////////////////////////////////////


BOOST_AUTO_TEST_SUITE(standard_engine)

BOOST_AUTO_TEST_CASE(run)
{
    using r8t::test::from_file;
    using r8t::test::engine::test;
    using r8t::test::context::test_variable_more;
    using r8t::test::context::fill_context;

    typedef r8t::standard_engine<r8t::standard_context> engine_type;
    typedef engine_type::context_type context_type;


    std::string input;
    std::string expected;

    const char* files[] = {
        "tests/parser/basic_1.txt.exp"
       ,"tests/parser/basic_2.txt.exp"
       ,"tests/parser/basic_3.txt.exp"
       ,"tests/parser/basic_4.txt.exp"
    };

    BOOST_FOREACH(const char* f, files)
    {

        engine_type e;
        context_type ctx = e.new_context();

        fill_context(ctx);
        test_variable_more(ctx, e);

        if (from_file(f, input, expected))
            r8t::test::engine::test(ctx, e, input, expected, f);
    }
}


BOOST_AUTO_TEST_SUITE_END() // standard_engine


///////////////////////////////////////////////////////


BOOST_AUTO_TEST_SUITE(r8t_interface)

BOOST_AUTO_TEST_CASE(combined1)
{
    using r8t::test::from_file;
    using r8t::test::context::fill_context;
    using r8t::test::r8t::test;


    typedef r8t::r8t tpl_type;

    tpl_type tpl;
    tpl_type::context_type ctx = tpl.new_context();

    fill_context(ctx);

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
        if (from_file(f, input, expected, ".exp.exp"))
        {
            r8t::test::r8t::test(tpl, ctx, input, expected, f);
        }
    }


}

BOOST_AUTO_TEST_SUITE_END() // r8t_interface


///////////////////////////////////////////////////////


