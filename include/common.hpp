#ifndef INCLUDE_COMMON_HPP
#define INCLUDE_COMMON_HPP

#include "logger.hpp"

struct Point_t
{
    int x;
    int y;
};

enum class ResourceTypes {
    DESERT,
    CLAY,
    SHEEP,
    WHEAT,
    WOOD,
    ORE
};

#endif /* INCLUDE_COMMON_HPP */
