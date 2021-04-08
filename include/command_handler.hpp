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

enum class ActionStatus
{
    SUCCESS = 0,
    NO_ACTION,  // "SUCCESS" without doing anything
    INFO,       // print aInfo returned by CommandHandler::act
    FAILED,     // internal error, write to log
};

class CliCommandManager; // forward declare CliCommandManager

class CommandHandler
{
public:
    virtual std::string command() = 0;
    virtual ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo) = 0;
    CommandHandler() = default;
    virtual ~CommandHandler() = default;
};

class ExitHandler: public CommandHandler
{
    virtual std::string command() override final;
    virtual ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo) override final;
};

class QuitHandler: public CommandHandler
{
    virtual std::string command() override final;
    virtual ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo) override final;
};

class HelpHandler: public CommandHandler
{
private:
    CliCommandManager* const mManager;
public:
    virtual std::string command() override final;
    virtual ActionStatus act(std::vector<std::string> aArgs, std::vector<std::string>& aInfo) override final;
    HelpHandler(CliCommandManager* const aManager);
};

#endif /* INCLUDE_COMMAND_HANDLER_HPP */
