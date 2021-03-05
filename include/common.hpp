#ifndef INCLUDE_COMMON_HPP
#define INCLUDE_COMMON_HPP

#include "logger.hpp"

struct Point_t
{
    int x;
    int y;
};

enum class ResourceTypes {
    ANY = -1,
    DESERT = 0,
    CLAY = 1,
    SHEEP = 2,
    WHEAT = 3,
    WOOD = 4,
    ORE = 5
};

#endif /* INCLUDE_COMMON_HPP */
