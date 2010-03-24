#pragma once



namespace r8t
{


template<typename Iterator
        ,template<typename T> class Grammar
        >
struct standard_parser
{

    typedef Iterator iterator_type;
    typedef Grammar<Iterator> grammar_type;


    template<typename String>
    bool parse(String const& input, String& output)
    {
        iterator_type first = input.begin();
        iterator_type last  = input.end();

        grammar_type p;

        return boost::spirit::qi::parse(first, last, p, output) && first == last;
    }



};





} // ns
