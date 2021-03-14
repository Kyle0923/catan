#include "utility.hpp"

size_t PointHash::operator()(const Point_t& aPoint) const
{
    const size_t shift = sizeof(size_t) / 2;
    return (aPoint.x << shift) | aPoint.y;
}

bool operator==(const Point_t& aPointA, const Point_t& aPointB)
{
    return (aPointA.x == aPointB.x) && (aPointA.y == aPointB.y);
}

std::ostream & operator<< (std::ostream &aStream, const Point_t& aPoint)
{
    return aStream << aPoint.x << ',' << aPoint.y;
}