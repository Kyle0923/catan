/**
 * Project: catan
 * @file command_common.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include <map>
#include "command_common.hpp"

std::string CommandHandler::description() const
{
    return "";
}

std::string ExitHandler::command() const
{
    return "exit";
}

std::string ExitHandler::description() const
{
    return "Exit current command";
}

ActionStatus ExitHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    return ActionStatus::EXIT;
}

std::string HelpHandler::command() const
{
    return "help";
}

ActionStatus HelpHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    const std::map<std::string, CommandHandler*>& handlers = mDispatcher->getHandlers();
    for (auto iter : handlers)
    {
        std::string cmd = iter.first;
        if (iter.second->description() != "")
        {
            cmd += " - " + iter.second->description();
        }
        if ( (aArgs.size() > 0 && isInVector(iter.first, aArgs) >= 0)
             || (aArgs.size() == 0) )
        {
            aReturnMsg.push_back(cmd);
        }
    }
    return ActionStatus::SUCCESS;
}

HelpHandler::HelpHandler(CommandDispatcher* const aDispatcher): mDispatcher(aDispatcher)
{
    // empty
}

std::string NextHandler::command() const
{
    return "next";
}

std::string NextHandler::description() const
{
    return "Pass to next player";
}

ActionStatus NextHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    size_t nextPlayer = aMap.nextPlayer();
    aReturnMsg.push_back(Logger::formatString("Next Player is player#", nextPlayer));
    return ActionStatus::SUCCESS;
}
