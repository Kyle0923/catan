/**
 * Project: catan
 * @file parameter_example.cpp
 *
 * @brief a example to use parameterized_command
 *        should not build when RELEASE=1
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef RELEASE

#include "command_common.hpp"

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

ActionStatus ParameterExampleCommandHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    if (!parameterComplete())
    {
        return ActionStatus::PARAM_REQUIRED;
    }
    aReturnMsg.push_back("User clicked on '" + aMap.getTerrain(mPoint)->getStringId() + "'");
    aReturnMsg.push_back("And passed in param '" + mParam + "'");
    aReturnMsg.push_back(command() + "::act() successful");
    return ActionStatus::SUCCESS;
}

ActionStatus ParameterExampleCommandHandler::processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
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
    return ActionStatus::FAILED;
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

#endif /* RELEASE */
