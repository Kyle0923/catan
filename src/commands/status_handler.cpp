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

#include "command_common.hpp"


std::string StatusHandler::command() const
{
    return "status";
}

std::string StatusHandler::description() const
{
    return "display player's status, optionally you may provide a 'player ID' to check on other's status";
}

ActionStatus StatusHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    int playerId = -1;
    if (aArgs.size() > 0)
    {
        char* endpos = nullptr;
        playerId = std::strtol(aArgs.front().c_str(), &endpos, 10);
        if (errno == ERANGE || endpos == aArgs.front().c_str())
        {
            // strtol failed, fallback to default -1
            playerId = -1;
        }
    }
    aMap.summarizePlayerStatus(playerId, aReturnMsg);
    return ActionStatus::SUCCESS;
}
