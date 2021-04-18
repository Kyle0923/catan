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

class PointHash
{
public:
    size_t operator()(const Point_t& aPoint) const;
};

extern bool operator==(const Point_t& aPointA, const Point_t& aPointB);
extern std::ostream& operator<< (std::ostream& aStream, const Point_t& aPoint);
extern std::string resourceTypesToStr(const ResourceTypes aResource);
extern void trimTrailingSpace(std::string& aString);

#endif /* INCLUDE_UTILITY_HPP */