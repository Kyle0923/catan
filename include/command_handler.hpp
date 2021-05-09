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
 * @usage:
 * command handlers except "help", "exit" & "quit" need to add to CommandDispatcher constructor
 */

class CommandHandler
{
public:

    /**
     * returns an unique string identifier for matching
     */
    virtual std::string command() const = 0;

    /**
     * description for 'help' command
     */
    virtual std::string description() const;

    /**
     * handling of the command.
     * @param aReturnMsg stores the messages that need to return
     *
     * If parameter(s) expected and not provided in aArgs, return ActionStatus::PARAM_REQUIRED
     *
     * If simple string parameter(s) is required, no additional effort is required
     * CommandParameterReader will split the next cli input and pass to act() through aArgs.
     *
     * If special handling for parameter(s) is required, the cmdHandler need to also
     * inherit ParameterizedCommand and implement processParameter()
     * CommandParameterReader will then call processParameter instead.
     */
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) = 0;

    /**
     * for auto complete, returns a vector of possible input parameters
     * @param aParamIndex - the index of the to-be-auto-complete parameter
     */
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const;

    CommandHandler() = default;
    virtual ~CommandHandler() = default;

protected:
    /**
     * an empty std::string vector, paramAutoFillPool() returns const reference to reduce copying
     * this empty vector is in static memory, so it is return-able as a reference
     */
    static const std::vector<std::string> mEmptyVector;
};

/**
 * ParameterizedCommand base class (interface)
 * commands that requires *special* handling for parameters should inherit this class
 * and implement readParameter() method
 */
class ParameterizedCommand
{
public:
    /**
     * @param aParam - string parameter
     * @param aPoint - for mouse event, the coord on GameMap
     * @param aReturnMsg - return message, instructions for user or error message
     *
     * @return processParameter should return either ActionStatus::SUCCESS or ActionStatus::FAILED
     */
    virtual ActionStatus processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) = 0;

    /**
     * return true when all necessary parameters are read in.
     * then readParameter() will call CommandHandler::act() to trigger the commandHandler
     */
    virtual bool parameterComplete() const = 0;

    /**
     * reset all internal parameters
     */
    virtual void resetParameters() = 0;

    /**
     * provide instructions to user
     */
    virtual void instruction(std::vector<std::string>& aReturnMsg) const = 0;

    ParameterizedCommand() = default;
    virtual ~ParameterizedCommand() = default;
};

#endif /* INCLUDE_COMMAND_HANDLER_HPP */
