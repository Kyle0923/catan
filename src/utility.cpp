/**
 * Project: catan
 * @file utility.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "iostream"
#include "utility.hpp"
#include "vertex.hpp"
#include "edge.hpp"

size_t PointHash::operator()(const Point_t& aPoint) const
{
    const size_t shift = sizeof(size_t) / 2;
    return (aPoint.x << shift) | aPoint.y;
}

bool operator==(const Point_t& aPointA, const Point_t& aPointB)
{
    return (aPointA.x == aPointB.x) && (aPointA.y == aPointB.y);
}

bool operator!=(const Point_t& aPointA, const Point_t& aPointB)
{
    return !(aPointA.x == aPointB.x) && (aPointA.y == aPointB.y);
}

std::ostream& operator<< (std::ostream& aStream, const Point_t& aPoint)
{
    return aStream << "Point{" << aPoint.x << ',' << aPoint.y << '}';
}

std::ostream& operator<< (std::ostream& aStream, const Edge* const & aEdge)
{
    return aStream << aEdge->getStringId();
}

std::ostream& operator<< (std::ostream& aStream, const Vertex* const & aVertex)
{
    return aStream << aVertex->getStringId();
}

std::string resourceTypesToStr(const ResourceTypes aResource)
{
#define CASE_PRINT(resource) \
    case ResourceTypes::resource: \
        return #resource

    switch (aResource)
    {
        CASE_PRINT(NONE);
        CASE_PRINT(CLAY);
        CASE_PRINT(SHEEP);
        CASE_PRINT(WHEAT);
        CASE_PRINT(WOOD);
        CASE_PRINT(ORE);
        CASE_PRINT(DESERT);
        CASE_PRINT(ANY);
    }
    return "";

#undef CASE_PRINT
}

std::string developmentCardTypesToStr(const DevelopmentCardTypes aCard)
{
#define CASE_PRINT(devCard) \
    case DevelopmentCardTypes::devCard: \
        return #devCard

    switch (aCard)
    {
        CASE_PRINT(KNIGHT);
        CASE_PRINT(ROAD_BUILDING);
        CASE_PRINT(YEAR_OF_PLENTY);
        CASE_PRINT(MONOPOLY);
        CASE_PRINT(VICTORY_POINT);
    }
    return "";

#undef CASE_PRINT
}

std::string colonyTypesToStr(const ColonyType aColony)
{
#define CASE_PRINT(colony) \
    case ColonyType::colony: \
        return #colony

    switch (aColony)
    {
        CASE_PRINT(NONE);
        CASE_PRINT(SETTLEMENT);
        CASE_PRINT(CITY);
    }
    return "";

#undef CASE_PRINT
}

void trimTrailingSpace(std::string& aString)
{
    static const std::string whitespaces (" \t\f\v\n\r");
    const std::size_t found = aString.find_last_not_of(whitespaces);
    if (found!=std::string::npos)
    {
        aString.erase(found+1);
    }
    else
    {
        // str is all whitespace
        aString.clear();
    }
}
