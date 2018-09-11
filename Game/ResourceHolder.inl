template <typename Resource, typename EnumID>
Resource& ResourceHolder<Resource, EnumID>::get(EnumID id)
{
    auto foundPosition = resourceMap.find(id);
    if(foundPosition == resourceMap.end())
    {
        std::string errorText("Could not find resource associated with ID: ");
        errorText += toString(static_cast<int>(id));
        throw std::logic_error(errorText);
    }
    return *(foundPosition->second);
}

template <typename Resource, typename EnumID>
const Resource& ResourceHolder<Resource, EnumID>::get(EnumID id) const
{
    auto foundPosition = resourceMap.find(id);
    if(foundPosition == resourceMap.end())
    {
        std::string errorText("Could not find resource associated with ID: ");
        errorText += toString(static_cast<int>(id));
        throw std::logic_error(errorText);
    }
    return *(foundPosition->second);
}

template <typename Resource, typename EnumID>
void ResourceHolder<Resource, EnumID>::loadFromFile(EnumID id, const std::string& path)
{
    std::unique_ptr<Resource> resourcePtr(new Resource);
    if(!resourcePtr->loadFromFile(path))
        throw std::logic_error("Could not find file: " + path);

    auto isEmplaced = resourceMap.emplace(id, std::move(resourcePtr));
    if(!isEmplaced.second)
        throw std::logic_error("Could not insert resource associated with ID: " + toString(id));
}

template <typename Resource, typename EnumID>
template <typename Parameter>
void ResourceHolder<Resource, EnumID>::loadFromFile(EnumID id, const std::string& path, const Parameter& parameter)
{
    std::unique_ptr<Resource> resourcePtr(new Resource);
    if(!resourcePtr->loadFromFile(path, parameter))
        throw std::logic_error("Could not find file: " + path);

    auto isEmplaced = resourceMap.emplace(id, std::move(resourcePtr));
    if(!isEmplaced->second)
        throw std::logic_error("Could not insert resource associated with ID: " + toString(id));
}
