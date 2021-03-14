#include "terrain.hpp"
#include "game_map.hpp"

Point_t Terrain::getTopRight()
{
    return mTopRight;
}

void Terrain::registerToMap(GameMap& aMap)
{
    aMap.registerTerrain(getAllPoints(), this);
}

Terrain::Terrain(const int aId, const Point_t aTopRight) :
    mId(aId),
    mTopRight(aTopRight)
{
    // empty
}

std::vector<Point_t> Terrain::getAllPoints()
{
    return std::vector<Point_t>{mTopRight};
}

Terrain::~Terrain()
{
    // empty
}