#ifndef INCLUDE_RESOURCE_HPP
#define INCLUDE_RESOURCE_HPP

#include <vector>
#include "common.hpp"

class ResourceGrid
{
private:
    int mLocation;
    // int mDiceNum;
    // ResourceTypes mResourceType;
    Point_t mTopRight;
public:
    std::vector<Point_t> getAllPoints();
    int getLocation() {return mLocation;};
    ResourceGrid(int id, Point_t topRight);
    ~ResourceGrid();
};



#endif  /* INCLUDE_RESOURCE_HPP */