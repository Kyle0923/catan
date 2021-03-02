#ifndef INCLUDE_GAME_MAP_HPP
#define INCLUDE_GAME_MAP_HPP

#include <vector>
#include "terrain.hpp"

class GameMap
{
private:
    int mSizeHorizontal;
    int mSizeVertical;
    std::vector< std::vector<Terrain*> > mGameMap;
    inline bool boundaryCheck(int x, int y);
public:
    GameMap(int aSizeHorizontal, int aSizeVertical);
    int registerTerrain(int x, int y, Terrain* aTerrain);
    void fillInBlank();
    ~GameMap();
};


#endif /* INCLUDE_GAME_MAP_HPP */
