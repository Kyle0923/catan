#include "port.hpp"

ResourceTypes Port::getResourceType()
{
    return mResourceType;
}

Port::Port(const int aId, const Point_t aTopRight, const ResourceTypes aResourceType) :
    Terrain(aId, aTopRight),
    mResourceType(aResourceType)
{
    // empty
}

Port::~Port()
{
}
