#include "command_dispatcher.hpp"
#include "command_handler.hpp"
#include "game_map.hpp"
#include "user_interface.hpp"
#include "logger.hpp"

std::string SubCmdHandler::command() const
{
    return "subcmd";
}

ActionStatus SubCmdHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    aUi.pushCmdDispatcher(std::make_unique<CommandDispatcher>(std::vector<CommandHandler*>({new SubCmdHandler()})));
    return ActionStatus::SUCCESS;
}
