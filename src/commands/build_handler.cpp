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

const std::vector<std::string> BuildHandler::mBuildTypeMatchingPool = {"road", "settlement", "city"};

BuildHandler::BuildHandler() :
    mPoint(Point_t{0, 0}),
    mBuildType("")
{
    // empty
}

std::string BuildHandler::command() const
{
    return "build";
}

std::vector<std::string> BuildHandler::paramAutoFillPool() const
{
    if (mBuildType == "")
    {
        return mBuildTypeMatchingPool;
    }
    return {};
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

    int rc = -1;
    if (mBuildType == "road")
    {
        rc = aMap.buildRoad(mPoint);
    }
    else if (mBuildType == "settlement")
    {
        rc = aMap.buildColony(mPoint, ColonyType::SETTLEMENT);
    }
    else if (mBuildType == "city")
    {
        rc = aMap.buildColony(mPoint, ColonyType::CITY);
    }

    switch (rc)
    {
        case 0:
            INFO_LOG("Successfully build " + mBuildType + " for Player#", aMap.currentPlayer());
            aReturnMsg.push_back("Construction complete");
            break;
        case 1:
            aReturnMsg.push_back("Incorrect coordinate");
            break;
        case 2:
            aReturnMsg.push_back(Logger::formatString("Incorrect owner of ", mPoint));
            break;
        case 3:
            if (mBuildType == "road")
            {
                aReturnMsg.push_back("None of the adjacent vertices is owner by current player");
            }
            else
            {
                aReturnMsg.push_back("One or more adjacent vertex is occupied");
            }
            break;
        case 4:
            aReturnMsg.push_back("Insufficient resources");
            break;
        case -1:
        default:
            aReturnMsg.push_back("Internal error");
            return ActionStatus::FAILED;
    }

    return ActionStatus::SUCCESS;
}

ActionStatus BuildHandler::processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    // first prompt user for build type: road or settlement or city
    if (mBuildType == "")
    {
        INFO_LOG(command() + " reading parameter " + aParam);
        if (indexInVector(aParam, mBuildTypeMatchingPool) >= 0)
        {
            mBuildType = aParam;
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
        if (mBuildType == "road")
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
        else if (mBuildType == "settlement" || mBuildType == "city" )
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
    return (mPoint != Point_t{0, 0} && mBuildType != "");
}

void BuildHandler::resetParameters()
{
    mPoint = Point_t{0, 0};
    mBuildType = "";
}

void BuildHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    if (mBuildType == "" && mPoint == Point_t{0, 0})
    {
        aReturnMsg.push_back("Please first type in what you want to build: road, settlement, or city");
        aReturnMsg.push_back("Then click on the map where you want to build");
        return;
    }
    else if (mBuildType != "" && mPoint == Point_t{0, 0})
    {
        aReturnMsg.push_back("You want to build a " + mBuildType);
        aReturnMsg.push_back("Please click on the map where you want to build");
        return;
    }
}