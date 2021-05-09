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

std::vector<std::string> CommandDispatcher::getPossibleInputs(const std::string& aInput, std::string* const aAutoFillString) const
{
    std::vector<std::string> inputBreakdown = splitString(aInput);
    const std::string& command = inputBreakdown.front();

    if (inputBreakdown.size() > 1)
    {
        if (mCommandHandlers.count(command) == 1)
        {
            // command matched, try to match the last param
            return stringMatcher(inputBreakdown.back(), mCommandHandlers.at(command)->paramAutoFillPool(inputBreakdown.size() - 2), aAutoFillString);
        }
    }
    else if (inputBreakdown.size() == 1)
    {
        // handling partial command, e.g., provide a match for "exit" when aInput is "exi"
        // only make senses when inputBreakdown is of size 1
        return stringMatcher(command, mCommands, aAutoFillString);
    }
    return {};
}

ActionStatus CommandDispatcher::act(GameMap& aMap, UserInterface& aUi, std::string aInput, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    if (aInput == "")
    {
        return ActionStatus::SUCCESS;
    }

    std::string command = aInput.substr(0, aInput.find(' '));

    if (command != mBackdoorHandler.command() && mCommandHandlers.count(command) != 1)
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
        pCmdHandler = mCommandHandlers.at(command);
    }
    ActionStatus rc = pCmdHandler->act(aMap, aUi, commandParam, aReturnMsg);
    ParameterizedCommand* pParamCmd = dynamic_cast<ParameterizedCommand*>(pCmdHandler);
    if (rc == ActionStatus::PARAM_REQUIRED)
    {
        aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(pCmdHandler));
        if (pParamCmd)
        {
            pParamCmd->instruction(aReturnMsg);
        }
    }
    else if (pParamCmd)
    {
        // reset parameters for ParameterizedCommand
        pParamCmd->resetParameters();
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

    if (mCommandHandlers.count(aHandler->command()) != 0)
    {
        ERROR_LOG("Handler for " + aHandler->command() + " exists");
        return 1;
    }

    mCommandHandlers.emplace(aHandler->command(), aHandler);
    mCommands.emplace_back(aHandler->command());
    return 0;
}

const std::vector<std::string>& CommandDispatcher::getCommands() const
{
    return mCommands;
}

const std::map<std::string, CommandHandler*>& CommandDispatcher::getHandlers() const
{
    return mCommandHandlers;
}

CommandDispatcher::CommandDispatcher(std::vector<CommandHandler*> aCmdHandler)
{
    addCommandHandler(new ExitHandler());
    addCommandHandler(new HelpHandler(this));

    for (auto& handler: aCmdHandler)
    {
        addCommandHandler(handler);
    }

    if (mCommandHandlers.size() != mCommands.size())
    {
        ERROR_LOG("CommandDispatcher::init failed, size of mCommands and mCommandHandlers do not match");
    }
    else
    {
        INFO_LOG("Successfully initialized CommandDispatcher");
    }
}

CommandDispatcher::~CommandDispatcher()
{
    for (auto& iter : mCommandHandlers)
    {
        delete iter.second;
    }
}