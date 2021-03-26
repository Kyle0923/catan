/**
 * Project: catan
 * @file terrain.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "terrain.hpp"
#include "game_map.hpp"

Point_t Terrain::getTopRight()
{
    return mTopRight;
}

int Terrain::getId() const
{
    return mId;
}

void Terrain::registerToMap(GameMap& aMap)
{
    aMap.registerTerrain(getAllPoints(), this);
}

int Terrain::populateNeighbours(GameMap& aMap)
{
    // empty
    // override by derive classes
    // populate vector<Terrain*> mNeighbour
    return 0;
}

Terrain::Terrain(const int aId, const Point_t aTopRight) :
    mId(aId),
    mTopRight(aTopRight)
{
    // empty
}

std::vector<Point_t> Terrain::getAllPoints() const
{
    return std::vector<Point_t>{mTopRight};
}

Terrain::~Terrain()
{
    // empty
}