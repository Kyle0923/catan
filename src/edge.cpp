#include "edge.hpp"


Edge::Edge(int aId, Point_t aTopRight) :
    Terrain(aId, aTopRight)
{
    mOwner = "None";
}

Edge::~Edge()
{
}