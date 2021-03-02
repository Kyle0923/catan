#include "game_map.hpp"

void GameMap::fillInBlank()
{
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        std::vector<Terrain*> row = mGameMap[jj];
        for (int ii = 0; ii < mSizeHorizontal; ++ii)
        {
            if (row[ii] != nullptr)
            {
                continue;
            }
            Terrain* blank = new Terrain(0, Point_t{ii, jj});
            row[ii] = blank;
        }
    }
}

int GameMap::registerTerrain(int x, int y, Terrain* aTerrain)
{
    if (boundaryCheck(x, y)) {
        mGameMap[y][x] = aTerrain;
        return 0;
    }
    else
    {
        Logger::warn("Coord (", x, ", ", y, ") is out of bound");
        return -1;
    }

}

bool GameMap::boundaryCheck(int x, int y)
{
    return (x < mSizeHorizontal && y < mSizeVertical);
}

GameMap::GameMap(int aSizeHorizontal, int aSizeVertical) :
    mSizeHorizontal(aSizeHorizontal),
    mSizeVertical(aSizeVertical)
{
    // init a 2D array with (Terrain*)nullptr
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        std::vector<Terrain*> row(mSizeHorizontal, nullptr);
        mGameMap.push_back(row);
    }
}

GameMap::~GameMap()
{
}
