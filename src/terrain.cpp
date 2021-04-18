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

Point_t Terrain::getTopLeft() const
{
    return mTopLeft;
}

int Terrain::getId() const
{
    return mId;
}

void Terrain::setColor(ColorPairIndex aColorIndex)
{
    mColorIndex = aColorIndex;
}

void Terrain::registerToMap(GameMap& aMap)
{
    aMap.registerTerrain(getAllPoints(), this);
}

int Terrain::populateAdjacencies(GameMap& aMap)
{
    // empty
    // override by derive classes
    // populate vector<Terrain*> mAdjacencies
    return 0;
}

Terrain::Terrain(const int aId, const Point_t aTopLeft) :
    mId(aId),
    mTopLeft(aTopLeft),
    mColorIndex(ColorPairIndex::COLOR_PAIR_INDEX_RESERVED)
{
    // empty
}

chtype Terrain::getColorCharRepresentation(const size_t aPointX, const size_t aPointY, \
                                            const bool aUseId) const
{
    return UserInterface::getColorText( \
                mColorIndex, getCharRepresentation(aPointX, aPointY, aUseId));
}

std::vector<Point_t> Terrain::getAllPoints() const
{
    return std::vector<Point_t>{mTopLeft};
}

Terrain::~Terrain()
{
    // empty
}