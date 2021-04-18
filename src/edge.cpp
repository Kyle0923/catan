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
#include "utility.hpp"

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

int Edge::populateAdjacencies(GameMap& aMap)
{
    mOwner = ""; // reset edge
    int rc = 0;
    switch (mDirection)
    {
    case '-':
        rc |= addAdjacency(aMap, mTopLeft.x - 1, mTopLeft.y);
        rc |= addAdjacency(aMap, mOtherEnd.x + 1, mOtherEnd.y);
        break;
    case '/':
        rc |= addAdjacency(aMap, mTopLeft.x + 1, mTopLeft.y - 1);
        rc |= addAdjacency(aMap, mOtherEnd.x - 1, mOtherEnd.y + 1);
        break;
    case '\\':
        rc |= addAdjacency(aMap, mTopLeft.x - 1, mTopLeft.y - 1);
        rc |= addAdjacency(aMap, mOtherEnd.x + 1, mOtherEnd.y + 1);
        break;
    default:
        ERROR_LOG("Unknow Edge direction [", mDirection, "]");
        break;
    }
    (rc != 0) ?
        WARN_LOG("Failed to populate adjacencies of ", getStringId())
        :
        DEBUG_LOG_L1("Successfully populated adjacencies of ", getStringId());
    return rc;
}

int Edge::addAdjacency(GameMap& aMap, const size_t aPointX, const size_t aPointY)
{
    const Terrain* const pTerrain = aMap.getTerrain(aPointX, aPointY);
    if (!GameMap::isTerrain<Vertex>(pTerrain))
    {
        // not vertex
        WARN_LOG("At Point [", aPointX, ", ", aPointY, "], Expected Vertex - Actual ", pTerrain->getStringId());
        return 1;
    }
    mAdjacencies.push_back(pTerrain);
    return 0;
}

const Vertex* Edge::getOtherVertex(const Vertex* const aVertex) const
{
    bool isAdjacent = false;
    const Vertex* pVertex = nullptr;
    for (const Terrain* const pTerrain : mAdjacencies)
    {
        if (pTerrain == aVertex)
        {
            isAdjacent = true;
        }
        else
        {
            pVertex = dynamic_cast<const Vertex*>(pTerrain);
        }
    }
    if (!isAdjacent)
    {
        WARN_LOG("Unknown adjacent vertex: " + aVertex->getStringId());
        return nullptr;
    }
    return pVertex;
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