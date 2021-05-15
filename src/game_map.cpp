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
#include <numeric>
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
            resourceSequence.emplace_back(index);
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

Terrain* GameMap::_getTerrain(const int x, const int y) const
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

Terrain* GameMap::_getTerrain(const Point_t& aPoint) const
{
    return _getTerrain(aPoint.x, aPoint.y);
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
    _getTerrain(x, y)->setColor(aColorIndex);
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
    mCurrentPlayer = 0;
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
    for (Player* pPlayer : mPlayers)
    {
        delete pPlayer;
    }
}

int GameMap::addPlayer(size_t aNumOfPlayer)
{
    for (size_t ii = 0; ii < aNumOfPlayer; ++ii)
    {
        mPlayers.push_back(new Player(ii));
    }
    return mPlayers.size();
}

size_t GameMap::nextPlayer()
{
    if (mPlayers.size() == 0)
    {
        ERROR_LOG("No player");
    }
    mCurrentPlayer = (mCurrentPlayer + 1) % mPlayers.size();
    return mCurrentPlayer;
}

size_t GameMap::currentPlayer() const
{
    return mCurrentPlayer;
}

bool GameMap::currentPlayerHasResourceForRoad() const
{
    // 1 clay, 1 wood
    return mPlayers[mCurrentPlayer]->hasResources({
        {ResourceTypes::CLAY, 1},
        {ResourceTypes::WOOD, 1}
    });
}

bool GameMap::currentPlayerHasResourceForSettlement() const
{
    // 1 clay, 1 wood, 1 wheat, 1 sheep
    return mPlayers[mCurrentPlayer]->hasResources({
        {ResourceTypes::CLAY, 1},
        {ResourceTypes::WOOD, 1},
        {ResourceTypes::WHEAT, 1},
        {ResourceTypes::SHEEP, 1}
    });
}

bool GameMap::currentPlayerHasResourceForCity() const
{
    // 2 wheat, 3 ore
    return mPlayers[mCurrentPlayer]->hasResources({
        {ResourceTypes::WHEAT, 2},
        {ResourceTypes::ORE, 3}
    });
}

bool GameMap::currentPlayerHasResourceForDevCard() const
{
    // 1 sheep, 1 wheat, 1 ore
    return mPlayers[mCurrentPlayer]->hasResources({
        {ResourceTypes::SHEEP, 1},
        {ResourceTypes::WHEAT, 1},
        {ResourceTypes::ORE, 1}
    });
}

int GameMap::currentPlayerBuyDevCard(DevelopmentCardTypes& aDevCard)
{
    if (!currentPlayerHasResourceForDevCard())
    {
        return 1;
    }
    // TODO: draw dev card from devCardPile
    static std::uniform_int_distribution<int> distribution(
        static_cast<int>(DevelopmentCardTypes::KNIGHT), static_cast<int>(DevelopmentCardTypes::ONE_VICTORY_POINT));
    aDevCard = static_cast<DevelopmentCardTypes>(distribution(mEngine));
    Player* & currPlayer = mPlayers[mCurrentPlayer];
    currPlayer->drawDevelopmentCard(aDevCard, 1);
    currPlayer->consumeResources(ResourceTypes::SHEEP, 1);
    currPlayer->consumeResources(ResourceTypes::WHEAT, 1);
    currPlayer->consumeResources(ResourceTypes::ORE, 1);
    return 0;
}

int GameMap::currentPlayerConsumeDevCard(const DevelopmentCardTypes aDevCard)
{
    return mPlayers[mCurrentPlayer]->consumeDevelopmentCard(aDevCard);
}

void GameMap::summarizePlayerStatus(int aPlayerId, std::vector<std::string>& aReturnMsg) const
{
    if (aPlayerId == -1)
    {
        aPlayerId = mCurrentPlayer;
    }
    if (aPlayerId < 0 || aPlayerId >= static_cast<int>(mPlayers.size()))
    {
        WARN_LOG("in summarizePlayerStatus() playerId is out-of-range");
        aReturnMsg.emplace_back("incorrect player ID");
        return;
    }
    const Player* const player = mPlayers.at(aPlayerId);
    auto playerResources = player->getResources();
    auto playerDevCard = player->getDevCards();
    auto playerDevCardUsed = player->getUsedDevCards();
    const size_t victoryPoint = player->getVictoryPoint(aPlayerId == static_cast<int>(mCurrentPlayer));

    aReturnMsg.emplace_back("Status of Player#" + std::to_string(aPlayerId));
    aReturnMsg.emplace_back("");
    if (aPlayerId == static_cast<int>(mCurrentPlayer))
    {
        // current player, show all information
        aReturnMsg.emplace_back("Resources: ");
        aReturnMsg.emplace_back("  " + summarizeEnumArray(playerResources, resourceTypesToStr));
        aReturnMsg.emplace_back("");

        aReturnMsg.emplace_back("Development card: ");
        aReturnMsg.emplace_back("  " + summarizeEnumArray(playerDevCard, developmentCardTypesToStr));
        aReturnMsg.emplace_back("");

        aReturnMsg.emplace_back("Used development card: ");
        aReturnMsg.emplace_back("  " + summarizeEnumArray(playerDevCardUsed, developmentCardTypesToStr));
        aReturnMsg.emplace_back("");

    }
    else
    {
        // current player, only show public information
        aReturnMsg.emplace_back("Number of Resources: " + \
                std::to_string(std::accumulate(playerResources.begin(), playerResources.end(), 0U)));
        aReturnMsg.emplace_back("Number of Development card: " + \
                std::to_string(std::accumulate(playerDevCard.begin(), playerDevCard.end(), 0U)));
        aReturnMsg.emplace_back("Number of Used development card: " + \
                std::to_string(std::accumulate(playerDevCardUsed.begin(), playerDevCardUsed.end(), 0U)));
    }
    aReturnMsg.emplace_back(Logger::formatString("Victory point: ", victoryPoint));
}

int GameMap::buildColony(const Point_t aPoint, const ColonyType aColony)
{
    if (!boundaryCheck(aPoint.x, aPoint.y))
    {
        WARN_LOG("buildColony called for an out-of-bound ", aPoint);
        return 1;
    }
    if (aColony == ColonyType::NONE)
    {
        WARN_LOG("buildColony called with ColonyType::NONE");
        return 1;
    }
    Vertex* const pVertex = dynamic_cast<Vertex*>(_getTerrain(aPoint));
    if (!pVertex)
    {
        WARN_LOG("Expected vertex at ", aPoint, ", actual: " + _getTerrain(aPoint)->getStringId());
        return 1;
    }

    int vertexOwner = pVertex->getOwner();
    if ((aColony == ColonyType::SETTLEMENT && vertexOwner != -1) || \
        (aColony == ColonyType::CITY && vertexOwner != static_cast<int>(mCurrentPlayer)))
    {
        WARN_LOG(pVertex->getStringId() + " is owned by Player#", vertexOwner, " already, cannot reset for Player#", mCurrentPlayer);
        return 2;
    }

    if (aColony == ColonyType::SETTLEMENT)
    {
        if (!pVertex->isAvailable())
        {
            WARN_LOG("One or more adjacent vertex of " + pVertex->getStringId() + " is owned, cannot build settlement here");
            return 3;
        }

        if (!currentPlayerHasResourceForSettlement())
        {
            return 4;
        }
        mPlayers[mCurrentPlayer]->consumeResources(ResourceTypes::CLAY, 1);
        mPlayers[mCurrentPlayer]->consumeResources(ResourceTypes::WOOD, 1);
        mPlayers[mCurrentPlayer]->consumeResources(ResourceTypes::WHEAT, 1);
        mPlayers[mCurrentPlayer]->consumeResources(ResourceTypes::SHEEP, 1);

        mPlayers[mCurrentPlayer]->addColony(*pVertex);
    }
    else
    {
        if (!currentPlayerHasResourceForCity())
        {
            return 4;
        }
        mPlayers[mCurrentPlayer]->consumeResources(ResourceTypes::WHEAT, 2);
        mPlayers[mCurrentPlayer]->consumeResources(ResourceTypes::ORE, 3);
    }

    return pVertex->setOwner(mCurrentPlayer, aColony);
}

int GameMap::buildRoad(const Point_t aPoint)
{
    if (!boundaryCheck(aPoint.x, aPoint.y))
    {
        WARN_LOG("buildRoad called for an out-of-bound ", aPoint);
        return 1;
    }
    Edge* const pEdge = dynamic_cast<Edge*>(_getTerrain(aPoint));
    if (!pEdge)
    {
        WARN_LOG("Expected edge at ", aPoint, ", actual: " + _getTerrain(aPoint)->getStringId());
        return 1;
    }
    if (pEdge->getOwner() != -1)
    {
        WARN_LOG(pEdge->getStringId() + " is owned by Player#", pEdge->getOwner(), " already, cannot reset for Player#", mCurrentPlayer);
        return 2;
    }

    if (!pEdge->isAvailable(mCurrentPlayer))
    {
        WARN_LOG("None of the adjacent vertices of " + pEdge->getStringId() + " is owned by current player, cannot build road here");
        return 3;
    }

    if (!currentPlayerHasResourceForRoad())
    {
        return 4;
    }

    mPlayers[mCurrentPlayer]->consumeResources(ResourceTypes::CLAY, 1);
    mPlayers[mCurrentPlayer]->consumeResources(ResourceTypes::WOOD, 1);

    mPlayers[mCurrentPlayer]->addRoad(*pEdge);

    return pEdge->setOwner(mCurrentPlayer);
}

int GameMap::moveRobber(const Point_t aDestination)
{
    if (!isTerrain<Land>(aDestination))
    {
        return 1;
    }
    int landId = _getTerrain(aDestination)->getId();
    mLands.at(mRobLandId)->rob(false);
    mLands.at(landId)->rob(true);
    mRobLandId = landId;
    return 0;
}

int GameMap::robVertex(const Point_t aVertex, ResourceTypes& aRobResource)
{
    Vertex* const pVertex = dynamic_cast<Vertex*>(_getTerrain(aVertex));
    if (!pVertex)
    {
        return 1;
    }
    int owner = pVertex->getOwner();
    if (owner == -1)
    {
        return 2;
    }
    auto resource = mPlayers.at(owner)->getResources();
    SequenceConfig_t seqConfig(resource);
    std::vector<int> randomResource = randomizeResource(seqConfig);
    if (randomResource.size() == 0)
    {
        return 3;
    }
    aRobResource = static_cast<ResourceTypes>(randomResource.front());
    mPlayers.at(owner)->consumeResources(aRobResource, 1U);
    mPlayers.at(mCurrentPlayer)->addResources(aRobResource, 1U);
    return 0;
}

int GameMap::rollDice()
{
    static std::uniform_int_distribution<int> distribution(1,6);
    int dice = distribution(mEngine) + distribution(mEngine);
    INFO_LOG("Player#", mCurrentPlayer, " rolled: ", dice);
    for (Land* const pLand : mLands)
    {
        if (pLand->getDiceNum() == dice && !pLand->isUnderRobber())
        {
            for (const Vertex* const pConstVertex : pLand->getAdjacentVertices())
            {
                const int playerId = pConstVertex->getOwner();
                if (playerId != -1)
                {
                    // vertex is owned by Player
                    const ResourceTypes resource = pLand->getResourceType();
                    const size_t numOfResource = static_cast<size_t>(pConstVertex->getColonyType());

                    DEBUG_LOG_L3("Adding ", numOfResource, " " + resourceTypesToStr(resource) + " to Player#", playerId);
                    mPlayers.at(playerId)->addResources(resource, numOfResource);
                }
            }
        }
    }
    return dice;
}
