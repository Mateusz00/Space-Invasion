#include "XMLParseExcepetion.hpp"
#include "../Utility.hpp"

XMLParseExcepetion::XMLParseExcepetion(const pugi::xml_parse_result& result, const std::string& filename)
    : XMLParseExcepetion(result, filename.c_str())
{
}

XMLParseExcepetion::XMLParseExcepetion(const pugi::xml_parse_result& result, const char* filename)
    : mWhatString(result.description())
{
    mWhatString.append("  File: ");
    mWhatString.append(filename);
    mWhatString.append("  at position: ");
    mWhatString.append(toString(result.offset));
}

const char* XMLParseExcepetion::what() const noexcept
{
    return mWhatString.c_str();
}
