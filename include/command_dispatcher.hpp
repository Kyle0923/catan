/**
 * Project: catan
 * @file command_dispatcher.hpp
 * @brief process CLI user commands, and calls the corresponding command_handler function
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_DISPATCHER_HPP
#define INCLUDE_COMMAND_DISPATCHER_HPP

#include <vector>
#include <map>
#include "common.hpp"
#include "command_handler.hpp"

class GameMap;
class UserInterface;

class CommandDispatcher
{
private:
    static BackdoorHandler mBackdoorHandler;  // handles backdoor commands
    std::map<std::string, CommandHandler*> mCommandHandler;
    std::vector<std::string> mCommand;
    static std::vector<std::string> stringMatcher(std::string aInput, const std::vector<std::string>& aMatchPool, std::string* const aLongestCommonStr);
    int addCommandHandler(CommandHandler* const aHandler);
public:
    std::vector<std::string> commandMatcher(std::string aInput, std::string* const aLongestCommonStr = nullptr) const;
    ActionStatus act(GameMap& aMap, UserInterface& aUi, std::string aInput, std::vector<std::string>& aReturnMsg);
    const std::vector<std::string>& getCommands() const;
    const std::map<std::string, CommandHandler*>& getHandlers() const;

    /**
     * the memory handling of CommandHandler* should be handled by Dispatcher
     * help, quit, exit handlers will be added automatically
     */
    CommandDispatcher(std::vector<CommandHandler*> aCmdHandler);
    ~CommandDispatcher();
};

#endif /* INCLUDE_COMMAND_DISPATCHER_HPP */
