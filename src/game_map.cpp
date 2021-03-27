/**
 * Project: catan
 * @file game_map.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include <unordered_map>
#include <algorithm>
#include <random>
#include <chrono>
#include <set>
#include <cstdlib>
#include "logger.hpp"
#include "utility.hpp"
#include "game_map.hpp"
#include "blank.hpp"
#include "constant.hpp"

void GameMap::fillInBlank()
{
    // traverse map, replace nullptr with Blank*
    for (size_t jj = 0; jj < mSizeVertical; ++jj)
    {
        std::deque<Terrain*>& row = mGameMap.at(jj);
        for (size_t ii = 0; ii < mSizeHorizontal; ++ii)
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

    // return codes
    int rcLand = 0;
    int rcEdge = 0;
    int rcVertex = 0;

    for (Land* const pLand : mLands)
    {
        rcLand |= pLand->populateNeighbours(*this);
    }
    rcLand ?
        ERROR_LOG("Failed to populate neighbours for Lands")
        :
        INFO_LOG("Successfully populated neighbours of Lands");

    for (Edge* const pEdge : mEdges)
    {
        rcEdge |= pEdge->populateNeighbours(*this);
    }
    rcEdge ?
        ERROR_LOG("Failed to populate neighbours for Edges")
        :
        INFO_LOG("Successfully populated neighbours of Edges");

    for (Vertex* const pVertex : mVertices)
    {
        rcVertex |= pVertex->populateNeighbours(*this);
    }
    rcVertex ?
        ERROR_LOG("Failed to populate neighbours for Vertices")
        :
        INFO_LOG("Successfully populated neighbours of Vertices");

    return (rcLand | rcEdge | rcVertex);
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
    SequenceConfig_t config(static_cast<size_t>(ResourceTypes::ANY) + 1);
    config[ResourceTypes::CLAY]  = constant::NUM_HARBOUR_CLAY;
    config[ResourceTypes::SHEEP] = constant::NUM_HARBOUR_SHEEP;
    config[ResourceTypes::WHEAT] = constant::NUM_HARBOUR_WHEAT;
    config[ResourceTypes::WOOD]  = constant::NUM_HARBOUR_WOOD;
    config[ResourceTypes::ORE]   = constant::NUM_HARBOUR_ORE;
    if (aUseDefaultResourceType && mHarbours.size() == constant::NUM_OF_HARBOUR)
    {
        DEBUG_LOG_L3("Using default 9 harbours");
        config[ResourceTypes::ANY] = constant::NUM_HARBOUR_ANY;
        std::vector<int> randomSeq = randomizeResource(config);
        for (size_t index = 0; index < mHarbours.size(); ++index)
        {
            if (index % 2 != 0)
            {
                mHarbours[index]->setResourceType(ResourceTypes::ANY);
            }
            else
            {
                mHarbours[index]->setResourceType(static_cast<ResourceTypes>(randomSeq.at(index / 2)));
            }
        }
    }
    else
    {
        if (mHarbours.size() > constant::NUM_HARBOUR_RESOURCE)
        {
            // one harbour for each resource, ANY for the rest
            config[ResourceTypes::ANY] = mHarbours.size() - constant::NUM_HARBOUR_RESOURCE;
        }
        else
        {
            // set num-of-ANY to default, shuffle the sequence and pick by luck
            INFO_LOG("User defined map has ", mHarbours.size(), " harbours only, randomly picking from the 9 default harbours");
            config[ResourceTypes::ANY] = constant::NUM_HARBOUR_ANY;
        }

        std::vector<int> randomSeq = randomizeResource(config);
        for (size_t index = 0; index < mHarbours.size(); ++index)
        {
            mHarbours[index]->setResourceType(static_cast<ResourceTypes>(randomSeq[index]));
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

    const size_t gap[] = {1U, 1U, 2U};
    size_t index = 0U;
    while (mHarbours.size() < mNumHarbour && index + 1 < harbourCandidates.size())
    {
        addHarbour(harbourCandidates[index], harbourCandidates[index + 1]);
        index += 2U + gap[mHarbours.size() % 3];
    }
    bool rc = (mHarbours.size() != mNumHarbour);
    rc ?
        WARN_LOG("Failed to create all harbours, created ", mHarbours.size(), " expected ", mNumHarbour)
        :
        INFO_LOG("Successfully created all harbours");

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
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    while (mHarbours.size() < mNumHarbour)
    {
        if (harbourCandidates.size() == 0U)
        {
            WARN_LOG("Unable to create ", mNumHarbour, " of harbours, current number of harbour ", mHarbours.size());
            break;
        }
        // get a vertex from candidates
        std::uniform_int_distribution<size_t> distribution(0U, harbourCandidates.size() - 1U);
        size_t index = distribution(generator);
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

std::vector<int> GameMap::randomizeResource(SequenceConfig_t aConfig)
{
    std::vector<int> resourceSequence;
    for (int index = 0; index < (int)aConfig.size(); ++index)
    {
        for (size_t counter = 0U; counter < aConfig[index]; ++counter)
        {
            resourceSequence.push_back(index);
        }
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(resourceSequence.begin(), resourceSequence.end(), std::default_random_engine(seed));
    return resourceSequence;
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
        lambda(pVertex->getAllPoints(), Logger::formatString(pVertex->getStringId()));
    }
    for (Edge* const pEdge : mEdges)
    {
        lambda(pEdge->getAllPoints(), Logger::formatString(pEdge->getStringId()));
    }
    for (Land* const pLand : mLands)
    {
        lambda(pLand->getAllPoints(), Logger::formatString(pLand->getStringId()));
    }
    for (Harbour* const pHarbour : mHarbours)
    {
        lambda(pHarbour->getAllPoints(), Logger::formatString(pHarbour->getStringId()));
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

int GameMap::assignResourceAndDice()
{
    SequenceConfig_t resourceConfig(static_cast<size_t>(ResourceTypes::ANY) + 1);
    // default config
    resourceConfig[ResourceTypes::DESERT] = constant::NUM_LAND_DESERT;
    resourceConfig[ResourceTypes::CLAY]   = constant::NUM_LAND_CLAY;
    resourceConfig[ResourceTypes::SHEEP]  = constant::NUM_LAND_SHEEP;
    resourceConfig[ResourceTypes::WHEAT]  = constant::NUM_LAND_WHEAT;
    resourceConfig[ResourceTypes::WOOD]   = constant::NUM_LAND_WOOD;
    resourceConfig[ResourceTypes::ORE]    = constant::NUM_LAND_ORE;
    for (Land* const pLand : mLands)
    {
        const ResourceTypes resource = pLand->getResourceType();
        if (resource != ResourceTypes::NONE)
        {
            size_t& amount = resourceConfig[resource];
            if (amount == 0)
            {
                WARN_LOG("Too much " + resourceTypesToStr(resource) + " in user defined map");
            }
            else
            {
                --amount;
            }
        }
    }
    std::vector<int> resourceSeq = randomizeResource(resourceConfig);

    SequenceConfig_t diceConfig(13); // 0 to 12
    for (size_t index = 3; index <= 11; ++index)
    {
        diceConfig[index] = constant::NUM_DICE_3_TO_11;
    }
    diceConfig[2]  = constant::NUM_DICE_2_OR_12;
    diceConfig[7]  = constant::NUM_DICE_7;
    diceConfig[12] = constant::NUM_DICE_2_OR_12;
    std::vector<int> diceSeq = randomizeResource(diceConfig);
    size_t resourceIndex = 0;
    size_t diceIndex = 0;
    for (Land* const pLand : mLands)
    {
        if (pLand->getResourceType() == ResourceTypes::NONE)
        {
            pLand->setResourceType(static_cast<ResourceTypes>(resourceSeq.at(resourceIndex++)));
        }
        if (pLand->getResourceType() != ResourceTypes::DESERT)
        {
            pLand->setDiceNum(diceSeq.at(diceIndex++));
        }
    }
    return 0;
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
    DEBUG_LOG_L2("adding port#", mHarbours.size(), " for ", aId1, " and ", aId2);
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
        ERROR_LOG("Map initialized, cannot registerTerrain for " + aTerrain->getStringId());
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
        ERROR_LOG("Map initialized, cannot registerTerrain for " + aTerrain->getStringId());
        return 1;
    }
    return registerTerrain(aPoint.x, aPoint.y, aTerrain);
}

int GameMap::registerTerrain(const int x, const int y, Terrain* const aTerrain)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot registerTerrain for " + aTerrain->getStringId());
        return 1;
    }
    if (boundaryCheck(x, y)) {
        mGameMap.at(y).at(x) = aTerrain;
        return 0;
    }
    else
    {
        ERROR_LOG("Failed to register point for " + aTerrain->getStringId(), " at (" , x, ", ", y, "), out of bound");
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
    rc |= populateMap();
    rc |= populateHarbours(true, true); // (bool aUseDefaultPosition, bool aUseDefaultResourceType)
    rc |= checkOverlap();
    rc |= assignResourceAndDice();

    if (rc != 0)
    {
        ERROR_LOG("failed to initialize GameMap");
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
    return (x < static_cast<int>(mSizeHorizontal) && y < static_cast<int>(mSizeVertical));
}

void GameMap::printMap(bool aUseId)
{
    if (!mInitialized)
    {
        WARN_LOG("Map not initialized, printMap may not function as expected");
    }
    for (size_t jj = 0; jj < mSizeVertical; ++jj)
    {
        const std::deque<Terrain*>& row = mGameMap.at(jj);
        for (size_t ii = 0; ii < mSizeHorizontal; ++ii)
        {
            // easier to debug using char c
            char c = row.at(ii)->getCharRepresentation(ii, jj, aUseId); //print ID
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

int GameMap::clearAndResize(const int aSizeHorizontal, const int aSizeVertical)
{
    mSizeHorizontal = aSizeHorizontal;
    mSizeVertical = aSizeVertical;
    mNumHarbour = constant::NUM_OF_HARBOUR;
    mInitialized = false;
    mVertices.clear();
    mEdges.clear();
    mLands.clear();
    mHarbours.clear();
    mGameMap.clear();
    // init a 2D array with (Terrain*)nullptr
    for (size_t jj = 0; jj < mSizeVertical; ++jj)
    {
        std::deque<Terrain*> row(mSizeHorizontal, nullptr);
        mGameMap.push_back(row);
    }
    return 0;
}

GameMap::GameMap(const int aSizeHorizontal, const int aSizeVertical) :
    mSizeHorizontal(aSizeHorizontal),
    mSizeVertical(aSizeVertical),
    mNumHarbour(constant::NUM_OF_HARBOUR),
    mInitialized(false)
{
    // init a 2D array with (Terrain*)nullptr
    for (size_t jj = 0; jj < mSizeVertical; ++jj)
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

SequenceConfig_t::SequenceConfig_t(const size_t aSize) : mConfig(aSize, 0)
{
    //empty
}

size_t SequenceConfig_t::size() const
{
    return mConfig.size();
}

size_t& SequenceConfig_t::operator[](const size_t aIndex)
{
    return mConfig.at(aIndex);
}

size_t& SequenceConfig_t::operator[](const ResourceTypes aIndex)
{
    return mConfig.at(static_cast<size_t>(aIndex));
}