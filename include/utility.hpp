/**
 * Project: catan
 * @file utility.hpp
 * @brief handy helper functions
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_UTILITY_HPP
#define INCLUDE_UTILITY_HPP

#include <vector>
#include <algorithm>
#include "common.hpp"

class Vertex;
class Edge;

class PointHash
{
public:
    size_t operator()(const Point_t& aPoint) const;
};

template<typename T>
int isInVector(const T& aTofind, const std::vector<T>& aVector)
{
    auto pos = std::find(aVector.begin(), aVector.end(), aTofind);
    if (pos != aVector.end())
    {
        // found
        return pos - aVector.begin();
    }
    else
    {
        return -1;
    }
}

extern std::vector<std::string> splitString(const std::string& aString, char aDelim = ' ');

extern bool operator==(const Point_t& aPointA, const Point_t& aPointB);
extern bool operator!=(const Point_t& aPointA, const Point_t& aPointB);
extern std::ostream& operator<< (std::ostream& aStream, const Point_t& aPoint);
extern std::ostream& operator<< (std::ostream& aStream, const Edge* const & aEdge);
extern std::ostream& operator<< (std::ostream& aStream, const Vertex* const & aVertex);
extern std::string resourceTypesToStr(const ResourceTypes aResource);
extern std::string developmentCardTypesToStr(const DevelopmentCardTypes aCard);
extern std::string colonyTypesToStr(const ColonyType aColony);
extern std::string actionStatusToStr(const ActionStatus aStatus);
extern void trimTrailingSpace(std::string& aString);

#endif /* INCLUDE_UTILITY_HPP */