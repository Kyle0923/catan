#include "vertex.hpp"

void Vertex::setOwner(std::string aOwner, BuildingType aBuilding)
{
    mOwner = aOwner;
    mBuilding = aBuilding;
}

std::string Vertex::getOwner()
{
    return mOwner;
}

Vertex::Vertex(int aId, Point_t aTopRight) :
    Terrain(aId, aTopRight)
{
    mOwner = "None";
    mBuilding = BuildingType::NONE;
}

Vertex::~Vertex()
{
}
