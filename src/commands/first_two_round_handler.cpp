/**
 * Project: catan
 * @file first_two_round_handler.cpp
 *
 * @brief handle first two rounds, each player will place their first two settlements
 *        and the resources next to the second settlement will be given to each player
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_handlers.hpp"
#include "command_parameter_reader.hpp"

FirstTwoRoundHandler::FirstTwoRoundHandler(const std::vector<int>& aOrder, std::unique_ptr<CommandHelper> aCmdDispatcher) :
    mRound(1U),
    mIndex(0U),
    mSettelment(Point_t{0, 0}),
    mRoad(Point_t{0, 0}),
    mOrder(aOrder),
    mTopLevelCmdDispatcher(std::move(aCmdDispatcher))
{
    // empty
}

std::string FirstTwoRoundHandler::command() const
{
    return "first_two_round_handler";
}

ActionStatus FirstTwoRoundHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    for (std::string arg : aArgs)
    {
        onParameterReceive(aMap, arg, Point_t{0, 0}, aReturnMsg);
    }

    if (!parameterComplete())
    {
        return ActionStatus::PARAM_REQUIRED;
    }

    return ActionStatus::SUCCESS;
}

ActionStatus FirstTwoRoundHandler::onParameterReceive(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    return ActionStatus::SUCCESS;
}

bool FirstTwoRoundHandler::parameterComplete() const
{
    return (mRound == 2 && mIndex == 0);
}

void FirstTwoRoundHandler::resetParameters()
{
    mSettelment = Point_t{0, 0};
    mRoad = Point_t{0, 0};
}

void FirstTwoRoundHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    //TODO:
}
