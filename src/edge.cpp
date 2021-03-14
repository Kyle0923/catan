#include "edge.hpp"

const int Edge::HORIZONTAL_LENGTH = 9;

const std::vector<Point_t> Edge::getAllPoints() const
{
    std::vector<Point_t> allPoints;
    allPoints.push_back(mTopRight);
    switch (mDirection)
    {
        case '-':
            for (int ii = 1; ii <= HORIZONTAL_LENGTH; ++ii)
            {
                allPoints.push_back(Point_t{mTopRight.x + ii, mTopRight.y});
            }
            break;
        case '/':
            allPoints.push_back(Point_t{mTopRight.x - 1, mTopRight.y + 1});
            break;
        case '\\':
            allPoints.push_back(Point_t{mTopRight.x + 1, mTopRight.y + 1});
            break;
        default:
            ERROR_LOG("Unknow Edge direction [", mDirection, "]");
    }
    return allPoints;
}

Edge::Edge(const int aId, const Point_t aTopRight, const char aDirection) :
    Terrain(aId, aTopRight)
{
    mOwner = "None";
    if (aDirection == '-' || aDirection == '/' || aDirection == '\\' )
    {
        mDirection = aDirection;
    }
    else
    {
        ERROR_LOG("Unknow Edge direction [", aDirection, "]");
    }
    switch (mDirection)
    {
        case '-':
            mOtherEnd = Point_t{mTopRight.x + HORIZONTAL_LENGTH, mTopRight.y};
            break;
        case '/':
            mOtherEnd = Point_t{mTopRight.x - 1, mTopRight.y + 1};
            break;
        case '\\':
            mOtherEnd = Point_t{mTopRight.x + 1, mTopRight.y + 1};
            break;
        default:
            // empty
            break;
    }
}

char Edge::getCharRepresentation(bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    else
    {
        return mDirection;
    }
}

Edge::~Edge()
{
}