#ifndef INCLUDE_TERRAIN_HPP
#define INCLUDE_TERRAIN_HPP

#include <vector>
#include "common.hpp"
#include "curses.h"
#include "game_map.hpp"

class Terrain
{
protected:
    const int mId;
    const Point_t mTopRight;
public:
    // return the top-right point (top first)
    Point_t getTopRight();
    Terrain(int aId, Point_t aTopRight);

    // Register all points of current terrain to map
    void registerToMap(GameMap* aMap);

    virtual std::vector<Point_t> getAllPoints();

    /* Return all terrains directly connected to
     * current terrain (edge, vertex, land) */
    virtual std::vector<Terrain*> getNeighbour(){return std::vector<Terrain*>();}; // TODO: implement
};

#endif /* INCLUDE_TERRAIN_HPP */
