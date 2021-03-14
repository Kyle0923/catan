#ifndef INCLUDE_TERRAIN_HPP
#define INCLUDE_TERRAIN_HPP

#include <vector>
#include "common.hpp"
#include "curses.h"

class GameMap;

class Terrain
{
protected:
    const int mId;
    Point_t mTopRight;
public:
    // return the top-right point (top first)
    Point_t getTopRight();
    Terrain(const int aId, const Point_t aTopRight);

    // Register all points of current terrain to map
    void registerToMap(GameMap& aMap);

    virtual const std::vector<Point_t> getAllPoints() const;

    virtual char getCharRepresentation(bool aUseId = false) const = 0;

    /* Return all terrains directly connected to
     * current terrain (edge, vertex, land) */
    virtual std::vector<Terrain*> getNeighbour(){return std::vector<Terrain*>();}; // TODO: implement

    virtual ~Terrain();
};

#endif /* INCLUDE_TERRAIN_HPP */
