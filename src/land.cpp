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
        WARN_LOG("Failed to populate adjacencies of ", getStringId(), " at ", mTopLeft)
        :
        DEBUG_LOG_L1("Successfully populated adjacencies of ", getStringId());
    return rc;
}

int Land::addAdjacency(GameMap& aMap, bool aIsVertex, const int aPointX, const int aPointY, const char aPattern)
{
    const Terrain* pTerrain = aMap.getTerrain(aPointX, aPointY);
    bool isCorrectTerrain;
    std::string expectedTerrain;

    // sanity check
    if (aIsVertex)
    {
        isCorrectTerrain = GameMap::isTerrain<Vertex>(pTerrain);
        expectedTerrain = "Vertex";
    }
    else
    {
        isCorrectTerrain = GameMap::isTerrain<Edge>(pTerrain);
        expectedTerrain = "Edge";
    }
    if (!isCorrectTerrain)
    {
        // incorrect terrain
        if (GameMap::isTerrain<Blank>(pTerrain))
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
            DEBUG_LOG_L3("Added new ", pTerrain->getStringId(), " for ", getStringId());
        }
        else
        {
            // collide
            WARN_LOG("At Point [", aPointX, ", ", aPointY, "], Expected " + expectedTerrain + " - Actual ", pTerrain->getStringId());
            return 1;
        }
    }
    // add adjcentVertices need to be after adding terrain
    const Vertex* pVertex = dynamic_cast<const Vertex*>(pTerrain);
    if (aIsVertex && pVertex)
    {
        mAdjacentVertices.push_back(pVertex);
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

void Land::rob(bool aIsRob)
{
    mIsUnderRob = aIsRob;
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
    auto printAtMiddle = [aPointX, this](const int aWidth, const int aOffset, const std::string& aString) {
        // add 1 before division, effectively round up the 0.5 when odd number is divided by 2, allows left justify
        const int PADDING = (aWidth - aString.length() + 1) / 2;
        const int index = (int)aPointX - (int)mTopLeft.x + aOffset - PADDING;
        if (index >= 0 && index < (int)aString.size())
        {
            return aString.at(index);
        }
        else
        {
            return ' ';
        }
    };
    if (aPointY == mTopLeft.y + 2)
    {
        // print label
        const size_t WIDTH = 16;
        const std::string label = resourceTypesToStr(mResourceType);
        return printAtMiddle(WIDTH, 2, label);
    }
    else if (aPointY == mTopLeft.y + 3)
    {
        const size_t WIDTH = 14;
        const size_t offset = 1;
        if (mResourceType != ResourceTypes::DESERT)
        {
            // print dice num
            const std::string robber = (mIsUnderRob ? "#" : "");
            const std::string label = robber + std::to_string(mDiceNum) + robber;
            return printAtMiddle(WIDTH, offset, label);
        }
        else if (mIsUnderRob)
        {
            // desert under rob
            return printAtMiddle(WIDTH, offset, "##");
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
