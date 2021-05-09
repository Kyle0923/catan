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

class BackdoorHandler: public CommandHandler
{
public:
    std::string command() const override final;
    ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

class ExitHandler: public CommandHandler
{
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

class HelpHandler: public CommandHandler
{
private:
    CommandDispatcher* const mDispatcher;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
    HelpHandler(CommandDispatcher* const aDispatcher);
};

class NextHandler: public CommandHandler
{
    virtual std::string command() const override;
    virtual std::string description() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

// alias of NextHandler
class PassHandler: public NextHandler
{
    virtual std::string command() const override final;
};

class BuildHandler: public CommandHandler, public ParameterizedCommand
{
private:
    Point_t mPoint;
    std::string mBuildType;
    const static std::vector<std::string> mBuildTypeMatchingPool; // possible values of mBuildType
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;

    virtual ActionStatus processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    BuildHandler();
};

// RobberMoveHandler should only be used by RollHandler
class RobberMoveHandler: public CommandHandler, public ParameterizedCommand
{
private:
    Point_t mRobberDestination;
    Point_t mRobbingVertex;
public:
    virtual std::string command() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;

    virtual ActionStatus processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    RobberMoveHandler();
};

class RollHandler: public CommandHandler
{
private:
    RobberMoveHandler mRobberMoveHandler;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

class DevelopmentCardHandler: public CommandHandler, public ParameterizedCommand
{
private:
    std::string mAction;
    int mDevCard;
    RobberMoveHandler mRobberMoveHandler;
    const static std::vector<std::string> mActionPool;
    const static std::vector<std::string> mPlayableDevCard;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
    virtual size_t currentParamIndex() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;

    virtual ActionStatus processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    DevelopmentCardHandler();
};

class StatusHandler: public CommandHandler
{
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
};

////////////////////////////////////////////////////////////////////////////////////
// the commands below are meant to be used for testing in development.
// they should not be built when RELEASE=1
////////////////////////////////////////////////////////////////////////////////////
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

    virtual ActionStatus processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    ParameterExampleCommandHandler();
};

class BuildingHandler: public CommandHandler
{
    // test auto complete for when one command if a substring of another
    // e.g., "build" and "building"
    virtual std::string command() const override final;
    virtual ActionStatus act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg) override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
};

#endif /* RELEASE */

#endif /* INCLUDE_COMMAND_COMMON_HPP */