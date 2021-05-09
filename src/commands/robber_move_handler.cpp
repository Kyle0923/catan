/**
 * Project: catan
 * @file robber_move_handler.cpp
 *
 * @brief move the robber when roller 7
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_common.hpp"

std::string RobberMoveHandler::command() const
{
    return "robber_move";   // this should only be used for logging purpose
}

ActionStatus RobberMoveHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    aMap.moveRobber(mRobberDestination);
    ResourceTypes resourceToRob = ResourceTypes::NONE;
    int rc = aMap.robVertex(mRobbingVertex, resourceToRob);
    if (resourceToRob == ResourceTypes::NONE)
    {
        aReturnMsg.emplace_back("Robbing failed");
    }
    else
    {
        aReturnMsg.emplace_back("Successfully robbed " + resourceTypesToStr(resourceToRob));
    }
    switch (rc)
    {
        case 2:
            aReturnMsg.emplace_back(Logger::formatString("The vertex at ", mRobbingVertex, " has no owner"));
            break;
        case 3:
            aReturnMsg.emplace_back(Logger::formatString("The owner of vertex at ", mRobbingVertex, " has no resources"));
            break;
    }
    return ActionStatus::SUCCESS;
}

ActionStatus RobberMoveHandler::processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    if (mRobberDestination == Point_t{0, 0})
    {
        if (aMap.isTerrain<Land>(aPoint))
        {
            mRobberDestination = aPoint;
            return ActionStatus::SUCCESS;
        }
        else
        {
            aReturnMsg.emplace_back("Player clicked is not land");
            return ActionStatus::FAILED;
        }
    }
    if (mRobbingVertex == Point_t{0, 0})
    {
        if (const Vertex* const pVertex = dynamic_cast<const Vertex*>(aMap.getTerrain(aPoint)))
        {
            const std::vector<const Vertex*>& adjVertices = dynamic_cast<const Land*>(aMap.getTerrain(mRobberDestination))->getAdjacentVertices();
            if (indexInVector(pVertex, adjVertices) >= 0)
            {
                mRobbingVertex = aPoint;
                return ActionStatus::SUCCESS;
            }
            else
            {
                aReturnMsg.emplace_back(Logger::formatString("Please click on a vertex directly connect to the land at ", mRobberDestination));
                return ActionStatus::FAILED;
            }
        }
        else
        {
            aReturnMsg.emplace_back("Player clicked is not vertex");
            return ActionStatus::FAILED;
        }
    }
    return ActionStatus::FAILED;
}

bool RobberMoveHandler::parameterComplete() const
{
    return (mRobberDestination != Point_t{0, 0} &&
            mRobbingVertex != Point_t{0, 0});
}

void RobberMoveHandler::resetParameters()
{
    mRobberDestination = {0, 0};
    mRobbingVertex = {0, 0};
}

void RobberMoveHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    if (mRobberDestination == Point_t{0, 0})
    {
        aReturnMsg.emplace_back("Please click on the map to choose where you want the robber to go");
        return;
    }
    aReturnMsg.emplace_back("Please click on the map to choose which settlement/city you want the rob");
}


RobberMoveHandler::RobberMoveHandler() :
    mRobberDestination(Point_t{0, 0}),
    mRobbingVertex(Point_t{0, 0})
{
    // empty
}
