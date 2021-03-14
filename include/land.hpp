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
    Land(const int aId, const Point_t aTopRight, const ResourceTypes aResourceType);
    void setResourceType(ResourceTypes aResourceType);
    ResourceTypes getResourceType();
    void setDiceNum(int aDice);
    int getDiceNum();
    char getCharRepresentation(bool aUseId = false) const override;
    virtual ~Land();
};

#endif /* INCLUDE_LAND_HPP */
