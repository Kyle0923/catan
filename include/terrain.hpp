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
#include "user_interface.hpp"

class GameMap;

class Terrain
{
protected:
    const int mId;
    Point_t mTopLeft;
    ColorPairIndex mColorIndex;

    /* mAdjacencies stores the adjacencies of insterest (not all adjacencies)
     * for lands    => vertices
     * for vertices => edges
     * for edges    => vertices
     */
    std::vector<const Terrain*> mAdjacencies;
public:
    // return the top-left point (top first)
    Point_t getTopLeft() const;
    int getId() const;
    Terrain(const int aId, const Point_t aTopLeft);

    void setColor(ColorPairIndex aColorIndex);

    // Register all points of current terrain to map
    void registerToMap(GameMap& aMap);

    virtual std::vector<Point_t> getAllPoints() const;

    virtual int populateAdjacencies(GameMap& aMap);

    virtual char getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const = 0;
    chtype getColorCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const;
    virtual std::string getStringId() const = 0;

    virtual ~Terrain();
};

#endif /* INCLUDE_TERRAIN_HPP */
