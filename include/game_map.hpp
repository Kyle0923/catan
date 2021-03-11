#ifndef INCLUDE_GAME_MAP_HPP
#define INCLUDE_GAME_MAP_HPP

#include <vector>
#include <deque>
#include "common.hpp"

class Terrain; // forward declare Terrain

class GameMap
{
private:
    int mSizeHorizontal;
    int mSizeVertical;
    std::deque< std::deque<Terrain*> > mGameMap;
    inline bool boundaryCheck(int x, int y);
public:
    GameMap(int aSizeHorizontal, int aSizeVertical);
    int registerTerrain(const std::vector<Point_t>& aPoints, Terrain* const aTerrain);
    int registerTerrain(const Point_t& aPoint, Terrain* const aTerrain);
    int registerTerrain(const int x, const int y, Terrain* const aTerrain);

    void fillInBlank();
    ~GameMap();
};


#endif /* INCLUDE_GAME_MAP_HPP */
