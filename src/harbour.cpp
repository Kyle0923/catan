/**
 * Project: catan
 * @file harbour.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include <algorithm>
#include "logger.hpp"
#include "game_map.hpp"
#include "harbour.hpp"
#include "blank.hpp"
#include "utility.hpp"

int Harbour::calculatePoints(GameMap& aMap)
{
    if (mVertex1.y == mVertex2.y)
    {
        // +----------+  |      WOOD
        //  xx      xx   |     xx  xx
        //    xx  xx     |   xx      xx
        //     SHEEP     |  +----------+
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

        mTopLeft.x = mVertex1.x + 4;
        mTopLeft.y = mVertex1.y + dy * 3;
    }
    else if (mVertex1.x > mVertex2.x)
    {
        //      xxxx+   !      +
        // BRICK   /    !     / x
        //      x /     !    /   WOOD
        //       +      !   +xxxx
        Point_t pointA = mVertex1;  // point next to "xxxx"
        Point_t pointB = mVertex2;  // the other point
        int dx = -1;
        int dy = -1;
        const Terrain* pTest = aMap.getTerrain(mVertex2.x + 1, mVertex2.y);
        if (pTest == Blank::getBlank() || pTest == nullptr)
        {
            // harbour is right to the vertices
            pointA = mVertex2;
            pointB = mVertex1;
            dx = 1;
            dy = 1;

            mTopLeft.x = pointB.x + 2;
        }
        else
        {
            mTopLeft.x = pointB.x - 1 - resourceTypesToStr(mResourceType).length();
        }
        mLinks.push_back(Point_t{pointA.x + dx * 1, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 2, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 3, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 4, pointA.y});

        mLinks.push_back(Point_t{pointB.x + dx, pointB.y + dy});

        mTopLeft.y = pointA.y - dy;
    }
    else
    {
        //     +     |  +xxxx
        //    x \    |   \   WHEAT
        // ORE   \   |    \ x
        //    xxxx+  |     +
        Point_t pointA = mVertex2;  // point next to "xxxx"
        Point_t pointB = mVertex1;  // the other point
        int dx = -1;
        int dy = 1;
        const Terrain* pTest = aMap.getTerrain(mVertex1.x + 1, mVertex1.y);
        if (pTest == Blank::getBlank() || pTest == nullptr)
        {
            // harbour is right to the vertices
            pointA = mVertex1;
            pointB = mVertex2;
            dx = 1;
            dy = -1;

            mTopLeft.x = pointB.x + 2;
        }
        else
        {
            mTopLeft.x = pointB.x - 1 - resourceTypesToStr(mResourceType).length();
        }
        mLinks.push_back(Point_t{pointA.x + dx * 1, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 2, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 3, pointA.y});
        mLinks.push_back(Point_t{pointA.x + dx * 4, pointA.y});

        mLinks.push_back(Point_t{pointB.x + dx, pointB.y + dy});

        mTopLeft.y = pointA.y - dy;
    }
    return 0;
}

std::vector<Point_t> Harbour::getAllPoints() const
{
    std::vector<Point_t> allPoints(mLinks);
    for (size_t ii = 0; ii < std::min(resourceTypesToStr(mResourceType).length(), size_t{5U}); ++ii)
    {
        allPoints.push_back(Point_t{mTopLeft.x + ii, mTopLeft.y});
    }
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

char Harbour::getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    else
    {
        if (std::find(mLinks.begin(), mLinks.end(), Point_t{aPointX, aPointY}) != mLinks.end())
        {
            return 'x';
        }
        else
        {
            if (mResourceType != ResourceTypes::ANY)
            {
                return resourceTypesToStr(mResourceType).at(aPointX - mTopLeft.x);
            }
            else
            {
                static const std::vector<char> harbourAny = {'3', ':', '1'};
                return harbourAny[aPointX - mTopLeft.x];
            }
        }
    }
}

std::string Harbour::getStringId() const
{
    return Logger::formatString("Harbour#", mId);
}

Harbour::~Harbour()
{
}
