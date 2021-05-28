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

#include "command_handlers.hpp"

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

std::string BuildHandler::description() const
{
    return "build settlement/city or road";
}

const std::vector<std::string>& BuildHandler::paramAutoFillPool(size_t aParamIndex) const
{
    if (mBuildType == "" && aParamIndex == 0)
    {
        return mBuildTypeMatchingPool;
    }
    return mEmptyVector;
}

ActionStatus BuildHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    if (aArgs.size() != 0)
    {
        onParameterReceive(aMap, aArgs.front(), Point_t{0, 0}, aReturnMsg);
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

    if (rc != 0)
    {
        INFO_LOG("build command failed, build_type=" + mBuildType + ", playerID=", aMap.currentPlayer(), ", rc=", rc);
        aReturnMsg.emplace_back("Construction failed");
    }

    switch (rc)
    {
        case 0:
            INFO_LOG("Successfully built " + mBuildType + " for Player#", aMap.currentPlayer());
            aReturnMsg.emplace_back("Construction complete");
            break;
        case 1:
            aReturnMsg.emplace_back("Incorrect coordinate");
            break;
        case 2:
            aReturnMsg.emplace_back(Logger::formatString("Incorrect owner of ", mPoint));
            break;
        case 3:
            if (mBuildType == "road")
            {
                aReturnMsg.emplace_back("None of the adjacent vertices is owner by current player");
            }
            else
            {
                aReturnMsg.emplace_back("One or more adjacent vertex is occupied");
            }
            break;
        case 4:
            aReturnMsg.emplace_back("Insufficient resources");
            break;
        case -1:
        default:
            aReturnMsg.emplace_back("Internal error");
            return ActionStatus::FAILED;
    }

    return ActionStatus::SUCCESS;
}

ActionStatus BuildHandler::onParameterReceive(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    // first prompt user for build type: road or settlement or city
    INFO_LOG(command() + " reading parameter " + aParam);
    if (mBuildType == "")
    {
        if (indexInVector(aParam, mBuildTypeMatchingPool) >= 0)
        {
            mBuildType = aParam;
            return ActionStatus::SUCCESS;
        }
        else
        {
            return ActionStatus::FAILED;
        }
    }

    if (mPoint == Point_t{0, 0})
    {
        if (mBuildType == "road")
        {
            if (aMap.isTerrain<Edge>(aPoint))
            {
                mPoint = aPoint;
                return ActionStatus::SUCCESS;
            }
            else
            {
                aReturnMsg.emplace_back("Player clicked is not an edge");
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
                aReturnMsg.emplace_back("Player clicked is not a vertex");
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
    if (mBuildType == "")
    {
        aReturnMsg.emplace_back("What do you want to build:");
        aReturnMsg.emplace_back(stringVectorJoin(mBuildTypeMatchingPool));
        return;
    }
    else if (mBuildType != "" && mPoint == Point_t{0, 0})
    {
        aReturnMsg.emplace_back("You want to build a " + mBuildType);
        aReturnMsg.emplace_back("Please click on the map where you want to build");
        return;
    }
}
