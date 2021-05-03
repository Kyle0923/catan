/**
 * Project: catan
 * @file command_handler.hpp
 * @brief CommandHandler and ParameterizedCommand base class (interface)
 *        All commands must be derived from CommandHandler
 *        The commands that require parameter handling should also inherit ParameterizedCommand
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_HANDLER_HPP
#define INCLUDE_COMMAND_HANDLER_HPP

#include <vector>
#include "common.hpp"
#include "logger.hpp"

class GameMap;
class UserInterface;

/**
 *  @usage:
 * command handlers except "help", "exit" & "quit" need to add to CommandDispatcher constructor
 */

class CommandDispatcher; // forward declare CommandDispatcher

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

/**
 * ParameterizedCommand base class (interface)
 * commands that requires parameter should inherit this class
 * and implement readParameter() method
 */
class ParameterizedCommand
{
public:
    /**
     * @param aParam - string parameter
     * @param aPoint - for mouse event, the coord on GameMap
     * @param aReturnMsg - return message, instructions for user or error message
     */
    virtual ActionStatus processParameter(std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) = 0;

    /**
     * return true when all necessary parameters are read in.
     * then readParameter() will call CommandHandler::act() to trigger the commandHandler
     */
    virtual bool parameterComplete() const = 0;

    /**
     * provide instructions to user
     */
    virtual void instruction(std::vector<std::string>& aReturnMsg) const = 0;

    ParameterizedCommand() = default;
    virtual ~ParameterizedCommand() = default;
};

#endif /* INCLUDE_COMMAND_HANDLER_HPP */
