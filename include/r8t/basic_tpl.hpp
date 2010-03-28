#pragma once

#include <string>
#include <stdexcept>

namespace r8t
{


template<typename Parser
        ,typename Context
        ,template<typename C> class Engine
        ,typename Loader
        ,template<typename L, typename E> class Cache
>
struct basic_tpl : public Parser
                 , public Engine<Context>
                 , public Loader
                 , public Cache<Loader, Engine<Context> >
{

    typedef Parser                          parser_type;
    typedef Context                         context_type;
    typedef Engine<Context>                 engine_type;
    typedef Loader                          loader_type;
    typedef Cache<Loader, Engine<Context> > cache_type;

    typedef typename loader_type::item_type item_type;


    std::string render(item_type const& item, context_type const& ctx);

    std::string render_string(std::string const& input, context_type const& ctx);


};


///////////////////////////////////////


template<typename Parser
        ,typename Context
        ,template<typename C> class Engine
        ,typename Loader
        ,template<typename L, typename E> class Cache
>
inline std::string basic_tpl<Parser, Context, Engine, Loader, Cache>::render(item_type const& item, context_type const& ctx)
{
    typename loader_type::id_type id = loader_type::id(item);
    if (id == loader_type::invalid_id())
        throw std::runtime_error("id == loader_type::invalid_id()");

    typename cache_type::const_iterator it = cache_type::get(id);
    if (it == cache_type::end())
    {
        std::string content;
        if (loader_type::load(item, content, id) == loader_type::invalid_id())
            throw std::runtime_error("loader_type::load(id, content, computed_id)");

        std::string parsed;
        if (!parser_type::parse(content, parsed))
            throw std::runtime_error("parser_type::parse(content, parsed)");

        typename engine_type::compiled_type compiled = engine_type::compile(parsed, ctx);
        it = cache_type::set(id, compiled);
    }

    return engine_type::run(it->second, ctx);
}



template<typename Parser
        ,typename Context
        ,template<typename C> class Engine
        ,typename Loader
        ,template<typename L, typename E> class Cache
>
inline std::string basic_tpl<Parser, Context, Engine, Loader, Cache>::render_string(std::string const& input, context_type const& ctx)
{
    std::string parsed;
    if (!parser_type::parse(input, parsed))
        throw std::runtime_error("!parser_type::parse(input, parsed)");

    return engine_type::eval(parsed, ctx);
}


} // ns
