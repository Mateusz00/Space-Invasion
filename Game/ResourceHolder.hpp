#ifndef RESOURCE_HOLDER_HPP
#define RESOURCE_HOLDER_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include <exception>
#include "Utility.hpp"

template <typename Resource, typename EnumID>
class ResourceHolder
{
    public:
        Resource& get(EnumID);
        const Resource& get(EnumID) const;
        void loadFromFile(EnumID, const std::string& path);
        template <typename Parameter>
        void loadFromFile(EnumID, const std::string& path, const Parameter&);

    private:
        std::unordered_map<int, std::unique_ptr<Resource>> resourceMap;
};

#include "ResourceHolder.inl"
#endif // RESOURCE_HOLDER_HPP
