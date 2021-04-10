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

/**
 *  @usage:
 * every command handler need to add to int CliCommandManager::init()
 */

enum class ActionStatus
{
    SUCCESS = 0,
    INCOMPLETE,  // incomplete command, do not remove current cli
    FAILED,      // internal error, write to log
};

class CliCommandManager; // forward declare CliCommandManager

class BackdoorHandler
{
public:
    std::string command() const;
    ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo);
};

class CommandHandler
{
public:
    virtual std::string command() const = 0;
    virtual std::string description() const;
    virtual ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo) = 0;
    CommandHandler() = default;
    virtual ~CommandHandler() = default;
};

class ExitHandler: public CommandHandler
{
    virtual std::string command() const override final;
    virtual ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo) override final;
};

class QuitHandler: public CommandHandler
{
    virtual std::string command() const override final;
    virtual ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo) override final;
};

class HelpHandler: public CommandHandler
{
private:
    CliCommandManager* const mManager;
public:
    virtual std::string command() const override final;
    virtual ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo) override final;
    HelpHandler(CliCommandManager* const aManager);
};

#endif /* INCLUDE_COMMAND_HANDLER_HPP */
