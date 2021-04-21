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
     * Land::populateAdjacencies adds necessary edges and vertices and records adjacent vertices
     * Vertex::populateAdjacencies records connected edges and adjcent vertices
     * Edge::populateAdjacencies require Vertex to know connected edges first
     */

    // return codes
    int rcLand = 0;
    int rcEdge = 0;
    int rcVertex = 0;

    for (Land* const pLand : mLands)
    {
        rcLand |= pLand->populateAdjacencies(*this);
    }
    rcLand ?
        ERROR_LOG("Failed to populate adjacencies for Lands")
        :
        INFO_LOG("Successfully populated adjacencies of Lands");

    for (Vertex* const pVertex : mVertices)
    {
        rcVertex |= pVertex->populateAdjacencies(*this);
    }
    rcVertex ?
        ERROR_LOG("Failed to populate adjacencies for Vertices")
        :
        INFO_LOG("Successfully populated adjacencies of Vertices");

    for (Edge* const pEdge : mEdges)
    {
        rcEdge |= pEdge->populateAdjacencies(*this);
    }
    rcEdge ?
        ERROR_LOG("Failed to populate adjacencies for Edges")
        :
        INFO_LOG("Successfully populated adjacencies of Edges");

    return (rcLand | rcEdge | rcVertex);
}

int GameMap::populateHarbours(bool aUseDefaultPosition, bool aUseDefaultResourceType)
{
    int rc = 0;
    if (mLands.size() != constant::NUM_LANDS_DEFAULT)
    {
        // override to use ramdon position when not using the default map
        aUseDefaultPosition = false;
    }
    if (aUseDefaultPosition)
    {
        INFO_LOG("using default position for harbours");
        rc = createHarboursDefault();
    }
    else
    {
        INFO_LOG("using random position for harbours");
        rc = createHarboursRandom();
    }

    rc ?
        WARN_LOG("Failed to create all harbours, created: ", mHarbours.size(), ", expected: ", mNumHarbour)
        :
        INFO_LOG("Successfully created all harbours");

    SequenceConfig_t config(static_cast<size_t>(ResourceTypes::ANY) + 1);
    config[ResourceTypes::CLAY]  = constant::NUM_HARBOUR_CLAY;
    config[ResourceTypes::SHEEP] = constant::NUM_HARBOUR_SHEEP;
    config[ResourceTypes::WHEAT] = constant::NUM_HARBOUR_WHEAT;
    config[ResourceTypes::WOOD]  = constant::NUM_HARBOUR_WOOD;
    config[ResourceTypes::ORE]   = constant::NUM_HARBOUR_ORE;
    if (aUseDefaultResourceType && mHarbours.size() == constant::NUM_OF_HARBOUR)
    {
        DEBUG_LOG_L3("Using 9 default harbour types");
        config[ResourceTypes::ANY] = 0; // ANY is determined by (index % 2 != 0)
        std::vector<int> randomSeq = randomizeResource(config);
        DEBUG_LOG_L2("Assigning randomSeq ", randomSeq, " to harbours");
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

    rc ?
        WARN_LOG("Failed to populateHarbours")
        :
        INFO_LOG("Successfully populated all harbours");
    return rc;
}

int GameMap::createHarboursDefault()
{
    std::vector<int> harbourCandidates;
    const Vertex* pVertex = nullptr;
    for (Vertex* pVertexTemp : mVertices)
    {
        // reset harbours
        pVertexTemp->setHarbour(nullptr);
        // find a coastal vertex as the starting point
        if ((pVertex == nullptr) && (pVertexTemp->isCoastal()))
        {
            harbourCandidates.push_back(pVertexTemp->getId());
            pVertex = pVertexTemp;
        }
    }
    if (!pVertex)
    {
        ERROR_LOG("Unable to find a string point");
        return 1;
    }
    const Vertex* pPreviousVertex = pVertex;
    do
    {
        for (const Vertex* const pNextVertex : pVertex->getAdjacentVertices())
        {
            if (pNextVertex == pPreviousVertex)
            {
                continue;
            }
            if (pNextVertex->isCoastal())
            {
                harbourCandidates.push_back(pNextVertex->getId());
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

    return (mHarbours.size() != mNumHarbour);
}

int GameMap::createHarboursRandom()
{
    if (mHarbours.size() >= mNumHarbour)
    {
        mNumHarbour = mHarbours.size();
        return 0;
    }
    std::vector<int> harbourCandidates;
    for (Vertex* const pVertex : mVertices)
    {
        if (pVertex->isCoastal() && !pVertex->hasHarbour())
        {
            harbourCandidates.push_back(pVertex->getId());
        }
    }
    while (mHarbours.size() < mNumHarbour)
    {
        if (harbourCandidates.size() == 0U)
        {
            WARN_LOG("Unable to create ", mNumHarbour, " of harbours, current number of harbour ", mHarbours.size());
            break;
        }
        // get a vertex from candidates
        std::uniform_int_distribution<size_t> distribution(0U, harbourCandidates.size() - 1U);
        size_t index = distribution(mEngine);
        int idVertex = harbourCandidates[index];
        if (mVertices[idVertex]->hasHarbour())
        {
            continue;
        }
        int idOtherVertex = -1;
        for (const Vertex* const pVertex : mVertices[idVertex]->getAdjacentVertices())
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
    std::shuffle(resourceSequence.begin(), resourceSequence.end(), mEngine);
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
                    WARN_LOG("Overlap detected at ", point, \
                         " Collision IDs: ", aId, ", ", points.at(point));
                }
                else
                {
                    points.emplace(point, aId);
                }
            }
        };

    for (Vertex* const pVertex : mVertices)
    {
        lambda(pVertex->getAllPoints(), pVertex->getStringId());
    }
    for (Edge* const pEdge : mEdges)
    {
        lambda(pEdge->getAllPoints(), pEdge->getStringId());
    }
    for (Land* const pLand : mLands)
    {
        lambda(pLand->getAllPoints(), pLand->getStringId());
    }
    for (Harbour* const pHarbour : mHarbours)
    {
        lambda(pHarbour->getAllPoints(), pHarbour->getStringId());
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
    SequenceConfig_t resourceConfig(static_cast<size_t>(ResourceTypes::ANY));
    // default config
    resourceConfig[ResourceTypes::CLAY]   = constant::NUM_LAND_CLAY;
    resourceConfig[ResourceTypes::SHEEP]  = constant::NUM_LAND_SHEEP;
    resourceConfig[ResourceTypes::WHEAT]  = constant::NUM_LAND_WHEAT;
    resourceConfig[ResourceTypes::WOOD]   = constant::NUM_LAND_WOOD;
    resourceConfig[ResourceTypes::ORE]    = constant::NUM_LAND_ORE;
    resourceConfig[ResourceTypes::DESERT] = constant::NUM_LAND_DESERT;
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
    if (resourceConfig.sum() < mLands.size())
    {
        // TODO: prompt for user input?
        WARN_LOG("Non-default map, extra Desert will be added, amount: ", mLands.size() - resourceConfig.sum());
        resourceConfig[ResourceTypes::DESERT] += mLands.size() - resourceConfig.sum();
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
    const size_t numOfDesert = resourceConfig[ResourceTypes::DESERT];
    if (diceConfig.sum() < mLands.size() - numOfDesert)
    {
        // TODO: prompt for user input?
        WARN_LOG("Non-default map, extra 10 will be added, amount: ", mLands.size() - numOfDesert - diceConfig.sum());
        diceConfig[10] += mLands.size() - numOfDesert - diceConfig.sum();
    }
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
        else if (mRobLandId == -1)
        {
            // robber initially is at desert
            mRobLandId = pLand->getId();
            pLand->rob(true);
        }
    }
    return 0;
}

const Terrain* GameMap::getTerrain(const int x, const int y) const
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

const Terrain* GameMap::getTerrain(const Point_t& aPoint) const
{
    return getTerrain(aPoint.x, aPoint.y);
}

const Vertex* GameMap::addVertex(const size_t aTopLeftX, const size_t aTopLeftY)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot add vertex");
        return nullptr;
    }
    Vertex* const pVertex = new Vertex(mVertices.size(), Point_t{aTopLeftX, aTopLeftY});
    pVertex->registerToMap(*this);
    mVertices.push_back(pVertex);
    return pVertex;
}

const Edge* GameMap::addEdge(const size_t aTopLeftX, const size_t aTopLeftY, const char aPattern)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot add edge");
        return nullptr;
    }
    Edge* const pEdge = new Edge(mEdges.size(), Point_t{aTopLeftX, aTopLeftY}, aPattern);
    pEdge->registerToMap(*this);
    mEdges.push_back(pEdge);
    return pEdge;

}

const Land* GameMap::addLand(const size_t aTopLeftX, const size_t aTopLeftY, const ResourceTypes aResource)
{
    if (mInitialized)
    {
        ERROR_LOG("Map initialized, cannot add land");
        return nullptr;
    }
    Land* const pLand = new Land(mLands.size(), Point_t{aTopLeftX, aTopLeftY}, aResource);
    pLand->registerToMap(*this);
    mLands.push_back(pLand);
    return pLand;
}

Harbour* GameMap::addHarbour(const int aId1, const int aId2)
{
    Harbour* const pHarbour = new Harbour(mHarbours.size(), ResourceTypes::NONE, \
                                        mVertices[aId1]->getTopLeft(), mVertices[aId2]->getTopLeft());
    mVertices[aId1]->setHarbour(pHarbour);
    mVertices[aId2]->setHarbour(pHarbour);

    mHarbours.push_back(pHarbour);
    DEBUG_LOG_L2("added harbour#", mHarbours.size(), " for ", aId1, " and ", aId2);
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

int GameMap::setTerrainColor(const int x, const int y, ColorPairIndex aColorIndex)
{
    if (!boundaryCheck(x, y))
    {
        WARN_LOG("SetColor called for an out-of-bound Point{", x, ", ", y, '}');
        return 1;
    }
    DEBUG_LOG_L3("setting color#", (int)aColorIndex, " for Point{", x, ", ", y, '}');
    mGameMap.at(y).at(x)->setColor(aColorIndex);
    return 0;

}

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

void GameMap::logMap(bool aUseId)
{
    if (!mInitialized)
    {
        WARN_LOG("Map not initialized, printMap may not function as expected");
    }
    std::string map = "\n========================\n|";
    for (size_t jj = 0; jj < mSizeVertical; ++jj)
    {
        const std::deque<Terrain*>& row = mGameMap.at(jj);
        for (size_t ii = 0; ii < mSizeHorizontal; ++ii)
        {
            // easier to debug using an extra char c
            char character = row.at(ii)->getCharRepresentation(ii, jj, aUseId); //print ID
            map += character;
        }
        map += "|\n|";
    }
    map += "========================";
    INFO_LOG("The map is", map);
}

const std::deque< std::deque<Terrain*> >& GameMap::getTerrainMap() const
{
    if (!mInitialized)
    {
        ERROR_LOG("Map not initialized, cannot printMap");
    }
    return mGameMap;
}

int GameMap::clearAndResize(const int aSizeHorizontal, const int aSizeVertical)
{
    mSizeHorizontal = aSizeHorizontal;
    mSizeVertical = aSizeVertical;
    mNumHarbour = constant::NUM_OF_HARBOUR;
    mRobLandId = -1;
    mInitialized = false;
    mVertices.clear();
    mEdges.clear();
    mLands.clear();
    mHarbours.clear();
    mGameMap.clear();
    // init a 2D array with (Terrain*)nullptr
    for (size_t jj = 0; jj < mSizeVertical; ++jj)
    {
        std::deque<Terrain*> row(mSizeHorizontal, Blank::getBlank());
        mGameMap.push_back(row);
    }
    return 0;
}

int GameMap::getSizeHorizontal() const
{
    return mSizeHorizontal;
}
int GameMap::getSizeVertical() const
{
    return mSizeVertical;
}

GameMap::GameMap(const int aSizeHorizontal, const int aSizeVertical) :
    mSeed(std::chrono::system_clock::now().time_since_epoch().count()),
    mEngine(mSeed)
{
    INFO_LOG("random engine seed: ", mSeed);
    clearAndResize(aSizeHorizontal, aSizeVertical);
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
