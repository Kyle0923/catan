#include "game_map.hpp"
#include "edge.hpp"

const std::vector<Point_t> Edge::getAllPoints() const
{
    std::vector<Point_t> allPoints;
    allPoints.push_back(mTopRight);
    switch (mDirection)
    {
        case '-':
            for (int ii = 1; ii <= HORIZONTAL_LENGTH; ++ii)
            {
                allPoints.push_back(Point_t{mTopRight.x + ii, mTopRight.y});
            }
            break;
        case '/':
            allPoints.push_back(Point_t{mTopRight.x - 1, mTopRight.y + 1});
            break;
        case '\\':
            allPoints.push_back(Point_t{mTopRight.x + 1, mTopRight.y + 1});
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
        rc |= populateNeighbour(aMap, mTopRight.x - 1, mTopRight.y);
        rc |= populateNeighbour(aMap, mOtherEnd.x + 1, mOtherEnd.y);
        break;
    case '/':
        rc |= populateNeighbour(aMap, mTopRight.x + 1, mTopRight.y - 1);
        rc |= populateNeighbour(aMap, mOtherEnd.x - 1, mOtherEnd.y + 1);
        break;
    case '\\':
        rc |= populateNeighbour(aMap, mTopRight.x - 1, mTopRight.y - 1);
        rc |= populateNeighbour(aMap, mOtherEnd.x + 1, mOtherEnd.y + 1);
        break;
    default:
        ERROR_LOG("Unknow Edge direction [", mDirection, "]");
        break;
    }
    if (rc)
    {
        ERROR_LOG("Failed to populate neighbours of ", getFullId());
    }
    else
    {
        INFO_LOG("Successfully populated neighbours of ", getFullId());
    }
    return rc;
}

int Edge::populateNeighbour(GameMap& aMap, const size_t aPointX, const size_t aPointY)
{
    Terrain* pTerrain = aMap.getTerrain(aPointX, aPointY);
    if (!dynamic_cast<Vertex*>(pTerrain))
    {
        // not vertex
        WARN_LOG("At Point [", aPointX, ", ", aPointY, "], Expected Vertex - Actual ", pTerrain->getFullId());
        return 1;
    }
    mNeighbour.push_back(pTerrain);
    return 0;
}

Vertex* Edge::getVertex(Vertex* const aVertex)
{
    for (Terrain* const pTerrain : mNeighbour)
    {
        if (pTerrain != aVertex)
            return dynamic_cast<Vertex*>(pTerrain);
    }
    return nullptr;
}

Edge::Edge(const int aId, const Point_t aTopRight, const char aDirection) :
    Terrain(aId, aTopRight)
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
            mOtherEnd = Point_t{mTopRight.x + HORIZONTAL_LENGTH, mTopRight.y};
            break;
        case '/':
            mOtherEnd = Point_t{mTopRight.x - 1, mTopRight.y + 1};
            break;
        case '\\':
            mOtherEnd = Point_t{mTopRight.x + 1, mTopRight.y + 1};
            break;
        default:
            ERROR_LOG("Unknow Edge direction [", mDirection, "]");
            break;
    }
}

char Edge::getCharRepresentation(bool aUseId) const
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

std::string Edge::getFullId() const
{
    return Logger::formatString("Edge#", mId);
}

Edge::~Edge()
{
}