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

#include "common.hpp"

class Vertex;
class Edge;

class PointHash
{
public:
    size_t operator()(const Point_t& aPoint) const;
};

extern bool operator==(const Point_t& aPointA, const Point_t& aPointB);
extern bool operator!=(const Point_t& aPointA, const Point_t& aPointB);
extern std::ostream& operator<< (std::ostream& aStream, const Point_t& aPoint);
extern std::ostream& operator<< (std::ostream& aStream, const Edge* const & aEdge);
extern std::ostream& operator<< (std::ostream& aStream, const Vertex* const & aVertex);
extern std::string resourceTypesToStr(const ResourceTypes aResource);
extern std::string developmentCardTypesToStr(const DevelopmentCardTypes aCard);
extern void trimTrailingSpace(std::string& aString);

#endif /* INCLUDE_UTILITY_HPP */