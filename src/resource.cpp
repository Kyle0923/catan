#include "resource.hpp"

std::vector<Point_t> ResourceGrid::getAllPoints()
{
    std::vector<Point_t> allPoints;
    const int horizontalLength = 12;
    for (int jj = 0; jj <= 2; ++jj)
    {
        for (int ii = -jj; ii < horizontalLength + jj; ++ii)
        {
            allPoints.push_back(Point_t{mTopRight.x + ii, mTopRight.y + jj});
        }
    }
    for (int jj = 3; jj <= 4; ++jj)
    {
        for (int ii = 2 - jj; ii < horizontalLength + (4 - jj); ++ii)
        {
            allPoints.push_back(Point_t{mTopRight.x + ii, mTopRight.y + jj});
        }
    }
    return allPoints;
}

ResourceGrid::ResourceGrid(int id, Point_t topRight) : mLocation(id), mTopRight{topRight}
{
}

ResourceGrid::~ResourceGrid()
{
}
