#include <unordered_map>
#include <set>
#include <cstdlib>
#include <ctime>
#include <algorithm>
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

int GameMap::populateHarbours(bool aUseDefaultPosition, bool aUseDefaultResourceType)
{
    int rc = 0;
    if (aUseDefaultPosition)
    {
        rc = createHarboursDefault();
    }
    else
    {
        rc = createHarboursRandom();
    }
    if (aUseDefaultResourceType)
    {
        size_t index = 0;
        int resourceType = static_cast<int>(ResourceTypes::CLAY);
        for (; index < mHarbours.size(); ++index)
        {
            if (index % 2)
            {
                mHarbours[index]->setResourceType(static_cast<ResourceTypes>(resourceType++));
            }
            else
            {
                mHarbours[index]->setResourceType(ResourceTypes::ANY);
            }
        }
    }
    else
    {
        // one harbour for each resource, ANY for the rest
        int resourceType = static_cast<int>(ResourceTypes::CLAY);
        std::set<size_t> resources;
        while (resources.size() < 5)
        {
            size_t index = rand() % mHarbours.size();
            resources.emplace(index);
        }
        for (Harbour* const pHarbour : mHarbours)
        {
            if (resources.count(pHarbour->getId()))
            {
                pHarbour->setResourceType(static_cast<ResourceTypes>(resourceType));
            }
            else
            {
                pHarbour->setResourceType(ResourceTypes::ANY);
            }
        }
    }
    for (Harbour* const pHarbour : mHarbours)
    {
        rc |= pHarbour->calculatePoints(*this);
        pHarbour->registerToMap(*this);
    }
    return rc;
}

int GameMap::createHarboursDefault()
{
    std::vector<int> harbourCandidates;
    Vertex* pVertex = nullptr;
    // find a coastal vertex as the starting point
    for (Vertex* pVertexTemp : mVertices)
    {
        if (pVertexTemp->isCoastal())
        {
            harbourCandidates.push_back(pVertexTemp->getId());
            pVertexTemp->setHarbour(nullptr);
            pVertex = pVertexTemp;
            break;
        }
    }
    if (!pVertex)
    {
        ERROR_LOG("Unable to find a string point");
        return 1;
    }
    Vertex* pPreviousVertex = pVertex;
    do
    {
        const std::vector<Vertex*>& adjacentVertices = pVertex->getAdjacentVertices();
        for (Vertex* const pNextVertex : adjacentVertices)
        {
            if (pNextVertex == pPreviousVertex)
            {
                continue;
            }
            if (pNextVertex->isCoastal())
            {
                harbourCandidates.push_back(pNextVertex->getId());
                pNextVertex->setHarbour(nullptr);
                pPreviousVertex = pVertex;
                pVertex = pNextVertex;
                break;
            }
        }
    } while (pVertex->getId() != harbourCandidates.at(0));

    const int gap[] = {1, 1, 2};
    size_t index = 0;
    while (mHarbours.size() < mNumHarbour && index + 1 < harbourCandidates.size())
    {
        addHarbour(harbourCandidates[index], harbourCandidates[index + 1]);
        index += 2 + gap[mHarbours.size() % 3];
    }
    bool rc = (mHarbours.size() != mNumHarbour);
    rc ?
        WARN_LOG("Failed to create all harbours, created ", mHarbours.size(), " expected ", mNumHarbour)
        :
        INFO_LOG("Successfully create all harbours");

    return rc;
}

int GameMap::createHarboursRandom()
{
    std::vector<int> harbourCandidates;
    for (Vertex* const pVertex : mVertices)
    {
        if (pVertex->isCoastal() && !pVertex->hasHarbour())
        {
            harbourCandidates.push_back(pVertex->getId());
        }
    }
    srand (time(NULL));
    while (mHarbours.size() < mNumHarbour)
    {
        if (harbourCandidates.size() == 0)
        {
            WARN_LOG("Unable to create ", mNumHarbour, " of harbours, current number of harbour ", mHarbours.size());
            break;
        }
        size_t index = rand() % harbourCandidates.size();
        int idVertex = harbourCandidates[index];
        if (mVertices[idVertex]->hasHarbour())
        {
            continue;
        }
        const std::vector<Vertex*>& adjacentVertices = mVertices[idVertex]->getAdjacentVertices();
        int idOtherVertex = -1;
        for (const Vertex* const pVertex : adjacentVertices)
        {
            if (pVertex->isCoastal() && !pVertex->hasHarbour())
            {
                idOtherVertex = pVertex->getId();
                break;
            }
        }
        if (idOtherVertex != -1)
        {
            // found a valid pair for harbour
            addHarbour(idVertex, idOtherVertex);
            harbourCandidates.erase(std::remove(harbourCandidates.begin(), harbourCandidates.end(), idOtherVertex), \
                                    harbourCandidates.end());
        }
        // if idOtherVertex != -1, add harbour, remove idVertex from harbourCandidates
        // if idOtherVertex == -1, no available adjacent vertex, remove idVertex from harbourCandidates
        harbourCandidates.erase(std::remove(harbourCandidates.begin(), harbourCandidates.end(), idVertex), \
                                harbourCandidates.end());
    }
    return (mHarbours.size() != mNumHarbour);
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
    for (Harbour* const pHarbour : mHarbours)
    {
        lambda(pHarbour->getAllPoints(), Logger::formatString(pHarbour->getFullId()));
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

Terrain* GameMap::getTerrain(const int x, const int y)
{
    if (boundaryCheck(x,y))
    {
        return mGameMap.at(y).at(x);
    }
    else
    {
        WARN_LOG("Coord (", x, ", ", y, ") is out of bound");
        return nullptr;
    }
}

Terrain* GameMap::getTerrain(const Point_t& aPoint)
{
    return getTerrain(aPoint.x, aPoint.y);
}

Vertex* GameMap::addVertex(const size_t aTopRightX, const size_t aTopRightY)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot add vertex");
        return nullptr;
    }
    Vertex* const pVertex = new Vertex(mVertices.size(), Point_t{aTopRightX, aTopRightY});
    pVertex->registerToMap(*this);
    mVertices.push_back(pVertex);
    return pVertex;
}

Edge* GameMap::addEdge(const size_t aTopRightX, const size_t aTopRightY, const char aPattern)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot add edge");
        return nullptr;
    }
    Edge* const pEdge = new Edge(mEdges.size(), Point_t{aTopRightX, aTopRightY}, aPattern);
    pEdge->registerToMap(*this);
    mEdges.push_back(pEdge);
    return pEdge;

}

Land* GameMap::addLand(const size_t aTopRightX, const size_t aTopRightY, const ResourceTypes aResource)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot add land");
        return nullptr;
    }
    Land* const pLand = new Land(mLands.size(), Point_t{aTopRightX, aTopRightY}, aResource);
    pLand->registerToMap(*this);
    mLands.push_back(pLand);
    return pLand;
}

Harbour* GameMap::addHarbour(const int aId1, const int aId2)
{
    DEBUG_LOG("adding port#", mHarbours.size(), " for ", aId1, " and ", aId2);
    Harbour* const pHarbour = new Harbour(mHarbours.size(), ResourceTypes::NONE, \
                                        mVertices[aId1]->getTopRight(), mVertices[aId2]->getTopRight());
    mVertices[aId1]->setHarbour(pHarbour);
    mVertices[aId2]->setHarbour(pHarbour);

    mHarbours.push_back(pHarbour);
    return pHarbour;
}

// int GameMap::addHarbourCandidate(Vertex* const aVertex)
// {
//     if (aVertex)
//     {
//         mHarbourCandidates.emplace(aVertex->getId());
//     }
//     return 0;
// }

int GameMap::registerTerrain(const std::vector<Point_t>& aPoints, Terrain* const aTerrain)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot registerTerrain for " + aTerrain->getFullId());
        return 1;
    }
    int rc = 0;
    for (std::vector<Point_t>::const_iterator it = aPoints.begin() ; it != aPoints.end(); ++it)
    {
        rc |= registerTerrain(*it, aTerrain);
    }
    return rc;
}

int GameMap::registerTerrain(const Point_t& aPoint, Terrain* const aTerrain)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot registerTerrain for " + aTerrain->getFullId());
        return 1;
    }
    return registerTerrain(aPoint.x, aPoint.y, aTerrain);
}

int GameMap::registerTerrain(const int x, const int y, Terrain* const aTerrain)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot registerTerrain for " + aTerrain->getFullId());
        return 1;
    }
    if (boundaryCheck(x, y)) {
        mGameMap.at(y).at(x) = aTerrain;
        return 0;
    }
    else
    {
        ERROR_LOG("Failed to register point for " + aTerrain->getFullId(), " at (" , x, ", ", y);
        return 1;
    }
}

void GameMap::setNumOfHarbour(const size_t aNum)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot setNumOfHarbour");
    }
    mNumHarbour = aNum;
}

int GameMap::initMap()
{
    int rc = 0;
    fillInBlank();
    if (!mNumHarbour)
    {
        setNumOfHarbour();
    }
    rc |= populateMap();
    rc |= populateHarbours(true, true); // (bool aUseDefaultPosition, bool aUseDefaultResourceType)
    rc |= checkOverlap();
    // TODO: assign dice num to land, assign resource to land

    if (rc != 0)
    {
        WARN_LOG("failed to initialize GameMap");
    }
    else
    {
        INFO_LOG("Successfully initialized GameMap");
        mInitialized = true;
    }
    return rc;
}

bool GameMap::boundaryCheck(const int x, const int y) const
{
    return (x < mSizeHorizontal && y < mSizeVertical);
}

void GameMap::printMap(bool aUseId)
{
    if (!mInitialized)
    {
        WARN_LOG("Map not initialized, printMap may not function as expected");
    }
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

int GameMap::clearAndResize(const int aSizeHorizontal, const int aSizeVertical)
{
    mSizeHorizontal = aSizeHorizontal;
    mSizeVertical = aSizeVertical;
    mNumHarbour = 0;
    mInitialized = false;
    mVertices.clear();
    mEdges.clear();
    mLands.clear();
    mHarbours.clear();
    mGameMap.clear();
    // init a 2D array with (Terrain*)nullptr
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        std::deque<Terrain*> row(mSizeHorizontal, nullptr);
        mGameMap.push_back(row);
    }
    return 0;
}

GameMap::GameMap(const int aSizeHorizontal, const int aSizeVertical) :
    mSizeHorizontal(aSizeHorizontal),
    mSizeVertical(aSizeVertical),
    mNumHarbour(0),
    mInitialized(false)
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
