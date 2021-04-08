#include "cli_command_manager.hpp"
#include "command_handler.hpp"
#include "logger.hpp"

std::string CommandHandler::description() const
{
    return "";
}

std::string ExitHandler::command() const
{
    return "exit";
}

ActionStatus ExitHandler::act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo)
{
    return ActionStatus::SUCCESS;
}

std::string QuitHandler::command() const
{
    return "quit";
}

ActionStatus QuitHandler::act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo)
{
    return ActionStatus::SUCCESS;
}

std::string HelpHandler::command() const
{
    return "help";
}

ActionStatus HelpHandler::act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo)
{
    const std::map<std::string, CommandHandler*>& handlers = mManager->getHandlers();
    for (auto iter : handlers)
    {
        std::string cmd = iter.first;
        if (iter.second->description() != "")
        {
            cmd += ": " + iter.second->description();
        }
        aInfo.push_back(cmd);
    }
    return ActionStatus::SUCCESS;
}

HelpHandler::HelpHandler(CliCommandManager* const aManager): mManager(aManager)
{
}