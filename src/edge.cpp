#include "edge.hpp"

const int Edge::HORIZONTAL_LENGTH = 9;

std::vector<Point_t> Edge::getAllPoints()
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

Edge::Edge(int aId, Point_t aTopRight, char aDirection) :
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

Edge::~Edge()
{
}