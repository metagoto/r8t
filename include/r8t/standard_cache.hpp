#pragma once

#include <boost/unordered_map.hpp>


namespace r8t
{


template<typename Loader, typename Engine>
struct standard_cache
{

    typedef typename Loader::id_type         key_type;
    typedef typename Engine::compiled_type   mapped_type;

    typedef boost::unordered_map<key_type, mapped_type> container_type;
    typedef typename container_type::const_iterator     const_iterator;


    ~standard_cache();

    void clear();

    const_iterator get(key_type const& id) const;

    const_iterator set(key_type const& id, mapped_type const& item);

    const_iterator end() const;


private:

    container_type storage;

};


//////////////////////////////////


template<typename Loader, typename Engine>
standard_cache<Loader, Engine>::~standard_cache()
{
    clear();
}


template<typename Loader, typename Engine>
void standard_cache<Loader, Engine>::clear()
{
    for (typename container_type::iterator i(storage.begin()), e(storage.end())
        ; i != e; ++i)
    {
        (*Engine::release)(i->second);
    }
    storage.clear();
}


template<typename Loader, typename Engine>
typename standard_cache<Loader, Engine>::const_iterator standard_cache<Loader, Engine>::get(key_type const& id) const
{
    return storage.find(id);
}


template<typename Loader, typename Engine>
typename standard_cache<Loader, Engine>::const_iterator standard_cache<Loader, Engine>::set(key_type const& id, mapped_type const& item)
{
    return storage.insert(storage.end(), std::make_pair(id, item));
}


template<typename Loader, typename Engine>
typename standard_cache<Loader, Engine>::const_iterator standard_cache<Loader, Engine>::end() const
{
    return storage.end();
}



} // ns


