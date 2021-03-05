#ifndef INCLUDE_PORT_HPP
#define INCLUDE_PORT_HPP

#include "terrain.hpp"

class Port : public Terrain
{
private:
    ResourceTypes mResourceType;
public:
    ResourceTypes getResourceType();
    Port(int aId, Point_t aTopRight, ResourceTypes aResourceType);
    ~Port();
};

#endif /* INCLUDE_PORT_HPP */
