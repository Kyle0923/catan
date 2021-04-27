/**
 * Project: catan
 * @file command_handler.hpp
 * @brief command command_handler base class (interface) and derive classes
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_HANDLER_HPP
#define INCLUDE_COMMAND_HANDLER_HPP

#include <vector>
#include "common.hpp"

class GameMap;
class UserInterface;

/**
 *  @usage:
 * command handlers except "help", "exit" & "quit" need to add to int CliCommandManager constructor
 */

class CliCommandManager; // forward declare CliCommandManager

class BackdoorHandler
{
public:
    std::string command() const;
    ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg);
};

class CommandHandler
{
public:
    virtual std::string command() const = 0;
    virtual std::string description() const;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) = 0;
    CommandHandler() = default;
    virtual ~CommandHandler() = default;
};

class ExitHandler: public CommandHandler
{
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

class QuitHandler: public CommandHandler
{
    virtual std::string command() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

class HelpHandler: public CommandHandler
{
private:
    CliCommandManager* const mManager;
public:
    virtual std::string command() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
    HelpHandler(CliCommandManager* const aManager);
};

#ifndef RELEASE

class SubCmdHandler: public CommandHandler
{
    virtual std::string command() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

#endif

#endif /* INCLUDE_COMMAND_HANDLER_HPP */
