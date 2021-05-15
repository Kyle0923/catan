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
constexpr size_t CONSUMABLE_RESOURCE_SIZE = static_cast<size_t>(ResourceTypes::ORE) + 1U;

// the order in DevelopmentCardTypes must be exactly the same as in DevelopmentCardHandler::mPlayableDevCard
enum class DevelopmentCardTypes
{
    KNIGHT = 0,
    ROAD_BUILDING,
    YEAR_OF_PLENTY,
    MONOPOLY,
    ONE_VICTORY_POINT
};
constexpr size_t DEVELOPMENT_CARD_TYPE_SIZE = static_cast<size_t>(DevelopmentCardTypes::ONE_VICTORY_POINT) + 1U;

enum ColonyType
{
    NONE = 0,
    SETTLEMENT = 1,
    CITY = 2
};

enum class ActionStatus
{
    SUCCESS = 0,
    PARTIAL_COMMAND,  // partial command, e.g, "he" is a partial command of "help", do not remove current cli
    PARAM_REQUIRED,   // parameter required, commandDispatcher will push cmdHelper_stack with CommandParameterReader
    EXIT,             // only "exit" & "quit" should return this, child command finished, back to parent command, effectively pop cmdHelper stack
    FAILED,           // internal error, write to log
};

#endif /* INCLUDE_COMMON_HPP */
