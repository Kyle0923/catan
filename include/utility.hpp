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
#include <array>
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
int indexInVector(const T& aTofind, const std::vector<T>& aVector)
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

template<typename T, typename U, size_t N, std::enable_if_t<N != 0>* = nullptr>
std::string summarizeEnumArray(const std::array<T, N>& aEnumArray, std::string (*enumToStringFunc)(const U))
{
    std::string message = enumToStringFunc(static_cast<U>(0U)) + ": " + std::to_string(aEnumArray.at(0U));
    for (size_t index = 1; index < N; ++index)
    {
        message += ", " + enumToStringFunc(static_cast<U>(index)) + ": " + std::to_string(aEnumArray.at(index));
    }
    return message;
};

// read string, return true if successful or false if failed
// if failed, aReturnInt is not changed
extern bool stringToInteger(const std::string& aToRead, int& aReturnInt);

// join a string_vector in the form of "[foo] [bar] [baz]""
extern std::string stringVectorJoin(const std::vector<std::string>& aStrVec);

extern std::string& trimLeadingSpace(std::string& aString);
extern std::string& trimTrailingSpace(std::string& aString);

#endif /* INCLUDE_UTILITY_HPP */