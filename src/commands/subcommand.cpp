/**
 * Project: catan
 * @file subcommand.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_common.hpp"

std::string SubCmdHandler::command() const
{
    return "subcmd";
}

ActionStatus SubCmdHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    aUi.pushCommandHelper(std::make_unique<CommandDispatcher>(std::vector<CommandHandler*>({new SubCmdHandler()})));
    return ActionStatus::SUCCESS;
}
