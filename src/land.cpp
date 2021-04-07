/**
 * Project: catan
 * @file land.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "logger.hpp"
#include "game_map.hpp"
#include "land.hpp"
#include "blank.hpp"
#include "utility.hpp"

std::vector<Point_t> Land::getAllPoints() const
{
    std::vector<Point_t> allPoints;
    constexpr int horizontalLength = Edge::HORIZONTAL_LENGTH + 3;
    for (int jj = 0; jj <= 2; ++jj)
    {
        for (int ii = -jj; ii < horizontalLength + jj; ++ii)
        {
            allPoints.push_back(Point_t{mTopLeft.x + ii, mTopLeft.y + jj});
        }
    }
    for (int jj = 3; jj <= 4; ++jj)
    {
        for (int ii = jj - 4; ii < horizontalLength + (4 - jj); ++ii)
        {
            allPoints.push_back(Point_t{mTopLeft.x + ii, mTopLeft.y + jj});
        }
    }
    return allPoints;
}

int Land::populateAdjacencies(GameMap& aMap)
{
    // check vertices and edges clockwise from top-left corner
    // add vertex or edge if needed
    int rc = 0;
    constexpr int horizontalLength = Edge::HORIZONTAL_LENGTH + 3;
    // top-left vertex
    rc |= addAdjacency(aMap, true, mTopLeft.x, mTopLeft.y - 1);
    // top left vertex
    rc |= addAdjacency(aMap, true, mTopLeft.x + horizontalLength - 1, mTopLeft.y - 1);
    // left vertex
    rc |= addAdjacency(aMap, true, mTopLeft.x + horizontalLength + 2, mTopLeft.y + 2);
    // bottom left vertex
    rc |= addAdjacency(aMap, true, mTopLeft.x + horizontalLength - 1, mTopLeft.y + 5);
    // bottom right vertex
    rc |= addAdjacency(aMap, true, mTopLeft.x, mTopLeft.y + 5);
    // right vertex
    rc |= addAdjacency(aMap, true, mTopLeft.x - 3, mTopLeft.y + 2);

    // top edge
    rc |= addAdjacency(aMap, false, mTopLeft.x + 1, mTopLeft.y - 1, '-');
    // left top edge
    rc |= addAdjacency(aMap, false, mTopLeft.x + horizontalLength, mTopLeft.y, '\\');
    // left bottom edge
    rc |= addAdjacency(aMap, false, mTopLeft.x + horizontalLength + 1, mTopLeft.y + 3, '/');
    // bottom edge
    rc |= addAdjacency(aMap, false, mTopLeft.x + 1, mTopLeft.y + 5, '-');
    // right bottom edge
    rc |= addAdjacency(aMap, false, mTopLeft.x - 2, mTopLeft.y + 3, '\\');
    // right top edge
    rc |= addAdjacency(aMap, false, mTopLeft.x - 1, mTopLeft.y, '/');

    (rc != 0) ?
        WARN_LOG("Failed to populate adjacencies of ", getStringId())
        :
        DEBUG_LOG_L2("Successfully populated adjacencies of ", getStringId());
    return rc;
}

int Land::addAdjacency(GameMap& aMap, bool aIsVertex, const int aPointX, const int aPointY, const char aPattern)
{
    const Terrain* pTerrain = aMap.getTerrain(aPointX, aPointY);
    bool isCorrectTerrain;
    std::string expectedTerrain;
    if (aIsVertex)
    {
        isCorrectTerrain = static_cast<bool>(dynamic_cast<const Vertex*>(pTerrain));
        expectedTerrain = "Vertex";
    }
    else
    {
        isCorrectTerrain = static_cast<bool>(dynamic_cast<const Edge*>(pTerrain));
        expectedTerrain = "Edge";
    }
    if (!isCorrectTerrain)
    {
        // incorrect terrain
        if (dynamic_cast<const Blank*>(pTerrain) || pTerrain == nullptr)
        {
            // is blank, add correct terrain
            if (aIsVertex)
            {
                pTerrain = aMap.addVertex(aPointX, aPointY);
            }
            else
            {
                pTerrain = aMap.addEdge(aPointX, aPointY, aPattern);
            }
            DEBUG_LOG_L1("Added ", pTerrain->getStringId(), " for ", getStringId());
        }
        else
        {
            // collide
            WARN_LOG("At Point [", aPointX, ", ", aPointY, "], Expected " + expectedTerrain + " - Actual ", pTerrain->getStringId());
            return 1;
        }
    }
    if (aIsVertex)
    {
        mAdjacencies.push_back(pTerrain);
    }
    return 0;
}

void Land::setResourceType(ResourceTypes aResourceType)
{
    mResourceType = aResourceType;
}

ResourceTypes Land::getResourceType()
{
    return mResourceType;
}

Land::Land(const int aId, const Point_t aTopLeft, const ResourceTypes aResourceType) :
    Terrain(aId, aTopLeft),
    mResourceType(aResourceType),
    mDiceNum(0),
    mIsUnderRob(false)
{
    // empty
}

void Land::setDiceNum(int aDice)
{
    mDiceNum = aDice;
}

char Land::getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    if (mResourceType == ResourceTypes::NONE)
    {
        return '.';
    }
    if (aPointY == mTopLeft.y + 2)
    {
        // print label
        const size_t WIDTH = 16;
        const std::string label = resourceTypesToStr(mResourceType);
        const int PADDING = (WIDTH - label.length()) / 2;
        const int index = (int)aPointX - (int)mTopLeft.x + 2 - PADDING;
        if (index >= 0 && index < (int)label.size())
        {
            return label.at(index);
        }
    }
    if (mResourceType != ResourceTypes::DESERT && aPointY == mTopLeft.y + 3)
    {
        // print dice num
        const size_t WIDTH = 14;
        const std::string label = std::to_string(mDiceNum);
        const int PADDING = (WIDTH - label.length()) / 2;
        const int index = (int)aPointX - (int)mTopLeft.x + 1 - PADDING;
        if (index >= 0 && index < (int)label.size())
        {
            return label.at(index);
        }
    }
    return ' ';
}

std::string Land::getStringId() const
{
    return Logger::formatString("Land#", mId);
}

int Land::getDiceNum()
{
    return mDiceNum;
}

Land::~Land()
{
}
