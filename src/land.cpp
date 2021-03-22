#include "game_map.hpp"
#include "land.hpp"
#include "blank.hpp"

std::vector<Point_t> Land::getAllPoints() const
{
    std::vector<Point_t> allPoints;
    constexpr int horizontalLength = Edge::HORIZONTAL_LENGTH + 3;
    for (int jj = 0; jj <= 2; ++jj)
    {
        for (int ii = -jj; ii < horizontalLength + jj; ++ii)
        {
            allPoints.push_back(Point_t{mTopRight.x + ii, mTopRight.y + jj});
        }
    }
    for (int jj = 3; jj <= 4; ++jj)
    {
        for (int ii = jj - 4; ii < horizontalLength + (4 - jj); ++ii)
        {
            allPoints.push_back(Point_t{mTopRight.x + ii, mTopRight.y + jj});
        }
    }
    return allPoints;
}

int Land::populateNeighbours(GameMap& aMap)
{
    // check vertices and edges clockwise from top-right corner
    // add vertex or edge if needed
    int rc = 0;
    constexpr int horizontalLength = Edge::HORIZONTAL_LENGTH + 3;
    // top right vertex
    rc |= populateNeighbour(aMap, true, mTopRight.x, mTopRight.y - 1);
    // top left vertex
    rc |= populateNeighbour(aMap, true, mTopRight.x + horizontalLength - 1, mTopRight.y - 1);
    // left vertex
    rc |= populateNeighbour(aMap, true, mTopRight.x + horizontalLength + 2, mTopRight.y + 2);
    // bottom left vertex
    rc |= populateNeighbour(aMap, true, mTopRight.x + horizontalLength - 1, mTopRight.y + 5);
    // bottom right vertex
    rc |= populateNeighbour(aMap, true, mTopRight.x, mTopRight.y + 5);
    // right vertex
    rc |= populateNeighbour(aMap, true, mTopRight.x - 3, mTopRight.y + 2);

    // top edge
    rc |= populateNeighbour(aMap, false, mTopRight.x + 1, mTopRight.y - 1, '-');
    // left top edge
    rc |= populateNeighbour(aMap, false, mTopRight.x + horizontalLength, mTopRight.y, '\\');
    // left bottom edge
    rc |= populateNeighbour(aMap, false, mTopRight.x + horizontalLength + 1, mTopRight.y + 3, '/');
    // bottom edge
    rc |= populateNeighbour(aMap, false, mTopRight.x + 1, mTopRight.y + 5, '-');
    // right bottom edge
    rc |= populateNeighbour(aMap, false, mTopRight.x - 2, mTopRight.y + 3, '\\');
    // right top edge
    rc |= populateNeighbour(aMap, false, mTopRight.x - 1, mTopRight.y, '/');

    (rc != 0) ?
        ERROR_LOG("Failed to populate neighbours of ", getFullId())
        :
        INFO_LOG("Successfully populated neighbours of ", getFullId());
    return rc;
}

int Land::populateNeighbour(GameMap& aMap, bool aIsVertex, const int aPointX, const int aPointY, const char aPattern)
{
    Terrain* pTerrain = aMap.getTerrain(aPointX, aPointY);
    bool isCorrectTerrain;
    std::string expectedTerrain;
    if (aIsVertex)
    {
        isCorrectTerrain = static_cast<bool>(dynamic_cast<Vertex*>(pTerrain));
        expectedTerrain = "Vertex";
    }
    else
    {
        isCorrectTerrain = static_cast<bool>(dynamic_cast<Edge*>(pTerrain));
        expectedTerrain = "Edge";
    }
    if (!isCorrectTerrain)
    {
        // incorrect terrain
        if (dynamic_cast<Blank*>(pTerrain) || pTerrain == nullptr)
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
            DEBUG_LOG("Added ", pTerrain->getFullId(), " for ", getFullId());
        }
        else
        {
            // collide
            WARN_LOG("At Point [", aPointX, ", ", aPointY, "], Expected " + expectedTerrain + " - Actual ", pTerrain->getFullId());
            return 1;
        }
    }
    if (aIsVertex)
    {
        mNeighbour.push_back(pTerrain);
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

Land::Land(const int aId, const Point_t aTopRight, const ResourceTypes aResourceType) :
    Terrain(aId, aTopRight),
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

char Land::getCharRepresentation(bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    else
    {
        if (mResourceType == ResourceTypes::NONE)
        {
            return '.';
        }
        else
        {
            return static_cast<char>((int)mResourceType) + '0';
        }
    }
}

std::string Land::getFullId() const
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
