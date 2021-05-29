/**
 * Project: catan
 * @file status_handler.cpp
 *
 * @brief print the status of player
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_handlers.hpp"


std::string StatusHandler::command() const
{
    return "status";
}

std::string StatusHandler::description() const
{
    return "display player's status, optionally you may provide a 'player ID' to check on other's status";
}

ActionStatus StatusHandler::statelessRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    int playerId = -1;
    if (aArgs.size() > 0)
    {
        stringToInteger(aArgs.front(), playerId);
    }
    aMap.summarizePlayerStatus(playerId, aReturnMsg);
    return ActionStatus::SUCCESS;
}
