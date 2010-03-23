#pragma once

#ifdef  BOOST_TEST_STATIC_LINK
#include <boost/test/unit_test.hpp>
#else
#include <boost/test/included/unit_test.hpp>
#endif

#include "../include/r8t/standard_context.hpp"
#include "../include/r8t/standard_engine.hpp"


namespace r8t { namespace test { namespace context
{

using namespace r8t;


inline void test_variable_basic(standard_context& ctx, standard_engine& e, bool set = true)
{
    if (set)
    {
        ctx.set("test",  std::string("string"));
        ctx.set("test1", "const char*");
        ctx.set("test2", true)
           .set("test3", 42)
           .set("test4", 3.14);

        std::vector<int> int_v;
        int_v.push_back(1);
        int_v.push_back(2);
        int_v.push_back(42);
        ctx.set("test5", int_v);

        std::map<std::string, int> int_m;
        int_m["a"] = 3;
        int_m["b"] = 4;
        int_m["c"] = 42;
        ctx.set("test6", int_m);
    }

    BOOST_CHECK_EQUAL(e.run("__p(test)",ctx), std::string("string"));

    BOOST_CHECK_EQUAL(e.run("__p(test1)",ctx), std::string("const char*"));

    BOOST_CHECK_EQUAL(e.run("__p(test2)",ctx), std::string("true"));

    BOOST_CHECK_EQUAL(e.run("__p(test3)",ctx), std::string("42"));

    BOOST_CHECK_EQUAL(e.run("__p(test4)",ctx), std::string("3.14"));

    BOOST_CHECK_EQUAL(e.run("__p(typeof test5)",ctx), std::string("object"));
    BOOST_CHECK_EQUAL(e.run("__p(test5.length)",ctx), std::string("3"));
    BOOST_CHECK_EQUAL(e.run("__p(test5)",ctx),        std::string("1,2,42"));

    BOOST_CHECK_EQUAL(e.run("__p(typeof test6)",ctx), std::string("object"));
    BOOST_CHECK_EQUAL(e.run("__p(test6)",ctx),        std::string("[object Object]"));
    BOOST_CHECK_EQUAL(e.run("__p(test6.a + test6.b + test6.c)",ctx), std::string("49"));

    //BOOST_CHECK_THROW(e.run("__p(none)",ctx), std::runtime_error);
}




} } } // ns
