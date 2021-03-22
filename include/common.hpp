#ifndef INCLUDE_COMMON_HPP
#define INCLUDE_COMMON_HPP

#include "logger.hpp"

struct Point_t
{
    size_t x;
    size_t y;
};

enum class ResourceTypes {
    NONE = -1,
    DESERT = 0,
    CLAY = 1,
    SHEEP = 2,
    WHEAT = 3,
    WOOD = 4,
    ORE = 5,
    ANY = 6,
};

#endif /* INCLUDE_COMMON_HPP */
