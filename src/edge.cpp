/**
 * Project: catan
 * @file edge.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "logger.hpp"
#include "edge.hpp"
#include "game_map.hpp"

std::vector<Point_t> Edge::getAllPoints() const
{
    std::vector<Point_t> allPoints;
    allPoints.push_back(mTopLeft);
    switch (mDirection)
    {
        case '-':
            for (int ii = 1; ii <= HORIZONTAL_LENGTH; ++ii)
            {
                allPoints.push_back(Point_t{mTopLeft.x + ii, mTopLeft.y});
            }
            break;
        case '/':
            allPoints.push_back(Point_t{mTopLeft.x - 1, mTopLeft.y + 1});
            break;
        case '\\':
            allPoints.push_back(Point_t{mTopLeft.x + 1, mTopLeft.y + 1});
            break;
        default:
            ERROR_LOG("Unknow Edge direction [", mDirection, "]");
            break;
    }
    return allPoints;
}

int Edge::populateNeighbours(GameMap& aMap)
{
    mOwner = ""; // reset edge
    int rc = 0;
    switch (mDirection)
    {
    case '-':
        rc |= populateNeighbour(aMap, mTopLeft.x - 1, mTopLeft.y);
        rc |= populateNeighbour(aMap, mOtherEnd.x + 1, mOtherEnd.y);
        break;
    case '/':
        rc |= populateNeighbour(aMap, mTopLeft.x + 1, mTopLeft.y - 1);
        rc |= populateNeighbour(aMap, mOtherEnd.x - 1, mOtherEnd.y + 1);
        break;
    case '\\':
        rc |= populateNeighbour(aMap, mTopLeft.x - 1, mTopLeft.y - 1);
        rc |= populateNeighbour(aMap, mOtherEnd.x + 1, mOtherEnd.y + 1);
        break;
    default:
        ERROR_LOG("Unknow Edge direction [", mDirection, "]");
        break;
    }
    (rc != 0) ?
        WARN_LOG("Failed to populate neighbours of ", getStringId())
        :
        DEBUG_LOG_L2("Successfully populated neighbours of ", getStringId());
    return rc;
}

int Edge::populateNeighbour(GameMap& aMap, const size_t aPointX, const size_t aPointY)
{
    const Terrain* const pTerrain = aMap.getTerrain(aPointX, aPointY);
    if (!dynamic_cast<const Vertex*>(pTerrain))
    {
        // not vertex
        WARN_LOG("At Point [", aPointX, ", ", aPointY, "], Expected Vertex - Actual ", pTerrain->getStringId());
        return 1;
    }
    mNeighbour.push_back(pTerrain);
    return 0;
}

const Vertex* Edge::getVertex(Vertex* const aVertex) const
{
    for (const Terrain* const pTerrain : mNeighbour)
    {
        if (pTerrain != aVertex)
            return dynamic_cast<const Vertex*>(pTerrain);
    }
    return nullptr;
}

Edge::Edge(const int aId, const Point_t aTopLeft, const char aDirection) :
    Terrain(aId, aTopLeft)
{
    mOwner = "None";
    if (aDirection == '-' || aDirection == '/' || aDirection == '\\' )
    {
        mDirection = aDirection;
    }
    else
    {
        ERROR_LOG("Unknow Edge direction [", aDirection, "]");
    }
    switch (mDirection)
    {
        case '-':
            mOtherEnd = Point_t{mTopLeft.x + HORIZONTAL_LENGTH, mTopLeft.y};
            break;
        case '/':
            mOtherEnd = Point_t{mTopLeft.x - 1, mTopLeft.y + 1};
            break;
        case '\\':
            mOtherEnd = Point_t{mTopLeft.x + 1, mTopLeft.y + 1};
            break;
        default:
            ERROR_LOG("Unknow Edge direction [", mDirection, "]");
            break;
    }
}

char Edge::getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    else
    {
        return mDirection;
    }
}

std::string Edge::getStringId() const
{
    return Logger::formatString("Edge#", mId);
}

Edge::~Edge()
{
}