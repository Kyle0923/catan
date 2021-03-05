#include "port.hpp"

ResourceTypes Port::getResourceType()
{
    return mResourceType;
}

Port::Port(int aId, Point_t aTopRight, ResourceTypes aResourceType) :
    Terrain(aId, aTopRight),
    mResourceType(aResourceType)
{
    // empty
}

Port::~Port()
{
}
