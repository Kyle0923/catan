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

char Vertex::getCharRepresentation(bool useId) const
{
    if (useId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    else
    {
        return '+';
    }
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
