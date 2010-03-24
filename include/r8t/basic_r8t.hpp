#pragma once

#include <string>

namespace r8t
{


template<typename Parser
        ,typename Context
        ,template<typename T> class Engine
>
struct basic_r8t : public Parser
                 , public Engine<Context>
{

    typedef Parser          parser_type;
    typedef Context         context_type;
    typedef Engine<Context> engine_type;

    // tmp
    std::string render(std::string const& input, context_type const& ctx);


};


template<typename Parser
        ,typename Context
        ,template<typename T> class Engine
>
std::string basic_r8t<Parser, Context, Engine>::render(std::string const& input, context_type const& ctx)
{
    std::string parsed;
    if (parser_type::parse(input, parsed))
    {
        return engine_type::run(parsed, ctx);
    }
    return parsed;
}


} // ns
