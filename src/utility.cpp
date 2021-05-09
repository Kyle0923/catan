/**
 * Project: catan
 * @file utility.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include <iostream>
#include <sstream>
#include "utility.hpp"
#include "vertex.hpp"
#include "edge.hpp"

std::vector<std::string> splitString(const std::string& aString, char aDelim)
{
    if (aString.length() == 0)
    {
        return {""};
    }

    std::vector<std::string> result;
    std::string temp;
    std::istringstream paramStream(aString);
    while (getline(paramStream, temp, aDelim))
    {
        if (temp != "")
        {
            result.emplace_back(temp);
        }
    }
    if (result.size() == 0 || aString.back() == aDelim)
    {
        result.emplace_back("");
    }
    return result;
}

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
    return !((aPointA.x == aPointB.x) && (aPointA.y == aPointB.y));
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
        CASE_PRINT(ONE_VICTORY_POINT);
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

std::string actionStatusToStr(const ActionStatus aStatus)
{
#define CASE_PRINT(status) \
    case ActionStatus::status: \
        return #status

    switch (aStatus)
    {
        CASE_PRINT(SUCCESS);
        CASE_PRINT(PARTIAL_COMMAND);
        CASE_PRINT(PARAM_REQUIRED);
        CASE_PRINT(EXIT);
        CASE_PRINT(FAILED);
    }
    return "";

#undef CASE_PRINT
}

bool stringToInteger(const std::string& aToRead, int& aReturnInt)
{
    int rtn;
    char* endpos = nullptr;
    rtn = std::strtol(aToRead.c_str(), &endpos, 10);
    if (errno == ERANGE || endpos == aToRead.c_str())
    {
        return false;
    }
    aReturnInt = rtn;
    return true;
}

std::string stringVectorJoin(const std::vector<std::string>& aStrVec)
{
    std::string rtn;
    for (auto str : aStrVec)
    {
        rtn += "[" + str + "] ";
    }
    return rtn;
}

std::string& trimLeadingSpace(std::string& aString)
{
    static const std::string whitespaces (" \t\f\v\n\r");
    const std::size_t found = aString.find_first_not_of(whitespaces);
    if (found != std::string::npos)
    {
        aString.erase(0, found);
    }
    else
    {
        // str is all whitespace
        aString.clear();
    }
    return aString;
}

std::string& trimTrailingSpace(std::string& aString)
{
    static const std::string whitespaces (" \t\f\v\n\r");
    const std::size_t found = aString.find_last_not_of(whitespaces);
    if (found != std::string::npos)
    {
        aString.erase(found + 1);
    }
    else
    {
        // str is all whitespace
        aString.clear();
    }
    return aString;
}
