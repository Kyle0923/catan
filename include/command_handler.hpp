/**
 * Project: catan
 * @file command_handler.hpp
 * @brief CommandHandler and StatefulCommandHandler base class (interface)
 *        Defines the APIs of the commands
 *        Stateless commands should derive from CommandHandler
 *        Commands that have multiple stages (usually required for handling parameters) should derive from StatefulCommandHandler
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
 * Command handlers except "help", "exit" need to add to CommandDispatcher constructor
 *
 * @brief:
 * CommandHandlers by default are *stateless*.
 * That is, it has no internal variables that record the states of the command.
 * The expected behaviour of the stateless commands are,
 * based on the string parameters, figure out what to do, perform the action,
 * and return control back to the upper level
 *
 * However, while it is possible (and supported) to pass multiple string parameters at once,
 * due to the nature of mouse events, it is impossible to pass string parameters and
 * mouse event(s) together at one stage. Therefore, in general, commands that require
 * mouse event(s) should inherit StatefulCommandHandler and accept paramters in a
 * multi-stage fashion.
 *
 * In addition, because StatefulCommandHandler provides an instruction() api,
 * it has a better use experience than the stateless CommandHandler.
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
     * If parameter(s) expected but not provided in aArgs, return ActionStatus::PARAM_REQUIRED
     *
     * If only *string* parameters are required, no additional effort is required
     * CommandParameterReader will split the next cli input and pass to act() through aArgs.
     *
     * If special handling for parameter(s) is required, the cmdHandler should instead
     * inherit StatefulCommandHandler and implement onParameterReceive()
     * CommandParameterReader will then call onParameterReceive instead.
     */
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) = 0;

    /**
     * for auto complete, returns a vector of possible input parameters
     * user of this command should also implement currentParamIndex() if more than 1 param is expected
     * @param aParamIndex - the index of the to-be-auto-complete parameter
     */
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const;
    /**
     * the index of current-reading parameter
     * it is used as a helper function to deduce aParamIndex in paramAutoFillPool
     */
    virtual size_t currentParamIndex() const;

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
 * StatefulCommandHandler base class (interface)
 * commands that requires *special* handling for parameters should inherit this class
 * and implement readParameter() method
 */
class StatefulCommandHandler : public CommandHandler
{
public:
    /**
     * @param aParam - string parameter
     * @param aPoint - for mouse event, the coord on GameMap
     * @param aReturnMsg - return message, instructions for user or error message
     *
     * @return onParameterReceive should return either ActionStatus::SUCCESS or ActionStatus::FAILED
     */
    virtual ActionStatus onParameterReceive(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) = 0;

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

    StatefulCommandHandler() = default;
    virtual ~StatefulCommandHandler() = default;
};

#endif /* INCLUDE_COMMAND_HANDLER_HPP */
