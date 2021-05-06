/**
 * Project: catan
 * @file build_handler.cpp
 *
 * @brief handle player build settlement/city and road
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_common.hpp"

BuildHandler::BuildHandler() :
    mPoint(Point_t{0, 0}),
    mParam("")
{
    // empty
}

std::string BuildHandler::command() const
{
    return "build";
}

ActionStatus BuildHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    if (aArgs.size() != 0)
    {
        processParameter(aMap, aArgs.front(), Point_t{0, 0}, aReturnMsg);
    }
    if (!parameterComplete())
    {
        return ActionStatus::PARAM_REQUIRED;
    }

    // TODO:
    // mParam == road ? settlement ? city ?
    // aMap::buildable(currentUser) ? false -> aReturnMsg.push("insufficient resource")
    // true -> aMap::build(Point_t), aResturnMsg.push("done")
    return ActionStatus::SUCCESS;
}

ActionStatus BuildHandler::processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    // first prompt user for build type: road or settlement or city
    if (mParam == "")
    {
        INFO_LOG(command() + " reading parameter " + aParam);
        if (aParam == "road" || aParam == "road" || aParam == "city")
        {
            mParam = aParam;
            instruction(aReturnMsg);
            return ActionStatus::SUCCESS;
        }
        else
        {
            instruction(aReturnMsg);
            return ActionStatus::FAILED;
        }
    }

    if (mPoint == Point_t{0, 0})
    {
        INFO_LOG(command() + "reading mouse event ", aPoint);
        if (mParam == "road")
        {
            if (aMap.isTerrain<Edge>(aPoint))
            {
                mPoint = aPoint;
                return ActionStatus::SUCCESS;
            }
            else
            {
                aReturnMsg.push_back("Player clicked is not an edge");
                instruction(aReturnMsg);
                return ActionStatus::FAILED;
            }
        }
        else if (mParam == "settlement" || mParam == "city" )
        {
            if (aMap.isTerrain<Vertex>(aPoint))
            {
                mPoint = aPoint;
                return ActionStatus::SUCCESS;
            }
            else
            {
                aReturnMsg.push_back("Player clicked is not a vertex");
                instruction(aReturnMsg);
                return ActionStatus::FAILED;
            }
        }
    }

    return ActionStatus::SUCCESS;
}

bool BuildHandler::parameterComplete() const
{
    return (mPoint != Point_t{0, 0} && mParam != "");
}

void BuildHandler::resetParameters()
{
    mPoint = Point_t{0, 0};
    mParam = "";
}

void BuildHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    if (mParam == "" && mPoint == Point_t{0, 0})
    {
        aReturnMsg.push_back("Please first type in what you want to build: road, settlement, or city");
        aReturnMsg.push_back("Then click on the map where you want to build");
        return;
    }
    else if (mParam != "" && mPoint == Point_t{0, 0})
    {
        aReturnMsg.push_back("You want to build a " + mParam);
        aReturnMsg.push_back("Please click on the map where you want to build");
        return;
    }
}