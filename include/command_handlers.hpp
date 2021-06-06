/**
 * Project: catan
 * @file command_handlers.hpp
 * @brief declaration of the common commands (quit, exit, help)
 *        and other individual commands
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_COMMAND_HANDLERS_HPP
#define INCLUDE_COMMAND_HANDLERS_HPP

#include "command_dispatcher.hpp"
#include "command_common.hpp"
#include "game_map.hpp"
#include "user_interface.hpp"
#include "utility.hpp"
#include "logger.hpp"

class BackdoorHandler: public StatelessCommandHandler
{
protected:
    ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
public:
    std::string command() const override final;
};

class ExitHandler: public StatelessCommandHandler
{
protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
};

class HelpHandler: public StatelessCommandHandler
{
private:
    CommandDispatcher* const mDispatcher;
protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    HelpHandler(CommandDispatcher* const aDispatcher);
};

class NextHandler: public StatelessCommandHandler
{
protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
public:
    virtual std::string command() const override;
    virtual std::string description() const override final;
};
class PassHandler: public NextHandler // alias of NextHandler
{
    virtual std::string command() const override final;
};

class BuildHandler: public StatefulCommandHandler
{
protected:
    Point_t mPoint;
    std::string mBuildType;
    const static std::vector<std::string> mBuildTypeMatchingPool; // possible values of mBuildType
protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;

    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    BuildHandler();
};

// for road_building development_card
class RoadBuildingHandler: public StatefulCommandHandler
{
private:
    size_t mRoadCounter;
protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
public:
    virtual std::string command() const override final;

    virtual void resetParameters() override;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    RoadBuildingHandler();
};

// for year_of_plenty development_card
class YearOfPlentyHandler: public StatefulCommandHandler
{
private:
    ResourceTypes mResource1;
    ResourceTypes mResource2;
    const static std::vector<std::string>& mResourceTypeMatchingPool; // possible values of mBuildType
protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
public:
    virtual std::string command() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
    virtual size_t currentParamIndex() const override final;

    virtual void resetParameters() override;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    YearOfPlentyHandler();
};

// for monopoly development_card
class MonopolyHandler: public StatefulCommandHandler
{
private:
    ResourceTypes mResource;
protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
public:
    virtual std::string command() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;

    virtual void resetParameters() override;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    MonopolyHandler();
};

// Singleton object
// should only be used by RollHandler, DevelopmentCardHandler
class RobberMoveHandler: public StatefulCommandHandler
{
private:
    Point_t mRobberDestination;
    Point_t mRobbingVertex;
    RobberMoveHandler();
protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
public:
    virtual std::string command() const override final;

    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;
    virtual void resetParameters() override final;

    static RobberMoveHandler& getRobberHandler();
};

class RollHandler: public StatelessCommandHandler
{
private:
    RobberMoveHandler& mRobberMoveHandler;
protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    RollHandler();
};

class DevelopmentCardHandler: public StatefulCommandHandler
{
private:
    std::string mAction;
    int mDevCard;
    RobberMoveHandler& mRobberMoveHandler;
    RoadBuildingHandler mRoadBuilder;
    YearOfPlentyHandler mYearOfPlentyHandler;
    MonopolyHandler mMonopolyHandler;
    const static std::vector<std::string> mActionPool;
    const static std::vector<std::string> mPlayableDevCard;
protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
    virtual size_t currentParamIndex() const override final;

    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    DevelopmentCardHandler();
};

class StatusHandler: public StatelessCommandHandler
{
protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
};

class FirstTwoRoundHandler: public StatefulCommandHandler
{
private:
    size_t mRound;  // round 1 or 2
    size_t mIndex;  // current index of mOrder
    Point_t mSettelment;
    Point_t mRoad;
    const std::vector<int> mOrder;
    std::unique_ptr<CommandHelper> mTopLevelCmdDispatcher;  // when first two rounds is finished, push this to UI cmdHelper stack
protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;

public:
    virtual std::string command() const override final;

    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    FirstTwoRoundHandler(const std::vector<int>& aOrder, std::unique_ptr<CommandHelper> aCmdDispatcher);
};

class TradeHandler: public StatefulCommandHandler
{
private:
    static const std::vector<std::string> mOffereeMatchingPool;
    std::string mOfferee;
    bool mOfferComposed;
    Offer_t mOutgoingOffer;
    std::vector<Offer_t> mIncomingOffer;

protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onStringParametersReceive(GameMap& aMap, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;

    // not used
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;

public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
    virtual size_t currentParamIndex() const override final;

    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    TradeHandler();
};

class TradeAnswerHandler: public StatefulCommandHandler
{
private:
    bool mOfferComposed;
    Offer_t mIncomingOffer;

protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onStringParametersReceive(GameMap& aMap, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;

    // not used
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;

public:
    virtual std::string command() const override final;
    virtual std::string description() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
    virtual size_t currentParamIndex() const override final;

    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    TradeAnswerHandler();
};

////////////////////////////////////////////////////////////////////////////////////
// the commands below are meant to be used for testing in development.
// they should not be built when RELEASE=1
////////////////////////////////////////////////////////////////////////////////////
#ifndef RELEASE

class SubCmdHandler: public StatelessCommandHandler
{
protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
public:
    virtual std::string command() const override final;
};

class ParameterExampleCommandHandler: public StatefulCommandHandler
{
private:
    Point_t mPoint;
    std::string mParam;
protected:
    virtual ActionStatus statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg) override final;
    virtual ActionStatus onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg) override final;
    virtual bool parameterComplete() const override final;
public:
    virtual std::string command() const override final;

    virtual void resetParameters() override final;
    virtual void instruction(std::vector<std::string>& aReturnMsg) const override final;

    ParameterExampleCommandHandler();
};

class BuildingHandler: public StatelessCommandHandler
{
    // test auto complete for when one command if a substring of another
    // e.g., "build" and "building"
public:
    virtual std::string command() const override final;
    virtual const std::vector<std::string>& paramAutoFillPool(size_t aParamIndex) const override final;
protected:
    virtual ActionStatus statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg) override final;
};

#endif /* RELEASE */

#endif /* INCLUDE_COMMAND_HANDLERS_HPP */