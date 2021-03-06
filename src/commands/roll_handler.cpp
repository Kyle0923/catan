/**
 * Project: catan
 * @file roll_handler.cpp
 *
 * @brief roll the dice, if rolled 7, push_back the mRobberMoveHandler for further handling
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_handlers.hpp"
#include "command_parameter_reader.hpp"

std::string RollHandler::command() const
{
    return "roll";
}

std::string RollHandler::description() const
{
    return "roll the dice";
}

ActionStatus RollHandler::statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& /* aArgs */, std::vector<std::string>& aReturnMsg)
{
    int dice = aMap.rollDice();
    aReturnMsg.emplace_back("You rolled: " + std::to_string(dice));
    if (dice == 7)
    {
        aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(&mRobberMoveHandler));
    }
    return ActionStatus::SUCCESS;
}

RollHandler::RollHandler() :
    mRobberMoveHandler(RobberMoveHandler::getRobberHandler())
{
    // empty
}
