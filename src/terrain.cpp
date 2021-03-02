#include "terrain.hpp"

Point_t Terrain::getTopRight()
{
    return mTopRight;
}

Terrain::Terrain(int aId, Point_t aTopRight) :
    mId(aId),
    mTopRight(aTopRight)
{
    // empty
}

std::vector<Point_t> Terrain::getAllPoints()
{
    return std::vector<Point_t>{mTopRight};
}