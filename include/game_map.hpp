/**
 * Project: catan
 * @file game_map.hpp
 * @brief 2D array, represents the game map
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_GAME_MAP_HPP
#define INCLUDE_GAME_MAP_HPP

#include <vector>
#include <deque>
#include <random>
#include "common.hpp"
#include "sequence_config.hpp"
#include "terrain.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "land.hpp"
#include "harbour.hpp"
#include "player.hpp"
#include "user_interface.hpp"

struct SequenceConfig_t;

class GameMap
{
private:
    size_t mSizeHorizontal;
    size_t mSizeVertical;
    size_t mNumHarbour;
    int mRobLandId;
    bool mInitialized;

    // random generator related
    const unsigned mSeed;
    std::default_random_engine mEngine;

    std::deque< std::deque<Terrain*> > mGameMap;

    std::vector<Vertex*> mVertices;
    std::vector<Edge*> mEdges;
    std::vector<Land*> mLands;
    std::vector<Harbour*> mHarbours;
    std::vector<Player*> mPlayers;

    Harbour* addHarbour(const int aId1, const int aId2);

    inline bool boundaryCheck(const int x, const int y) const;
    int populateMap();
    int assignResourceAndDice(); // assign resources and dice number to lands
    int checkOverlap() const;
    [[deprecated]] void fillInBlank();  // no longer used, kept here for reference

    /*
     * aUseDefaultPosition - randomize the position of the harbours, though the harbours tend to not evenly distributed
     *                       useful when using user defined map
     * aUseDefaultResourceType - by default, 1 ANY harbour is placed between 2 Resource harbours (where possible)
     *                           set this to false to lift that restriction
     */
    int populateHarbours(bool aUseDefaultPosition, bool aUseDefaultResourceType);
    int createHarboursDefault();
    int createHarboursRandom();

    /* SequenceConfig_t is an array whose value represents the num of occurrences of the index in the output
     * e.g., aConfig[CLAY] = 2 means, there should be 2 CLAY in the returned vector
     */
    std::vector<int> randomizeResource(SequenceConfig_t aConfig);

public:
    GameMap(const int aSizeHorizontal = 0, const int aSizeVertical = 0);

    int clearAndResize(const int aSizeHorizontal, const int aSizeVertical);

    int getSizeHorizontal() const;
    int getSizeVertical() const;

    const Terrain* getTerrain(const int x, const int y) const;
    const Terrain* getTerrain(const Point_t& aPoint) const;

    template<typename T>
    static inline bool isTerrain(const Terrain* const aTerrain)
    {
        return dynamic_cast<const T* const>(aTerrain);
    };

    const Vertex* addVertex(const size_t aTopLeftX, const size_t aTopLeftY);
    const Edge* addEdge(const size_t aTopLeftX, const size_t aTopLeftY, const char aPattern);
    const Land* addLand(const size_t aTopLeftX, const size_t aTopLeftY, const ResourceTypes aResource);

    int setTerrainColor(const int x, const int y, ColorPairIndex aColorIndex);

    // /* add a vertex to be a condidate of harbour
    //  * a condidate must be coastal
    //  * a pair of connected candidates is considered a valid habour and will be assign a harbour
    //  * for a dangling condidate, GameMap will try to find a adjacent vertex to form a pair
    //  * for three connected candidates, the rightmost / bottommost will be treated as dangling
    //  */
    // int addHarbourCandidate(Vertex* const aVertex); //TODO: not supported currently

    int registerTerrain(const std::vector<Point_t>& aPoints, Terrain* const aTerrain);
    int registerTerrain(const Point_t& aPoint, Terrain* const aTerrain);
    int registerTerrain(const int x, const int y, Terrain* const aTerrain);

    void setNumOfHarbour(const size_t aNum);

    int initMap();
    void logMap(bool aUseId = false);  // std::cout implementation, convenient in development
    const std::deque< std::deque<Terrain*> >& getTerrainMap() const;

    // Player related
    int addPlayer(size_t aNumOfPlayer);

    /**
     * return 0: ok
     * return > 0 : incorrect coordinate
     * return < 0 : error with Vertex/Edge internal handling
     */
    int buildColony(const int aPlayerId, const int aVertexX, const int aVertexY, const ColonyType aColony);
    int buildRoad(const int aPlayerId, const int aEdgeX, const int aEdgeY);

    GameMap(const GameMap &) = delete;
    GameMap& operator=(const GameMap&) = delete;
    ~GameMap();
};

#endif /* INCLUDE_GAME_MAP_HPP */
