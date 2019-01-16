#ifndef XMLPARSEEXCEPETION_HPP
#define XMLPARSEEXCEPETION_HPP

#include <pugixml.hpp>
#include <exception>
#include <string>

class XMLParseException : public std::exception
{
    public:
                            XMLParseException(const pugi::xml_parse_result&, const std::string& filename);
                            XMLParseException(const pugi::xml_parse_result&, const char* filename);
        virtual const char* what() const noexcept override;

    private:
        std::string             mWhatString;
};

#endif // XMLPARSEEXCEPETION_HPP
