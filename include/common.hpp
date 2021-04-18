/**
 * Project: catan
 * @file common.hpp
 * @brief common data types
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMON_HPP
#define INCLUDE_COMMON_HPP

#include <string>
#include "curses.h"

struct Point_t
{
    size_t x;
    size_t y;
};

enum class ResourceTypes
{
    NONE = -1,
    CLAY = 0,
    SHEEP = 1,
    WHEAT = 2,
    WOOD = 3,
    ORE = 4,
    DESERT = 5,
    ANY = 6,
};

#endif /* INCLUDE_COMMON_HPP */
