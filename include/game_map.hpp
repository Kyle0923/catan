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

class GameMap
{
private:
    size_t mSizeHorizontal;
    size_t mSizeVertical;
    size_t mNumHarbour;
    bool mInitialized;
    std::deque< std::deque<Terrain*> > mGameMap;

    std::vector<Vertex*> mVertices;
    std::vector<Edge*> mEdges;
    std::vector<Land*> mLands;
    std::vector<Harbour*> mHarbours;

    Harbour* addHarbour(const int aId1, const int aId2);

    inline bool boundaryCheck(const int x, const int y) const;
    void fillInBlank();
    int populateMap();
    int checkOverlap() const;

    // harbour related
    int populateHarbours(bool aUseDefaultPosition, bool aUseDefaultResourceType);
    int createHarboursDefault();
    int createHarboursRandom();

public:
    GameMap(const int aSizeHorizontal = 0, const int aSizeVertical = 0);

    int clearAndResize(const int aSizeHorizontal, const int aSizeVertical);

    Terrain* getTerrain(const int x, const int y);
    Terrain* getTerrain(const Point_t& aPoint);

    Vertex* addVertex(const size_t aTopRightX, const size_t aTopRightY);
    Edge* addEdge(const size_t aTopRightX, const size_t aTopRightY, const char aPattern);
    Land* addLand(const size_t aTopRightX, const size_t aTopRightY, const ResourceTypes aResource);

    // /* add a vertex to be a condidate of harbour
    //  * a condidate must be coastal
    //  * a pair of connected candidates is considered a valid habour and will be assign a harbour
    //  * for a dangling condidate, GameMap will try to find a neighbour to form a pair
    //  * for three connected candidates, the rightmost / bottommost will be treated as dangling
    //  */
    // int addHarbourCandidate(Vertex* const aVertex); //TODO: not supported currently, description may not be accurate

    int registerTerrain(const std::vector<Point_t>& aPoints, Terrain* const aTerrain);
    int registerTerrain(const Point_t& aPoint, Terrain* const aTerrain);
    int registerTerrain(const int x, const int y, Terrain* const aTerrain);

    // standard map has 9 harbours
    void setNumOfHarbour(const size_t aNum = 9);

    int initMap();
    void printMap(bool aUseId = false);
    ~GameMap();
};


#endif /* INCLUDE_GAME_MAP_HPP */
