#include "game_map.hpp"
#include "harbour.hpp"
#include "blank.hpp"

int Harbour::calculatePoints(GameMap& aMap)
{
    if (mVertex1.y == mVertex2.y)
    {
        // +----------+  |       H01
        //  xx      xx   |     xx  xx
        //    xx  xx     |   xx      xx
        //      H01      |  +----------+
        int dy = 1;
        if (aMap.getTerrain(mVertex1.x, mVertex1.y - 1) == Blank::getBlank())
        {
            // harbour is above the vertices
            dy = -1;
        }
        mLinks.push_back(Point_t{mVertex1.x + 1, mVertex1.y + dy});
        mLinks.push_back(Point_t{mVertex1.x + 2, mVertex1.y + dy});
        mLinks.push_back(Point_t{mVertex2.x - 1, mVertex1.y + dy});
        mLinks.push_back(Point_t{mVertex2.x - 2, mVertex1.y + dy});

        mLinks.push_back(Point_t{mVertex1.x + 3, mVertex1.y + dy * 2});
        mLinks.push_back(Point_t{mVertex1.x + 4, mVertex1.y + dy * 2});
        mLinks.push_back(Point_t{mVertex2.x - 3, mVertex1.y + dy * 2});
        mLinks.push_back(Point_t{mVertex2.x - 4, mVertex1.y + dy * 2});

        mTopRight.x = mVertex1.x + 5;
        mTopRight.y = mVertex1.y + dy * 3;
    }
    else if (mVertex1.x > mVertex2.x)
    {
        //    xxxx+   !      +
        // H01   /    !     / x
        //    x /     !    /   H01
        //     +      !   +xxxx
        Point_t pointA = mVertex1;  // point next to "xxxx"
        Point_t pointB = mVertex2;  // the other point
        int dx = -1;
        int dy = -1;
        Terrain* pTest = aMap.getTerrain(mVertex1.x + 1, mVertex1.y);
        if (pTest == Blank::getBlank() || pTest == nullptr)
        {
            // harbour is right to the vertices
            pointA = mVertex2;
            pointB = mVertex1;
            dx = 1;
            dy = 1;

            mTopRight.x = pointB.x + 2;
        }
        else
        {
            mTopRight.x = pointB.x - 4;
        }
        mLinks.push_back(Point_t{pointA.x + dx * 1, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 2, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 3, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 4, pointA.y});

        mLinks.push_back(Point_t{pointB.x + dx, pointB.y + dy});

        mTopRight.y = pointA.y - dy;
    }
    else
    {
        //     +     |  +xxxx
        //    x \    |   \   H01
        // H01   \   |    \ x
        //    xxxx+  |     +
        Point_t pointA = mVertex2;  // point next to "xxxx"
        Point_t pointB = mVertex1;  // the other point
        int dx = -1;
        int dy = 1;
        Terrain* pTest = aMap.getTerrain(mVertex1.x + 1, mVertex1.y);
        if (pTest == Blank::getBlank() || pTest == nullptr)
        {
            // harbour is right to the vertices
            pointA = mVertex1;
            pointB = mVertex2;
            dx = 1;
            dy = -1;

            mTopRight.x = pointB.x + 2;
        }
        else
        {
            mTopRight.x = pointB.x - 4;
        }
        mLinks.push_back(Point_t{pointA.x + dx * 1, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 2, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 3, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 4, pointA.y});

        mLinks.push_back(Point_t{pointB.x + dx, pointB.y + dy});

        mTopRight.y = pointA.y - dy;
    }
    return 0;
}

std::vector<Point_t> Harbour::getAllPoints() const
{
    std::vector<Point_t> allPoints(mLinks);
    allPoints.push_back(mTopRight);
    allPoints.push_back(Point_t{mTopRight.x + 1, mTopRight.y});
    allPoints.push_back(Point_t{mTopRight.x + 2, mTopRight.y});
    return allPoints;
}

ResourceTypes Harbour::getResourceType()
{
    return mResourceType;
}

void Harbour::setResourceType(ResourceTypes aResource)
{
    mResourceType = aResource;
}

Harbour::Harbour(const int aId, const ResourceTypes aResource, const Point_t aVertex1, const Point_t aVertex2) :
    Terrain(aId, Point_t{0, 0}),
    mResourceType(aResource)
{
    if (aVertex1.y < aVertex2.y)
    {
        mVertex1 = aVertex1;
        mVertex2 = aVertex2;
    }
    else if (aVertex1.y == aVertex2.y && aVertex1.x < aVertex2.x)
    {
        mVertex1 = aVertex1;
        mVertex2 = aVertex2;
    }
    else
    {
        mVertex1 = aVertex2;
        mVertex2 = aVertex1;
    }
}

char Harbour::getCharRepresentation(bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    else
    {
        return 'x';
    }
}

std::string Harbour::getFullId() const
{
    return Logger::formatString("Harbour#", mId);
}

Harbour::~Harbour()
{
}
