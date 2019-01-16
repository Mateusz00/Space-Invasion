#include "XMLParseException.hpp"
#include "../Utility.hpp"

XMLParseException::XMLParseException(const pugi::xml_parse_result& result, const std::string& filename)
    : XMLParseException(result, filename.c_str())
{
}

XMLParseException::XMLParseException(const pugi::xml_parse_result& result, const char* filename)
    : mWhatString(result.description())
{
    mWhatString.append("  File: ");
    mWhatString.append(filename);
    mWhatString.append("  at position: ");
    mWhatString.append(toString(result.offset));
}

const char* XMLParseException::what() const noexcept
{
    return mWhatString.c_str();
}
