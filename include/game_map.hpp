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
#include "common.hpp"
#include "terrain.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "land.hpp"
#include "harbour.hpp"

struct SequenceConfig_t;

class GameMap
{
private:
    size_t mSizeHorizontal;
    size_t mSizeVertical;
    size_t mNumHarbour;
    int mRobLandId;
    bool mInitialized;
    std::deque< std::deque<Terrain*> > mGameMap;

    std::vector<Vertex*> mVertices;
    std::vector<Edge*> mEdges;
    std::vector<Land*> mLands;
    std::vector<Harbour*> mHarbours;

    Harbour* addHarbour(const int aId1, const int aId2);

    inline bool boundaryCheck(const int x, const int y) const;
    int populateMap();
    int assignResourceAndDice(); // assign resources and dice number to lands
    int checkOverlap() const;
    [[deprecated]] void fillInBlank();  // no longer needed, kept here for reference

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

    const Terrain* getTerrain(const int x, const int y);
    const Terrain* getTerrain(const Point_t& aPoint);

    template<typename T>
    static inline bool isTerrain(const Terrain* const aTerrain)
    {
        return dynamic_cast<const T* const>(aTerrain);
    };

    const Vertex* addVertex(const size_t aTopLeftX, const size_t aTopLeftY);
    const Edge* addEdge(const size_t aTopLeftX, const size_t aTopLeftY, const char aPattern);
    const Land* addLand(const size_t aTopLeftX, const size_t aTopLeftY, const ResourceTypes aResource);

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

    GameMap(const GameMap &) = delete;
    GameMap& operator=(const GameMap&) = delete;
    ~GameMap();
};

struct SequenceConfig_t
{
    std::vector<size_t> mConfig;
    SequenceConfig_t(const size_t aSize);
    size_t size() const;
    size_t& operator[](const size_t aIndex);
    size_t& operator[](const ResourceTypes aIndex);
    size_t sum() const
    {
        size_t sum = 0;
        for (size_t num : mConfig)
        {
            sum += num;
        }
        return sum;
    }
};

#endif /* INCLUDE_GAME_MAP_HPP */
