/**
 * Project: catan
 * @file land.hpp
 * @brief abstraction of the resource tiles
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_LAND_HPP
#define INCLUDE_LAND_HPP

#include "terrain.hpp"

class Land : public Terrain
{
private:
    ResourceTypes mResourceType;
    int mDiceNum;
    bool mIsUnderRobber;
    std::vector<const Vertex*> mAdjacentVertices;

    int addAdjacency(GameMap& aMap, bool aIsVertex, const int aPointX, const int aPointY, const char aPattern = '.');

public:
    std::vector<Point_t> getAllPoints() const override;
    int populateAdjacencies(GameMap& aMap) override;

    Land(const int aId, const Point_t aTopLeft, const ResourceTypes aResourceType);
    void setResourceType(ResourceTypes aResourceType);
    ResourceTypes getResourceType();
    void setDiceNum(int aDice);
    int getDiceNum();
    void rob(bool aIsRob);
    bool isUnderRobber() const;
    const std::vector<const Vertex*>& getAdjacentVertices() const;

    char getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const override;
    std::string getStringId() const override;

    virtual ~Land();
};

#endif /* INCLUDE_LAND_HPP */
