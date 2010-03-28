#pragma once

#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>

namespace r8t
{


struct standard_loader
{

    typedef std::string item_type;
    typedef std::size_t id_type;


    void base_path(std::string const& path);

    id_type load(std::string const& filename, std::string& content, id_type const& i = 0) const;

    id_type id(std::string const& filename, bool realpath = false) const;

    static id_type invalid_id() { return 0; }


private:

    bool load_file(const char* filename, std::string& content) const;

    boost::filesystem::path real_path(std::string const& filename) const;

    boost::filesystem::path root_path;

};


//////////////////////////////////


inline void standard_loader::base_path(std::string const& path)
{
    root_path = path;
}


inline standard_loader::id_type standard_loader::load(std::string const& filename
                                                     ,std::string& content, id_type const& i) const
{
    boost::filesystem::path const& file = real_path(filename);

    if (!boost::filesystem::exists(file) || !boost::filesystem::is_regular_file(file))
        return invalid_id();

    std::string const& file_str = file.string();

    if (!load_file(file_str.c_str(), content))
        return invalid_id();

    return (i ? i : id(file_str, true));
}


inline bool standard_loader::load_file(char const* filename, std::string& content) const
{
    std::ifstream in(filename, std::ios_base::in);
    if (!in)
        return false;

    in.unsetf(std::ios::skipws);
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(content));

    return true;
}


inline boost::filesystem::path standard_loader::real_path(std::string const& filename) const
{
    return boost::filesystem::complete(filename, root_path).normalize();
}


inline standard_loader::id_type standard_loader::id(std::string const& filename, bool realpath) const
{
    if (realpath)
        return boost::hash<std::string>()(filename);

    boost::filesystem::path const& file = real_path(filename);
    return boost::hash<std::string>()(file.string());
}


} // ns


