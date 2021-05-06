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

    bool enoughResource = false;
    int rc = 1;
    if (mParam == "road")
    {
        enoughResource = aMap.playerHasResourceForRoad();
        if (enoughResource)
        {
            rc = aMap.buildRoad(mPoint);
        }
    }
    else if (mParam == "settlement")
    {
        enoughResource = aMap.playerHasResourceForSettlement();
        if (enoughResource)
        {
            rc = aMap.buildColony(mPoint, ColonyType::SETTLEMENT);
        }
    }
    else if (mParam == "city")
    {
        enoughResource = aMap.playerHasResourceForCity();
        if (enoughResource)
        {
            rc = aMap.buildColony(mPoint, ColonyType::CITY);
        }
    }

    if (!enoughResource)
    {
        aReturnMsg.push_back("current player has insufficient resource");
        return ActionStatus::SUCCESS;
    }

    if (rc == 2)
    {
        aReturnMsg.push_back(Logger::formatString("Incorrect owner of ", mPoint));
    }
    else if (rc == 0)
    {
        INFO_LOG("Successfully build " + mParam + " for Player#", aMap.currentPlayer());
        aReturnMsg.push_back("Construction complete");
    }

    return ActionStatus::SUCCESS;
}

ActionStatus BuildHandler::processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    // first prompt user for build type: road or settlement or city
    if (mParam == "")
    {
        INFO_LOG(command() + " reading parameter " + aParam);
        if (aParam == "road" || aParam == "settlement" || aParam == "city")
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