#include <sstream>
#include "cli_command_manager.hpp"
#include "logger.hpp"

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
        if (matchingString.find(aInput) == 0) // starting with aInput
        {
            found = true;
        }
        else if (aInput.find(matchingString) == 0) // aInput with matchingString
        {
            found = true;
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
    if (!mInitialized)
    {
        ERROR_LOG("CliCommandManager not initialized");
    }
    return stringMatcher(aInput, mCommand, aLongestCommonStr);
}

ActionStatus CliCommandManager::act(std::string aInput, std::vector<std::string>& aInfoMsg)
{
    if (!mInitialized)
    {
        ERROR_LOG("CliCommandManager not initialized");
    }
    std::vector<std::string> matchingCommand = commandMatcher(aInput);
    if (matchingCommand.size() == 0)
    {
        aInfoMsg.push_back("Unknown command: \'" + aInput + '\'');
        return ActionStatus::INFO;
    }
    else if (matchingCommand.size() > 1)
    {
        // too much command
        return ActionStatus::NO_ACTION;
    }

    std::string& command = matchingCommand.at(0);
    // parse aInput, extra word are split by whitespace and pass to command_handler as parameter
    size_t pos = aInput.find(command);
    std::vector<std::string> commandParam = {};
    if (pos != std::string::npos)
    {
        std::string params = aInput.substr(pos);
        std::string temp;
        std::istringstream paramStream(params);
        while (getline(paramStream, temp, ' '))
        {
            commandParam.push_back(temp);
        }
    }
    INFO_LOG("calling handler for cmd \'" + aInput + '\'');
    return mCommandAction.at(command)->act(commandParam, aInfoMsg);
}

int CliCommandManager::init()
{
    addCommandHandler(new ExitHandler());
    addCommandHandler(new QuitHandler());
    addCommandHandler(new HelpHandler(this));

    for (auto iter = mCommandAction.begin(); iter != mCommandAction.end(); ++iter)
    {
        mCommand.push_back(iter->first);
    }

    if (mCommandAction.size() != mCommand.size())
    {
        ERROR_LOG("CliCommandManager::init failed, size of mCommand and mCommandAction do not match");
    }
    else
    {
        INFO_LOG("Successfully initialized CliCommandManager");
    }
    mInitialized = true;
    return 0;
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
        mCommandAction.emplace(aHandler->command(), aHandler);
        return 0;
    }
}

const std::vector<std::string>& CliCommandManager::getCommands() const
{
    return mCommand;
}

CliCommandManager::CliCommandManager(/* args */) : mInitialized(false)
{
}

CliCommandManager::~CliCommandManager()
{
}