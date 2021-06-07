/**
 * Project: catan
 * @file command_common.hpp
 * @brief CommandHandler and StatefulCommandHandler base class (interface)
 *        Defines the APIs of the commands
 *        Stateless commands should derive from CommandHandler
 *        Commands that have multiple stages (usually required for handling parameters) should derive from StatefulCommandHandler
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_COMMON_HPP
#define INCLUDE_COMMAND_COMMON_HPP

#include <vector>
#include "common.hpp"
#include "logger.hpp"

class GameMap;
class UserInterface;

/**
 * @usage:
 * DO NOT inherit CommandHandler directly
 * Instead, inherit StatelessCommandHandler or StatefulCommandHandler and
 * implement the corresponding APIs
 * CommandHandlers other than "help", "exit" should add to CommandDispatcher constructor
 *
 * @brief:
 * CommandHandler class provides top-level abstraction to interface with CommandHelper
 * classes (CommandDispatcher and CommandParameterReader)
 * It has two derive classes, StatelessCommandHandler and StatefulCommandHandler.
 * Commands should inherit one of these two instead of directly from top-level CommandHandler
 *
 **
 * StatelessCmd
 * Stateless means it has no internal variable that record the states of the command.
 * Therefore, all parameters must pass in at once.
 * The expected behaviour of the stateless commands are,
 * based on the string parameters, figure out what to do, perform the action,
 * and return control back to the upper level.
 * Because of the differences between string paramters and mouse event, and the requirement
 * that all parameters must pass in at once, we chose to only support string parameters for
 * StatelessCmd. All commandHandlers that require mouse event must use StatefulCommand instead
 *
 **
 * StatefulCmd
 * In general, commands that require mouse event(s) should inherit StatefulCommandHandler
 * and accept paramters in a multi-stage fashion.
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
     * @param aReturnMsg stores the messages that need to return
     *
     * @brief
     * Top-level API for CommandHelper classes
     * the internal implemetaion is done by StatefulCmdHandler and StatelessCmdHandler
     * and sub-level API provided by StatefulCmdHandler and StatelessCmdHandler
     * Commands should implement the sub-level APIs provided by these two classes
     */
    virtual ActionStatus \
        run(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, \
            Point_t aPoint, std::vector<std::string>& aReturnMsg) = 0;

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
 * StatelessCommandHandler base class (interface)
 * commands that are stateless (execute and done) should inherit this class
 */
class StatelessCommandHandler : public CommandHandler
{
protected:
    /**
     * @brief The actual action to be performed
     *        derived class should implement this method
     * @param aArgs the arguments passed in
     * @param aReturnMsg - return message, instructions for user or error message
     *
     * @return PARAM_REQUIRED if parameter is expected but not found in aArgs
     *         CommandParameterReader will split the next cli input and
     *         pass in through aArgs.
     */
    virtual ActionStatus \
        statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, \
            std::vector<std::string>& aReturnMsg) = 0;
public:
    virtual ActionStatus \
        run(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, \
            Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    StatelessCommandHandler() = default;
    virtual ~StatelessCommandHandler() = default;
};

/**
 * StatefulCommandHandler base class (interface)
 * commands that requires *special* handling for parameters should inherit this class
 * and implement readParameter() method
 *
 **
 * StatefulCommandHandler model
 * when run() get called, it will first call onParameterReceive() recursively until all
 * parameter is processed.
 * Then run() will check parameterComplete(), if false, it will return PARAM_REQUIRED
 * else, it will call statefulRun() to perform the action
 * See command_common.cpp for implementation detail
 */
class StatefulCommandHandler : public CommandHandler
{
protected:
    /**
     * @param aParam - string parameter
     * @param aPoint - for mouse event, the coord on GameMap
     * @param aReturnMsg - return message, instructions for user or error message
     *
     * @return onParameterReceive should return either ActionStatus::SUCCESS or ActionStatus::FAILED
     */
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) = 0;

    /**
     * @brief This is a wrapper that use a loop to call the onParameterReceive() above.
     *        Some commands expect arguments in a key-value pair format, then they will
     *        need to override this method instead; in this case, these commands should
     *        implement a trivial "return SUCCESS" for onParameterReceive
     *        NOTE: this function does not handle mouse event, if mouse event is required
     *              commands need to implement the handling in onParameterReceive() above
     *
     * @param aArgs - string parameters
     * @param aReturnMsg - return message, instructions for user or error message
     *
     * @return onStringParametersReceive should return either ActionStatus::SUCCESS or ActionStatus::FAILED
     */
    virtual ActionStatus onStringParametersReceive(GameMap& aMap, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg);

    /**
     * @brief The actual action to be performed
     *        derived class should implement this method
     */
    virtual ActionStatus \
        statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) = 0;

    /**
     * return true when all necessary parameters are read in.
     * Then CommandHandler::statefulRun() gets called
     */
    virtual bool parameterComplete() const = 0;

public:
    virtual ActionStatus \
        run(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, \
            Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;

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

#endif /* INCLUDE_COMMAND_COMMON_HPP */
