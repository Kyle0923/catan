/**
 * Project: catan
 * @file development_card_handler.cpp
 *
 * @brief handle player's development card (play, buy)
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "command_common.hpp"

const std::vector<std::string> DevelopmentCardHandler::mActionPool = {"play", "buy"};
// the order in mPlayableDevCard must be exactly the same as defined in enum DevelopmentCardTypes
const std::vector<std::string> DevelopmentCardHandler::mPlayableDevCard = \
    {"knight", "road_building", "year_of_plenty", "monoploy"};

DevelopmentCardHandler::DevelopmentCardHandler() :
    mAction(""),
    mDevCard(-1)
{
    // empty
}

std::string DevelopmentCardHandler::command() const
{
    return "development_card";
}

std::string DevelopmentCardHandler::description() const
{
    return "play or buy development cards";
}

const std::vector<std::string>& DevelopmentCardHandler::paramAutoFillPool(size_t aParamIndex) const
{
    if (aParamIndex == 0 && mAction == "")
    {
        return mActionPool;
    }
    else if (aParamIndex == 1)
    {
        return mPlayableDevCard;
    }
    return mEmptyVector;
}

size_t DevelopmentCardHandler::currentParamIndex() const
{
    if (mAction == "")
    {
        return 0;
    }
    else if (mDevCard == -1)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}

ActionStatus DevelopmentCardHandler::act(GameMap& aMap, UserInterface& aUi, std::vector<std::string> aArgs, std::vector<std::string>& aReturnMsg)
{
    for (std::string arg : aArgs)
    {
        processParameter(aMap, arg, Point_t{0, 0}, aReturnMsg);
    }

    if (!parameterComplete())
    {
        return ActionStatus::PARAM_REQUIRED;
    }
    // TODO: aMap.playCard / aMap.buyCard
    return ActionStatus::SUCCESS;
}

ActionStatus DevelopmentCardHandler::processParameter(GameMap& aMap, std::string aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
{
    if (mAction == "")
    {
        if (aParam == "play" || aParam == "buy")
        {
            mAction = aParam;
            return ActionStatus::SUCCESS;
        }
        else
        {
            aReturnMsg.emplace_back("unkown parameter: " + aParam);
            return ActionStatus::FAILED;
        }
    }
    if (mAction == "play" && mDevCard == -1)
    {
        mDevCard = indexInVector(aParam, mPlayableDevCard);
        if (mDevCard == -1)
        {
            aReturnMsg.emplace_back("unkown parameter: " + aParam);
            return ActionStatus::FAILED;
        }
        return ActionStatus::SUCCESS;
    }
    aReturnMsg.emplace_back("discarded parameter: " + aParam);
    return ActionStatus::SUCCESS;
}

bool DevelopmentCardHandler::parameterComplete() const
{
    return (mAction == "play" && mDevCard != -1) || (mAction == "buy");
}

void DevelopmentCardHandler::resetParameters()
{
    mAction = "";
    mDevCard = -1;
}

void DevelopmentCardHandler::instruction(std::vector<std::string>& aReturnMsg) const
{
    if (mAction == "")
    {
        aReturnMsg.emplace_back("What do you want to do with development card?");
        aReturnMsg.emplace_back(stringVectorJoin(mActionPool));
        return;
    }
    if (mAction == "play" && mDevCard == -1)
    {
        aReturnMsg.emplace_back("Which development card you want to play?");
        aReturnMsg.emplace_back(stringVectorJoin(mPlayableDevCard));
        return;
    }
}
