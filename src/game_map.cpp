#include <unordered_map>
#include "utility.hpp"
#include "game_map.hpp"
#include "blank.hpp"

void GameMap::fillInBlank()
{
    // traverse map, replace nullptr with Blank*
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        std::deque<Terrain*>& row = mGameMap.at(jj);
        for (int ii = 0; ii < mSizeHorizontal; ++ii)
        {
            if (row.at(ii) == nullptr)
            {
                row.at(ii) = Blank::getBlank();
            }
        }
    }
}

int GameMap::populateMap()
{
    /* populate terrains
     * order matters!!
     * Land::populateNeighbours adds necessary edges and vertices and records neighbour vertices
     * Edge::populateNeighbours records vertices on both end
     * Vertex::populateNeighbours records connected edges and adjcent vertices
     */
    int rc = 0;
    for (Land* const pLand : mLands)
    {
        rc |= pLand->populateNeighbours(*this);
    }
    for (Edge* const pEdge : mEdges)
    {
        rc |= pEdge->populateNeighbours(*this);
    }
    for (Vertex* const pVertex : mVertices)
    {
        rc |= pVertex->populateNeighbours(*this);
    }

    return rc;
}

int GameMap::checkOverlap() const
{
    std::unordered_map<Point_t, std::string, PointHash> points;
    int overlapCount = 0;
    auto lambda = [&points, &overlapCount](const std::vector<Point_t>& aPoints, const std::string& aId)
        {
            for (const Point_t& point : aPoints)
            {
                int count = points.count(point);
                if (count) {
                    overlapCount += count;
                    WARN_LOG("Overlap detected at Point [", point, \
                         "] Collision IDs: ", aId, ", ", points.at(point));
                }
                else
                {
                    points.emplace(point, aId);
                }
            }
        };

    for (Vertex* const pVertex : mVertices)
    {
        lambda(pVertex->getAllPoints(), Logger::formatString(pVertex->getFullId()));
    }
    for (Edge* const pEdge : mEdges)
    {
        lambda(pEdge->getAllPoints(), Logger::formatString(pEdge->getFullId()));
    }
    for (Land* const pLand : mLands)
    {
        lambda(pLand->getAllPoints(), Logger::formatString(pLand->getFullId()));
    }
    if (overlapCount)
    {
        ERROR_LOG("Detected ", overlapCount, " Overlap");
    }
    else
    {
        INFO_LOG("No Overlap detected");
    }
    return overlapCount;
}

Terrain* const GameMap::getTerrain(const int x, const int y)
{
    if (boundaryCheck(x,y))
    {
        return mGameMap.at(y).at(x);
    }
    else
    {
        WARN_LOG("Coord (", x, ", ", y, ") is out of bound, need to resize first");
        return nullptr;
    }
}

Terrain* const GameMap::getTerrain(const Point_t& aPoint)
{
    return getTerrain(aPoint.x, aPoint.y);
}

Terrain* const GameMap::addVertex(const size_t aTopRightX, const size_t aTopRightY)
{
    Vertex* const pVertex = new Vertex(mVertices.size(), Point_t{aTopRightX, aTopRightY});
    pVertex->registerToMap(*this);
    mVertices.push_back(pVertex);
    return pVertex;
}

Terrain* const GameMap::addEdge(const size_t aTopRightX, const size_t aTopRightY, const char aPattern)
{
    Edge* const pEdge = new Edge(mEdges.size(), Point_t{aTopRightX, aTopRightY}, aPattern);
    pEdge->registerToMap(*this);
    mEdges.push_back(pEdge);
    return pEdge;

}

Terrain* const GameMap::addLand(const size_t aTopRightX, const size_t aTopRightY, const ResourceTypes aResource)
{
    Land* const pLand = new Land (mLands.size(), Point_t{aTopRightX, aTopRightY}, aResource);
    pLand->registerToMap(*this);
    mLands.push_back(pLand);
    return pLand;
}

int GameMap::registerTerrain(const std::vector<Point_t>& aPoints, Terrain* const aTerrain)
{
    int rc = 0;
    for (std::vector<Point_t>::const_iterator it = aPoints.begin() ; it != aPoints.end(); ++it)
    {
        rc += registerTerrain(*it, aTerrain);
    }
    return rc;
}

int GameMap::registerTerrain(const Point_t& aPoint, Terrain* const aTerrain)
{
    return registerTerrain(aPoint.x, aPoint.y, aTerrain);
}

int GameMap::registerTerrain(const int x, const int y, Terrain* const aTerrain)
{
    if (boundaryCheck(x, y)) {
        mGameMap.at(y).at(x) = aTerrain;
        return 0;
    }
    else
    {
        WARN_LOG("Coord (", x, ", ", y, ") is out of bound");
        return -1;
    }

}

int GameMap::initMap()
{
    fillInBlank();
    populateMap();
    checkOverlap();
    return 0;
}

bool GameMap::boundaryCheck(int x, int y)
{
    return (x < mSizeHorizontal && y < mSizeVertical);
}

void GameMap::printMap(bool aUseId)
{
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        const std::deque<Terrain*>& row = mGameMap.at(jj);
        for (int ii = 0; ii < mSizeHorizontal; ++ii)
        {
            // easier to debug using char c
            char c = row.at(ii)->getCharRepresentation(aUseId); //print ID
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

GameMap::GameMap(int aSizeHorizontal, int aSizeVertical) :
    mSizeHorizontal(aSizeHorizontal),
    mSizeVertical(aSizeVertical)
{
    // init a 2D array with (Terrain*)nullptr
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        std::deque<Terrain*> row(mSizeHorizontal, nullptr);
        mGameMap.push_back(row);
    }
}

GameMap::~GameMap()
{
    for (Vertex* pVertex : mVertices)
    {
        delete pVertex;
    }
    for (Edge* pEdge : mEdges)
    {
        delete pEdge;
    }
    for (Land* pLand : mLands)
    {
        delete pLand;
    }
}
