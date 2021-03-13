#include "game_map.hpp"
#include "blank.hpp"

void GameMap::fillInBlank()
{
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        std::deque<Terrain*>& row = mGameMap.at(jj);
        for (int ii = 0; ii < mSizeHorizontal; ++ii)
        {
            if (row.at(ii) == nullptr)
            {
                row.at(ii) = Blank::getBlank();
            }
        }
    }
}

Terrain* const GameMap::getTerrain(const int x, const int y)
{
    if (!boundaryCheck(x,y))
    {
        Logger::warn("Coord (", x, ", ", y, ") is out of bound, need to resize first");
        return nullptr;
    }
    else
    {
        return mGameMap.at(y).at(x);
    }
}

int GameMap::registerTerrain(const std::vector<Point_t>& aPoints, Terrain* const aTerrain)
{
    int rc = 0;
    for (std::vector<Point_t>::const_iterator it = aPoints.begin() ; it != aPoints.end(); ++it)
    {
        rc += registerTerrain(*it, aTerrain);
    }
    return rc;
}

int GameMap::registerTerrain(const Point_t& aPoint, Terrain* const aTerrain)
{
    return registerTerrain(aPoint.x, aPoint.y, aTerrain);
}

int GameMap::registerTerrain(const int x, const int y, Terrain* const aTerrain)
{
    if (boundaryCheck(x, y)) {
        mGameMap.at(y).at(x) = aTerrain;
        return 0;
    }
    else
    {
        Logger::warn("Coord (", x, ", ", y, ") is out of bound");
        return -1;
    }

}

int GameMap::initMap()
{
    fillInBlank();
    return 0;
}

bool GameMap::boundaryCheck(int x, int y)
{
    return (x < mSizeHorizontal && y < mSizeVertical);
}

void GameMap::printMap()
{
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        const std::deque<Terrain*>& row = mGameMap.at(jj);
        for (int ii = 0; ii < mSizeHorizontal; ++ii)
        {
            // easier to debug using char c
            // char c = row.at(ii)->getCharRepresentation(true); //print ID
            char c = row.at(ii)->getCharRepresentation();
            std::cout << c;
        }
        std::cout << std::endl;
    }
}

GameMap::GameMap(int aSizeHorizontal, int aSizeVertical) :
    mSizeHorizontal(aSizeHorizontal),
    mSizeVertical(aSizeVertical)
{
    // init a 2D array with (Terrain*)nullptr
    for (int jj = 0; jj < mSizeVertical; ++jj)
    {
        std::deque<Terrain*> row(mSizeHorizontal, nullptr);
        mGameMap.push_back(row);
    }
}

GameMap::~GameMap()
{
}
