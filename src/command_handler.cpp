#include "cli_command_manager.hpp"
#include "command_handler.hpp"
#include "logger.hpp"

std::string ExitHandler::command()
{
    return "exit";
}

ActionStatus ExitHandler::act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo)
{
    return ActionStatus::NO_ACTION;
}

std::string QuitHandler::command()
{
    return "quit";
}

ActionStatus QuitHandler::act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo)
{
    return ActionStatus::NO_ACTION;
}

std::string HelpHandler::command()
{
    return "help";
}

ActionStatus HelpHandler::act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo)
{
    aInfo = mManager->getCommands();
    DEBUG_LOG_L0("help handler: ", aInfo);
    return ActionStatus::INFO;
}

HelpHandler::HelpHandler(CliCommandManager* const aManager): mManager(aManager)
{
}