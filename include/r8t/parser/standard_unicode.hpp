#pragma once

#include <string>

#define BOOST_SPIRIT_UNICODE
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_container.hpp>
#include <boost/spirit/include/phoenix_statement.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/regex/pending/unicode_iterator.hpp>


namespace r8t { namespace parser
{

    using boost::spirit::unicode::char_;
    using boost::spirit::unicode::space;
    using boost::spirit::qi::grammar;
    using boost::spirit::qi::rule;
    using boost::spirit::qi::_val;
    using boost::spirit::qi::_r1;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::lit;
    using boost::spirit::qi::eps;
    using boost::spirit::qi::eol;
    using boost::phoenix::function;

    typedef boost::spirit::char_encoding::unicode unicode;
    typedef boost::uint32_t uchar; // a unicode code point


    namespace detail
    {
        struct push_utf8
        {
            template <typename S, typename C>
            struct result { typedef void type; };

            void operator()(std::string& buf, uchar code_point) const
            {
                typedef std::back_insert_iterator<std::string> insert_iter;
                insert_iter out_iter(buf);
                boost::utf8_output_iterator<insert_iter> utf8_iter(out_iter);
                *utf8_iter++ = code_point;
            }
        };

        struct push_utf8_esc
        {
            template <typename S, typename C>
            struct result { typedef void type; };

            void operator()(std::string& buf, uchar code_point) const
            {
                switch (code_point)
                {
                    case '\'': buf += "\\'"; break;
                    case '\n': buf += "\\n"; break;
                    default: push_utf8()(buf, code_point);
                }
            }
        };

        struct push_plain
        {
            template <typename S, typename C>
            struct result { typedef void type; };

            void operator()(std::string& buf, std::string const& plain) const
            {
                buf.append("__pr('").append(plain).append("');");
            }
        };

        struct push_expr
        {
            template <typename S, typename C>
            struct result { typedef void type; };

            void operator()(std::string& buf, std::string const& expr) const
            {
                buf.append("__p(").append(expr).append(");");
            }
        };

        struct push_block
        {
            template <typename S, typename C>
            struct result { typedef void type; };

            void operator()(std::string& buf, std::string const& block) const
            {
                if (block.size()) {
                    buf += block;
                    std::string::iterator it = buf.end()-1;
                    if (*it == ':')
                        *it = '{';
                    else
                        buf += ";";
                }
            }
        };


    } // ns detail




template <typename Iterator>
struct standard_unicode : grammar<Iterator, unicode, std::string()>
{
    standard_unicode() : standard_unicode::base_type(start)
    {

        function<detail::push_utf8>     push_utf8;
        function<detail::push_utf8_esc> push_utf8_esc;
        function<detail::push_plain>    push_plain;
        function<detail::push_expr>     push_expr;
        function<detail::push_block>    push_block;


        open_block   = lit("{%") >> *space;
        open_expr    = lit("{{") >> *space;
        open_comment = lit("{#") >> *space;

        close_block  = *space >> lit("%}") >> -eol; // eat immediate end of line
        close_expr   = *space >> lit("}}");
        close_comment= *space >> lit("#}") >> -eol;


        start =
               *( directive                [_val += _1]
                | comment
                | plain                    [push_plain(_val, _1)]
                )
        ;


        open_marker =
                  open_block
                | open_expr
                | open_comment
        ;


        comment =
                  open_comment
               >> *(char_ - close_comment)
               >> close_comment
        ;


        plain =
                +(char_ - open_marker)     [push_utf8_esc(_val, _1)]
        ;


        directive =
                  block                    [push_block(_val, _1)]
                | expr                     [push_expr(_val, _1)]
        ;


        block =
                  open_block
               >> ( block_end(_val) | block_begin )
               >> close_block
        ;


        block_begin =
                +(char_ - close_block)     [push_utf8(_val, _1)]
        ;


        block_end =
                  lit("end")
               >> *(char_ - close_block)
               >> eps[_r1 += '}']
        ;


        expr =
                  open_expr
               >> +(char_ - close_expr)    [push_utf8(_val, _1)]
               >> close_expr
        ;


    }

    rule<Iterator, unicode, std::string()> start
                                         , directive
                                         , expr
                                         , plain;

    rule<Iterator, unicode> open_block
                          , open_expr
                          , open_comment
                          , open_marker;

    rule<Iterator, unicode> close_block
                          , close_expr
                          , close_comment
                          , comment;

    rule<Iterator, unicode, std::string()> block
                                         , block_begin;

    rule<Iterator, unicode, void(std::string&)> block_end;


};

} } // ns


