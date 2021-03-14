#ifndef INCLUDE_GAME_MAP_HPP
#define INCLUDE_GAME_MAP_HPP

#include <vector>
#include <deque>
#include "common.hpp"
#include "terrain.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "land.hpp"

class GameMap
{
private:
    int mSizeHorizontal;
    int mSizeVertical;
    std::deque< std::deque<Terrain*> > mGameMap;

    std::vector<Vertex*> mVertices;
    std::vector<Edge*> mEdges;
    std::vector<Land*> mLands;

    inline bool boundaryCheck(int x, int y);
    void fillInBlank();
    int populateAdjacency();
    int checkOverlap() const;

public:
    GameMap(int aSizeHorizontal, int aSizeVertical);
    Terrain* const getTerrain(const int x, const int y);

    int addVertex(const size_t aTopRightX, const size_t aTopRightY);
    int addEdge(const size_t aTopRightX, const size_t aTopRightY, const char aPattern);
    int addLand(const size_t aTopRightX, const size_t aTopRightY, const ResourceTypes aResource);

    int registerTerrain(const std::vector<Point_t>& aPoints, Terrain* const aTerrain);
    int registerTerrain(const Point_t& aPoint, Terrain* const aTerrain);
    int registerTerrain(const int x, const int y, Terrain* const aTerrain);

    int initMap();
    void printMap(bool aUseId = false);
    ~GameMap();
};


#endif /* INCLUDE_GAME_MAP_HPP */
