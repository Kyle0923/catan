/**
 * Project: catan
 * @file road_building_handler.cpp
 *
 * @brief handling road_building developmentcard
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_common.hpp"

RoadBuildingHandler::RoadBuildingHandler() :
    mRoadCounter(0U)
{
    // empty
}

std::string RoadBuildingHandler::command() const
{
    return "road_building";
}

ActionStatus RoadBuildingHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    if (!parameterComplete())
    {
        return ActionStatus::PARAM_REQUIRED;
    }

    return ActionStatus::SUCCESS;
}

ActionStatus RoadBuildingHandler::processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    INFO_LOG(command() + "reading mouse event ", aPoint);

    int rc = aMap.buildRoad(aPoint, false);

    if (rc != 0)
    {
        INFO_LOG("Construction failed, GameMap::buildRoad() rc=", rc);
    }
    else
    {
        INFO_LOG("Successfully built road for Player#", aMap.currentPlayer());
        ++mRoadCounter;
    }

    switch (rc)
    {
        case 0:
            aReturnMsg.emplace_back("Construction complete");
            return ActionStatus::SUCCESS;
        case 1:
            aReturnMsg.emplace_back("Incorrect coordinate, Player clicked is not an edge");
            break;
        case 2:
            aReturnMsg.emplace_back(Logger::formatString("Incorrect owner of ", aPoint));
            break;
        case 3:
            aReturnMsg.emplace_back("None of the adjacent vertices is owner by current player");
            break;
        case -1:
        default:
            aReturnMsg.emplace_back("Internal error");
    }
    return ActionStatus::FAILED;

}

bool RoadBuildingHandler::parameterComplete() const
{
    return (mRoadCounter >= 2);
}

void RoadBuildingHandler::resetParameters()
{
    mRoadCounter = 0;
}

void RoadBuildingHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    if (mRoadCounter == 0)
    {
        aReturnMsg.emplace_back("Please click on the map where you want to build the first road");
        return;
    }
    if (mRoadCounter == 1)
    {
        aReturnMsg.emplace_back("Please click on the map where you want to build the second road");
        return;
    }
}