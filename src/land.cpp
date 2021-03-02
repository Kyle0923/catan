#include "land.hpp"

std::vector<Point_t> Land::getAllPoints()
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

Land::Land(int aId, Point_t aTopRight, ResourceTypes aResourceType) :
    Terrain(aId, aTopRight),
    mResourceType(aResourceType),
    mDiceNum(0),
    isUnderRob(false)
{
    // empty
}

void Land::setDiceNum(int aDice)
{
    mDiceNum = aDice;
}

int Land::getDiceNum()
{
    return mDiceNum;
}

Land::~Land()
{
}
