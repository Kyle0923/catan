/**
 * Project: catan
 * @file command_common.hpp
 * @brief declaration of the common commands (quit, exit, help)
 *        and other individual commands
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_COMMON_HPP
#define INCLUDE_COMMAND_COMMON_HPP

#include "command_dispatcher.hpp"
#include "command_handler.hpp"
#include "game_map.hpp"
#include "user_interface.hpp"
#include "utility.hpp"
#include "logger.hpp"

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
    CommandDispatcher* const mDispatcher;
public:
    virtual std::string command() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
    HelpHandler(CommandDispatcher* const aDispatcher);
};

#ifndef RELEASE

class SubCmdHandler: public CommandHandler
{
    virtual std::string command() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

class ParameterExampleCommandHandler: public CommandHandler, public ParameterizedCommand
{
private:
    Point_t mPoint;
    std::string mParam;
public:
    virtual std::string command() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;

    virtual ActionStatus processParameter(std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    ParameterExampleCommandHandler();
    virtual ~ParameterExampleCommandHandler() = default;

};

#endif

#endif /* INCLUDE_COMMAND_COMMON_HPP */