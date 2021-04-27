/**
 * Project: catan
 * @file cli_command_manager.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include <sstream>
#include "cli_command_manager.hpp"
#include "logger.hpp"

BackdoorHandler CliCommandManager::mBackdoorHandler;  // handles backdoor commands

std::vector<std::string> CliCommandManager::stringMatcher(std::string aInput, const std::vector<std::string>& aMatchPool, std::string* const aLongestCommonStr = nullptr)
{
    if (aInput.length() == 0)
    {
        return {}; //return empty
    }
    std::vector<std::string> matched;
    std::string shortestStr;
    for (const std::string& matchingString : aMatchPool)
    {
        bool found = false;
        if (matchingString.length() > aInput.size())
        {
            if (matchingString.find(aInput) == 0)
            {
                // cmd starts with aInput, partial match
                found = true;
            }
        }
        else
        {
            if (aInput.find(matchingString) == 0) // aInput includes matchingString
            {
                if (aInput.size() == matchingString.size() || aInput.at(matchingString.size()) == ' ')
                {
                    // either aInput is extra the same as matchingString or aInput has a whitespace terminating the command
                    // to prevent "helpX" matching "help"
                    found = true;
                }
            }
        }

        if (found)
        {
            matched.push_back(matchingString);
            if (matchingString.size() < shortestStr.size() || shortestStr.size() == 0)
            {
                shortestStr = matchingString;
            }
        }
    }
    DEBUG_LOG_L0("stringMatcher matched: ", matched);

    // if aLongestCommonStr is provided, return the longest common string back via aLongestCommonStr
    if (aLongestCommonStr)
    {
        *aLongestCommonStr = aInput;
        if (matched.size() == 1 && matched.at(0).length() > aInput.size())
        {
            *aLongestCommonStr = matched.at(0);
        }
        else if (matched.size() > 1)
        {
            std::string longestCommonSubstring;
            // find the longest common string
            for (size_t ii = aInput.length() - 1; ii < shortestStr.size(); ++ii)
            {
                char c = matched.at(0).at(ii); // the ii-th char of the first matched string
                bool notmatch = false;
                for (size_t jj = 1; jj < matched.size(); ++jj)
                {
                    const std::string& str = matched.at(jj);
                    DEBUG_LOG_L0("cmp: ", str.at(ii), " & ", c);
                    if (str.at(ii) != c)
                    {
                        notmatch = true;
                        break;
                    }
                    if (jj == matched.size() - 1)
                    {
                        // all matched
                        longestCommonSubstring = matched.at(0).substr(0, ii + 1);
                    }
                }
                if (notmatch)
                {
                    break;
                }
            }
            *aLongestCommonStr = longestCommonSubstring;
        }
    }
    return matched;
}

std::vector<std::string> CliCommandManager::commandMatcher(std::string aInput, std::string* const aLongestCommonStr) const
{
    return stringMatcher(aInput, mCommand, aLongestCommonStr);
}

ActionStatus CliCommandManager::act(GameMap& aMap, UserInterface& aUi, std::string aInput, std::vector<std::string>& aReturnMsg)
{
    std::string command;
    if (aInput.find(mBackdoorHandler.command() + " ") == 0)
    {
        command = mBackdoorHandler.command();
    }
    else
    {
        std::vector<std::string> matchingCommand = commandMatcher(aInput);
        if (matchingCommand.size() != 1)
        {
            INFO_LOG("Unknown command: \'" + aInput + '\'');
            aReturnMsg.push_back("Unknown command: \'" + aInput + '\'');
            return ActionStatus::SUCCESS;
        }
        command = matchingCommand.front();
    }

    if (command.length() > aInput.length())
    {
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
        std::string temp;
        std::istringstream paramStream(params);
        while (getline(paramStream, temp, ' '))
        {
            if (temp != "")
            {
                commandParam.push_back(temp);
            }
        }
    }
    INFO_LOG("calling handler for cmd \'" + command + "\', arg: ", commandParam);
    if (command == mBackdoorHandler.command())
    {
        return mBackdoorHandler.act(aMap, aUi, commandParam, aReturnMsg);
    }
    else
    {
        return mCommandHandler.at(command)->act(aMap, aUi, commandParam, aReturnMsg);
    }
}

int CliCommandManager::addCommandHandler(CommandHandler* const aHandler)
{
    if (!aHandler)
    {
        WARN_LOG("aHandler == nullptr");
        return 1;
    }
    else
    {
        mCommandHandler.emplace(aHandler->command(), aHandler);
        mCommand.push_back(aHandler->command());
        return 0;
    }
}

const std::vector<std::string>& CliCommandManager::getCommands() const
{
    return mCommand;
}

const std::map<std::string, CommandHandler*>& CliCommandManager::getHandlers() const
{
    return mCommandHandler;
}

CliCommandManager::CliCommandManager(std::vector<CommandHandler*> aCmdHandler)
{
    addCommandHandler(new ExitHandler());
    addCommandHandler(new QuitHandler());
    addCommandHandler(new HelpHandler(this));

    for (auto& handler: aCmdHandler)
    {
        addCommandHandler(handler);
    }

    if (mCommandHandler.size() != mCommand.size())
    {
        ERROR_LOG("CliCommandManager::init failed, size of mCommand and mCommandHandler do not match");
    }
    else
    {
        INFO_LOG("Successfully initialized CliCommandManager");
    }
}

CliCommandManager::~CliCommandManager()
{
    for (auto& iter : mCommandHandler)
    {
        delete iter.second;
    }
}