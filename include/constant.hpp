/**
 * Project: catan
 * @file constant.hpp
 * @brief global constants
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_CONSTANT_HPP
#define INCLUDE_CONSTANT_HPP

#include <cstdint>

constexpr uint32_t VER_MAJOR = 0;
constexpr uint32_t VER_MINOR = 8;
constexpr uint32_t VER_PATCH = 4;

namespace constant
{

constexpr size_t NUM_LAND_DESERT = 1U;
constexpr size_t NUM_LAND_BRICK  = 3U;
constexpr size_t NUM_LAND_SHEEP  = 4U;
constexpr size_t NUM_LAND_WHEAT  = 4U;
constexpr size_t NUM_LAND_WOOD   = 4U;
constexpr size_t NUM_LAND_ORE    = 3U;

constexpr size_t NUM_LANDS_DEFAULT = 19U; // num of lands for default map

constexpr size_t NUM_OF_HARBOUR = 9U;   // total number of harbours
constexpr size_t NUM_HARBOUR_BRICK = 1U;
constexpr size_t NUM_HARBOUR_SHEEP = 1U;
constexpr size_t NUM_HARBOUR_WHEAT = 1U;
constexpr size_t NUM_HARBOUR_WOOD  = 1U;
constexpr size_t NUM_HARBOUR_ORE   = 1U;
constexpr size_t NUM_HARBOUR_RESOURCE = NUM_HARBOUR_BRICK + NUM_HARBOUR_SHEEP + NUM_HARBOUR_WHEAT + NUM_HARBOUR_WOOD + NUM_HARBOUR_ORE;
constexpr size_t NUM_HARBOUR_ANY = NUM_OF_HARBOUR - NUM_HARBOUR_RESOURCE; // num of 3:1 harbour

constexpr size_t NUM_DICE_3_TO_11 = 2U;
constexpr size_t NUM_DICE_2_OR_12 = 1U;
constexpr size_t NUM_DICE_7       = 0U;

constexpr size_t MAX_HISTORY_SIZE = 10U; // number of history command recorded

#ifdef RELEASE
constexpr int DEFAULT_DEBUG_LEVEL = 5;
#else
constexpr int DEFAULT_DEBUG_LEVEL = 2;
#endif /* ifdef RELEASE */

} /* namespace constant */

#endif /* INCLUDE_CONSTANT_HPP */