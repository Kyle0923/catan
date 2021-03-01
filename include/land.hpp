#ifndef INCLUDE_LAND_HPP
#define INCLUDE_LAND_HPP

#include "terrain.hpp"

class Land : public Terrain
{
private:
    int mDiceNum;
    ResourceTypes mResourceType;
public:
    std::vector<Point_t> getAllPoints() override;
    Land(int aId, Point_t aTopRight, ResourceTypes aResourceType);
    ~Land();
};



#endif /* INCLUDE_LAND_HPP */
