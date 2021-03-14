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
        for (int ii = jj - 4; ii < horizontalLength + (4 - jj); ++ii)
        {
            allPoints.push_back(Point_t{mTopRight.x + ii, mTopRight.y + jj});
        }
    }
    return allPoints;
}

void Land::setResourceType(ResourceTypes aResourceType)
{
    mResourceType = aResourceType;
}

ResourceTypes Land::getResourceType()
{
    return mResourceType;
}

Land::Land(const int aId, const Point_t aTopRight, const ResourceTypes aResourceType) :
    Terrain(aId, aTopRight),
    mResourceType(aResourceType),
    mDiceNum(0),
    mIsUnderRob(false)
{
    // empty
}

void Land::setDiceNum(int aDice)
{
    mDiceNum = aDice;
}

char Land::getCharRepresentation(bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    else
    {
        if (mResourceType == ResourceTypes::ANY)
        {
            return '.';
        }
        else
        {
            return static_cast<char>((int)mResourceType) + '0';
        }
    }
}

int Land::getDiceNum()
{
    return mDiceNum;
}

Land::~Land()
{
}
