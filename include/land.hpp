#ifndef INCLUDE_LAND_HPP
#define INCLUDE_LAND_HPP

#include "terrain.hpp"

class Land : public Terrain
{
private:
    ResourceTypes mResourceType;
    int mDiceNum;
    bool mIsUnderRob;
public:
    std::vector<Point_t> getAllPoints() override;
    Land(int aId, Point_t aTopRight, ResourceTypes aResourceType);
    ResourceTypes getResourceType();
    void setDiceNum(int aDice);
    int getDiceNum();
    ~Land();
};

#endif /* INCLUDE_LAND_HPP */
