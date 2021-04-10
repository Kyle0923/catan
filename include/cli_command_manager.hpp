/**
 * Project: catan
 * @file cli_command_manager.hpp
 * @brief process CLI user commands, and calls the corresponding command_handler function
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_CLI_COMMAND_MANAGER_HPP
#define INCLUDE_CLI_COMMAND_MANAGER_HPP

#include <vector>
#include <map>
#include "common.hpp"
#include "command_handler.hpp"

class CliCommandManager
{
private:
    bool mInitialized;
    InternalHandler* const mInternalCmdHandler;  // internal cmd handler, "backdoor"
    std::map<std::string, CommandHandler*> mCommandHandler;
    std::vector<std::string> mCommand;
    static std::vector<std::string> stringMatcher(std::string aInput, const std::vector<std::string>& aMatchPool, std::string* const aLongestCommonStr);
    int addCommandHandler(CommandHandler* const aHandler);
public:
    std::vector<std::string> commandMatcher(std::string aInput, std::string* const aLongestCommonStr = nullptr) const;
    ActionStatus act(std::string aInput, std::vector<std::string>& aInfoMsg);
    const std::vector<std::string>& getCommands() const;
    const std::map<std::string, CommandHandler*>& getHandlers() const;
    int init();
    CliCommandManager(/* args */);
    ~CliCommandManager();
};

#endif /* INCLUDE_CLI_COMMAND_MANAGER_HPP */
