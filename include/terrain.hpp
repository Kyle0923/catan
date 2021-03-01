#ifndef INCLUDE_TERRAIN_HPP
#define INCLUDE_TERRAIN_HPP

#include <vector>
#include "common.hpp"
#include "curses.h"

class Terrain
{
protected:
    const int mId;
    const Point_t mTopRight;
public:
    virtual std::vector<Point_t> getAllPoints();
    Point_t getTopRight();
    Terrain(int aId, Point_t aTopRight);
};

#endif /* INCLUDE_TERRAIN_HPP */
