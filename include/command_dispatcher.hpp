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
#include "command_helper.hpp"
#include "command_common.hpp"

class BackdoorHandler; // forward declaration of BackdoorHandler

class CommandDispatcher : public CommandHelper
{
private:
    static BackdoorHandler mBackdoorHandler;  // handles backdoor commands
    std::map<std::string, CommandHandler*> mCommandHandlers;
    std::vector<std::string> mCommands;
    int addCommandHandler(CommandHandler* const aHandler);
public:
    virtual std::vector<std::string> getPossibleInputs(const std::string& aInput, std::string* const aAutoFillString = nullptr) const override;

    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::string aInput, \
                              Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;

    const std::vector<std::string>& getCommands() const;
    const std::map<std::string, CommandHandler*>& getHandlers() const;

    /**
     * the memory handling of CommandHandler* should be handled by Dispatcher
     * help, next, exit handlers will be added automatically
     */
    CommandDispatcher(std::vector<CommandHandler*> aCmdHandler);
    virtual ~CommandDispatcher();
};

#endif /* INCLUDE_COMMAND_DISPATCHER_HPP */
