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
#include "vertex.hpp"
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
    // reset edge
    mOwner = -1;
    mAdjacentVertices.clear();
    mAdjacentEdges.clear();

    int rc = 0;
    std::pair<Point_t, Point_t> vertexPoints = getAdjacentVertexPoints();
    rc |= addAdjacency(aMap, vertexPoints.first);
    rc |= addAdjacency(aMap, vertexPoints.second);

    DEBUG_LOG_L0("Populated Adjacent Vertices for " + getStringId() + " ", mAdjacentVertices);
    DEBUG_LOG_L0("Populated Adjacent Edges for " + getStringId() + " ", mAdjacentEdges);

    (rc != 0) ?
        WARN_LOG("Failed to populate adjacencies of ", getStringId(), " at ", mTopLeft)
        :
        DEBUG_LOG_L1("Successfully populated adjacencies of ", getStringId());
    return rc;
}

int Edge::addAdjacency(GameMap& aMap, const Point_t aPoint)
{
    const Vertex* const pVertex = dynamic_cast<const Vertex*>(aMap.getTerrain(aPoint));
    if (!pVertex)
    {
        // not vertex
        WARN_LOG("At ", aPoint, ", Expected Vertex - Actual ", pVertex->getStringId());
        return 1;
    }
    mAdjacentVertices.emplace(pVertex);
    std::set<const Edge*> edges = pVertex->getOtherEdges(*this);
    mAdjacentEdges.insert(edges.begin(), edges.end());
    return 0;
}

const Vertex* Edge::getOtherVertex(const GameMap& aMap, const Vertex& aVertex) const
{
    std::pair<Point_t, Point_t> vertexPoints = getAdjacentVertexPoints();
    if (vertexPoints.first == aVertex.getTopLeft())
    {
        return dynamic_cast<const Vertex*>(aMap.getTerrain(vertexPoints.second));
    }
    else if (vertexPoints.second == aVertex.getTopLeft())
    {
        return dynamic_cast<const Vertex*>(aMap.getTerrain(vertexPoints.first));
    }

    WARN_LOG("Unknown adjacent vertex: " + aVertex.getStringId() + " at ", aVertex.getTopLeft());
    return nullptr;
}

std::pair<Point_t, Point_t> Edge::getAdjacentVertexPoints() const
{
    switch (mDirection)
    {
    case '-':
        return std::make_pair<Point_t, Point_t>(
            Point_t{mTopLeft.x - 1, mTopLeft.y},
            Point_t{mOtherEnd.x + 1, mOtherEnd.y}
        );
    case '/':
        return std::make_pair<Point_t, Point_t>(
            Point_t{mTopLeft.x + 1, mTopLeft.y - 1},
            Point_t{mOtherEnd.x - 1, mOtherEnd.y + 1}
        );
    case '\\':
        return std::make_pair<Point_t, Point_t>(
            Point_t{mTopLeft.x - 1, mTopLeft.y - 1},
            Point_t{mOtherEnd.x + 1, mOtherEnd.y + 1}
        );
    default:
        ERROR_LOG("Unknow Edge direction [", mDirection, "]");
        return std::make_pair<Point_t, Point_t>(Point_t{0, 0}, Point_t{0, 0});
    }
}

int Edge::setOwner(int aPlayerId)
{
    mOwner = aPlayerId;
    mColorIndex = static_cast<ColorPairIndex>(mOwner + ColorPairIndex::PLAYER_START);
    INFO_LOG("Successfully set owner Player#", mOwner, " for " + getStringId());
    return 0;
}

int Edge::getOwner() const
{
    return mOwner;
}

bool Edge::isAvailable(int aPlayerId) const
{
    for (const Vertex* const pAdjVertex : mAdjacentVertices)
    {
        if (pAdjVertex->getOwner() == aPlayerId)
        {
            return true;
        }
    }
    for (const Edge* const pAdjEdge : mAdjacentEdges)
    {
        if (pAdjEdge->getOwner() == aPlayerId)
        {
            return true;
        }
    }
    return false;
}

Edge::Edge(const int aId, const Point_t aTopLeft, const char aDirection) :
    Terrain(aId, aTopLeft),
    mOwner(-1)
{
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