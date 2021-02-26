#ifndef INCLUDE_COMMON_HPP
#define INCLUDE_COMMON_HPP

#include <utility>

struct Point_t
{
    int x;
    int y;
};


enum class ResourceTypes {
    SHEEP,
    WHEAT,
    WOOD,
    CLAY,
    ORE
};

#endif /* INCLUDE_COMMON_HPP */