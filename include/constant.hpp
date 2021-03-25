#ifndef INCLUDE_CONSTANT_HPP
#define INCLUDE_CONSTANT_HPP

#include <cstddef>

namespace constant
{

constexpr size_t NUM_LAND_DESERT = 1U;
constexpr size_t NUM_LAND_CLAY   = 3U;
constexpr size_t NUM_LAND_SHEEP  = 4U;
constexpr size_t NUM_LAND_WHEAT  = 4U;
constexpr size_t NUM_LAND_WOOD   = 4U;
constexpr size_t NUM_LAND_ORE    = 3U;

constexpr size_t NUM_OF_HARBOUR = 9U;   // total number of harbours
constexpr size_t NUM_HARBOUR_CLAY  = 1U;
constexpr size_t NUM_HARBOUR_SHEEP = 1U;
constexpr size_t NUM_HARBOUR_WHEAT = 1U;
constexpr size_t NUM_HARBOUR_WOOD  = 1U;
constexpr size_t NUM_HARBOUR_ORE   = 1U;
constexpr size_t NUM_HARBOUR_RESOURCE = NUM_HARBOUR_CLAY + NUM_HARBOUR_SHEEP + NUM_HARBOUR_WHEAT + NUM_HARBOUR_WOOD + NUM_HARBOUR_ORE;
constexpr size_t NUM_HARBOUR_ANY = NUM_OF_HARBOUR - NUM_HARBOUR_RESOURCE; // num of 3:1 harbour

constexpr size_t NUM_DICE_3_TO_11 = 2U;
constexpr size_t NUM_DICE_2_OR_12 = 1U;
constexpr size_t NUM_DICE_7       = 0U;

} /* namespace constant */

#endif /* INCLUDE_CONSTANT_HPP */