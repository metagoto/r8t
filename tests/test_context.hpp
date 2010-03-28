#pragma once

#include "test_base.hpp"
#include "../include/r8t/standard_engine.hpp"
#include "../include/r8t/standard_context.hpp"


namespace r8t { namespace tests { namespace context
{

using namespace r8t;

template<typename Context, typename Engine>
inline void test_variable_basic(Context& ctx, Engine& e, bool set = true)
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

        std::list<int> int_l(int_v.size());
        std::copy(int_v.begin(), int_v.end(), int_l.begin());
        ctx.set("test6", int_l);

        std::map<std::string, int> int_m;
        int_m["a"] = 3;
        int_m["b"] = 4;
        int_m["c"] = 42;
        ctx.set("test7", int_m);
    }

    BOOST_CHECK_EQUAL(e.eval("__p(test)",ctx), std::string("string"));

    BOOST_CHECK_EQUAL(e.eval("__p(test1)",ctx), std::string("const char*"));

    BOOST_CHECK_EQUAL(e.eval("__p(test2)",ctx), std::string("true"));

    BOOST_CHECK_EQUAL(e.eval("__p(test3)",ctx), std::string("42"));

    BOOST_CHECK_EQUAL(e.eval("__p(test4)",ctx), std::string("3.14"));

    BOOST_CHECK_EQUAL(e.eval("__p(typeof test5)",ctx), std::string("object"));
    BOOST_CHECK_EQUAL(e.eval("__p(test5.length)",ctx), std::string("3"));
    BOOST_CHECK_EQUAL(e.eval("__p(test5)",ctx),        std::string("1,2,42"));

    BOOST_CHECK_EQUAL(e.eval("__p(typeof test6)",ctx), std::string("object"));
    BOOST_CHECK_EQUAL(e.eval("__p(test6.length)",ctx), std::string("3"));
    BOOST_CHECK_EQUAL(e.eval("__p(test6)",ctx),        std::string("1,2,42"));

    BOOST_CHECK_EQUAL(e.eval("__p(typeof test7)",ctx), std::string("object"));
    BOOST_CHECK_EQUAL(e.eval("__p(test7)",ctx),        std::string("[object Object]"));
    BOOST_CHECK_EQUAL(e.eval("__p(test7.a + test7.b + test7.c)",ctx), std::string("49"));

    BOOST_CHECK_THROW(e.eval("__p(none)",ctx), std::runtime_error);
}


template<typename Context>
inline void fill_context(Context& ctx)
{
    typedef std::map<std::string, std::string> post_t;
    typedef std::vector<post_t>                posts_t;

    typedef std::vector<int>              ints_t;
    typedef std::map<std::string, ints_t> mapi_t;


    post_t post1;
    post1["title"] = "da freaking title";
    post1["date"]  = "date";

    post_t post2;
    post2["title"] = "da freaking title2";
    post2["date"]  = "date2";

    post_t post3;
    post3["title"] = "a title3";
    post3["date"]  = "date3";

    posts_t posts;
    posts.push_back(post1);
    posts.push_back(post2);
    posts.push_back(post3);

    ctx.set("posts", posts);


    ints_t ints1;
    ints1.push_back(1);
    ints1.push_back(2);
    ints1.push_back(3);

    ints_t ints2;
    ints2.push_back(4);
    ints2.push_back(5);
    ints2.push_back(6);

    mapi_t ints_m;
    ints_m["ints1"] = ints1;
    ints_m["ints2"] = ints2;

    ctx.set("map", ints_m);
}


template<typename Context, typename Engine>
inline void test_variable_more(Context& ctx, Engine& e)
{

    BOOST_CHECK_EQUAL(e.eval("__p(typeof posts)",ctx), std::string("object"));
    BOOST_CHECK_EQUAL(e.eval("__p(posts.length)",ctx), std::string("3"));
    BOOST_CHECK_EQUAL(e.eval("__p(posts[2].date)",ctx), std::string("date3"));

    BOOST_CHECK_EQUAL(e.eval("__p(typeof map)",ctx), std::string("object"));
    BOOST_CHECK_EQUAL(e.eval("__p(map.ints2.length)",ctx), std::string("3"));
    BOOST_CHECK_EQUAL(e.eval("__p(map.ints2[2])",ctx), std::string("6"));

}


} } } // ns
