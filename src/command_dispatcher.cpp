/**
 * Project: catan
 * @file command_dispatcher.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include <sstream>
#include "command_dispatcher.hpp"
#include "command_common.hpp"
#include "logger.hpp"
#include "command_parameter_reader.hpp"

BackdoorHandler CommandDispatcher::mBackdoorHandler;  // handles backdoor commands

std::vector<std::string> CommandDispatcher::getPossibleInputs(const std::string& aInput, std::string* const aLongestCommonStr) const
{
    return stringMatcher(aInput, mCommand, aLongestCommonStr);
}

ActionStatus CommandDispatcher::act(GameMap& aMap, UserInterface& aUi, std::string aInput, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    if (aInput == "")
    {
        return ActionStatus::SUCCESS;
    }

    std::string command = aInput.substr(0, aInput.find(' '));

    if (command != mBackdoorHandler.command() && mCommandHandler.count(command) != 1)
    {
        // unknown command, possible partial command

        std::vector<std::string> matchingCommand = getPossibleInputs(aInput);
        if (matchingCommand.size() != 1)
        {
            INFO_LOG("Unknown command: \'" + aInput + '\'');
            aReturnMsg.push_back("Unknown command: \'" + aInput + '\'');
            return ActionStatus::SUCCESS;
        }
        command = matchingCommand.front();

        // incomplete command
        INFO_LOG("Possible incomplete command: \'" + aInput + "\', maybe: " + command);
        aReturnMsg.push_back("Unknown command: \'" + aInput + '\'');
        aReturnMsg.push_back("maybe: \'" + command + "\'?");
        return ActionStatus::PARTIAL_COMMAND;
    }

    // parse aInput, extra word are split by whitespace and pass to command_handler as parameter
    std::vector<std::string> commandParam;
    if (aInput.size() > command.size())
    {
        std::string params = aInput.substr(command.size());
        commandParam = splitString(params);
    }
    INFO_LOG("calling handler for cmd \'" + command + "\', arg: ", commandParam);

    CommandHandler* pCmdHandler;
    if (command == mBackdoorHandler.command())
    {
        pCmdHandler = &mBackdoorHandler;
    }
    else
    {
        pCmdHandler = mCommandHandler.at(command);
    }
    ActionStatus rc = pCmdHandler->act(aMap, aUi, commandParam, aReturnMsg);
    if (rc == ActionStatus::PARAM_REQUIRED)
    {
        aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(pCmdHandler));
        ParameterizedCommand* pParamCmd = dynamic_cast<ParameterizedCommand*>(pCmdHandler);
        if (pParamCmd)
        {
            aReturnMsg.clear();
            pParamCmd->instruction(aReturnMsg);
        }
    }
    INFO_LOG(pCmdHandler->command() + "::act() returned " + actionStatusToStr(rc));
    return rc;
}

int CommandDispatcher::addCommandHandler(CommandHandler* const aHandler)
{
    if (!aHandler)
    {
        WARN_LOG("aHandler == nullptr");
        return 1;
    }

    if (aHandler->command().find(' ') != std::string::npos)
    {
        ERROR_LOG("Command cannot contain whitespace");
        return 1;
    }

    if (mCommandHandler.count(aHandler->command()) != 0)
    {
        ERROR_LOG("Handler for " + aHandler->command() + " exists");
        return 1;
    }

    mCommandHandler.emplace(aHandler->command(), aHandler);
    mCommand.push_back(aHandler->command());
    return 0;
}

const std::vector<std::string>& CommandDispatcher::getCommands() const
{
    return mCommand;
}

const std::map<std::string, CommandHandler*>& CommandDispatcher::getHandlers() const
{
    return mCommandHandler;
}

CommandDispatcher::CommandDispatcher(std::vector<CommandHandler*> aCmdHandler)
{
    addCommandHandler(new ExitHandler());
    addCommandHandler(new HelpHandler(this));

    for (auto& handler: aCmdHandler)
    {
        addCommandHandler(handler);
    }

    if (mCommandHandler.size() != mCommand.size())
    {
        ERROR_LOG("CommandDispatcher::init failed, size of mCommand and mCommandHandler do not match");
    }
    else
    {
        INFO_LOG("Successfully initialized CommandDispatcher");
    }
}

CommandDispatcher::~CommandDispatcher()
{
    for (auto& iter : mCommandHandler)
    {
        delete iter.second;
    }
}