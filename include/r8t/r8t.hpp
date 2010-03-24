#pragma once

#include "basic_r8t.hpp"
#include "grammar/standard_unicode.hpp"
#include "standard_parser.hpp"
#include "standard_context.hpp"
#include "standard_engine.hpp"

namespace r8t
{

    namespace
    {
        typedef boost::u8_to_u32_iterator<std::string::const_iterator> iterator_type;

        typedef r8t::standard_parser<iterator_type, r8t::grammar::standard_unicode> parser_type;
    }


typedef r8t::basic_r8t<
                parser_type
               ,r8t::standard_context
               ,r8t::standard_engine
        >
        r8t;


} // ns
