/**
 * Project: catan
 * @file terrain.hpp
 * @brief base class, abstraction of different types of points on the map
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

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
    Point_t mTopLeft;

    /* mNeighbour stores the neighbours of insterest (not all neighbours)
     * for lands    => vertices
     * for vertices => edges
     * for edges    => vertices
     */
    std::vector<const Terrain*> mNeighbour;
public:
    // return the top-left point (top first)
    Point_t getTopLeft();
    int getId() const;
    Terrain(const int aId, const Point_t aTopLeft);

    // Register all points of current terrain to map
    void registerToMap(GameMap& aMap);

    virtual std::vector<Point_t> getAllPoints() const;

    virtual int populateNeighbours(GameMap& aMap);

    virtual char getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const = 0;
    virtual std::string getStringId() const = 0;

    virtual ~Terrain();
};

#endif /* INCLUDE_TERRAIN_HPP */
