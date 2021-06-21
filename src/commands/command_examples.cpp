/**
 * Project: catan
 * @file command_examples.cpp
 *
 * @brief various commands used for testing in development
 *        should not build when RELEASE=1
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef RELEASE

#include "command_handlers.hpp"

std::string SubCmdHandler::command() const
{
    return "subcmd";
}

ActionStatus SubCmdHandler::statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg)
{
    aUi.pushCommandHelper(std::make_unique<CommandDispatcher>(std::vector<CommandHandler*>({new SubCmdHandler()})));
    return ActionStatus::SUCCESS;
}

ParameterExampleCommandHandler::ParameterExampleCommandHandler() :
    mPoint(Point_t{0, 0}),
    mParam("")
{
    // empty
}

std::string ParameterExampleCommandHandler::command() const
{
    return "parameter";
}

ActionStatus ParameterExampleCommandHandler::statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg)
{
    aReturnMsg.push_back("User clicked on '" + aMap.getTerrain(mPoint)->getStringId() + "'");
    aReturnMsg.push_back("And passed in param '" + mParam + "'");
    aReturnMsg.push_back(command() + "::act() successful");
    return ActionStatus::SUCCESS;
}

ActionStatus ParameterExampleCommandHandler::onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    // first prompt user for a mouse event
    if (aParam == "" && aPoint == Point_t{0, 0})
    {
        aReturnMsg.push_back("Please click on the GameMap");
        return ActionStatus::SUCCESS;
    }
    if (mPoint == Point_t{0, 0})
    {
        DEBUG_LOG_L3("reading mouse event ", aPoint);
        mPoint = aPoint;
        aReturnMsg.push_back(Logger::formatString("Mouse clicked on ", aPoint));
        aReturnMsg.push_back("please enter param...");
        return ActionStatus::SUCCESS;
    }
    if (mParam == "")
    {
        DEBUG_LOG_L3("reading string param ", aParam);
        mParam = aParam;
        aReturnMsg.push_back("entered: " + aParam);
        return ActionStatus::SUCCESS;
    }
    return ActionStatus::SUCCESS;
}

bool ParameterExampleCommandHandler::parameterComplete() const
{
    return (mPoint != Point_t{0, 0} && mParam != "");
}

void ParameterExampleCommandHandler::resetParameters()
{
    mPoint = Point_t{0, 0};
    mParam = "";
}

void ParameterExampleCommandHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    aReturnMsg.push_back("Please first click a terrain on the map");
    aReturnMsg.push_back("then enter a string value");
}

std::string BuildingHandler::command() const
{
    return "building";
}

ActionStatus BuildingHandler::statelessRun(GameMap& aMap, UserInterface& aUi, const std::vector<std::string>& aArgs, std::vector<std::string>& aReturnMsg)
{
    for (size_t ii = 0; ii < aArgs.size(); ++ii)
    {
        aReturnMsg.emplace_back("param " + std::to_string(ii) + ": '" + aArgs.at(ii) + "'");
    }
    return ActionStatus::SUCCESS;
}

const std::vector<std::string>& BuildingHandler::paramAutoFillPool(size_t aParamIndex) const
{
    static const std::vector<std::string> param0 = {"default0"};
    static const std::vector<std::string> param1 = {"default1"};
    switch (aParamIndex)
    {
    case 0:
        return param0;
    case 1:
        return param1;
    default:
        return EMPTY_STRING_VECTOR;
    }
}

#endif /* RELEASE */
