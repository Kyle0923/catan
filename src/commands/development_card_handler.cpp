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

#include "command_handlers.hpp"
#include "command_parameter_reader.hpp"

const std::vector<std::string> DevelopmentCardHandler::mActionPool = {"play", "buy"};
// the order in mPlayableDevCard must be exactly the same as defined in enum DevelopmentCardTypes
const std::vector<std::string> DevelopmentCardHandler::mPlayableDevCard = \
    {"knight", "road_building", "year_of_plenty", "monoploy"};

DevelopmentCardHandler::DevelopmentCardHandler() :
    mAction(""),
    mDevCard(-1),
    mRobberMoveHandler(RobberMoveHandler::getRobberHandler())
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
    else if (aParamIndex == 1 && mAction == "play")
    {
        return mPlayableDevCard;
    }
    return EMPTY_STRING_VECTOR;
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

ActionStatus DevelopmentCardHandler::statefulRun(GameMap& aMap, UserInterface& aUi, std::vector<std::string>& aReturnMsg)
{
    DevelopmentCardTypes devCard = static_cast<DevelopmentCardTypes>(mDevCard);
    if (mAction == "buy")
    {
        int rc = aMap.currentPlayerBuyDevCard(devCard);
        rc == 0 ?
            aReturnMsg.emplace_back("Successfully buy a development card: " + developmentCardTypesToStr(devCard))
            :
            aReturnMsg.emplace_back("Failed to buy a development card, insufficient resources");
        return ActionStatus::SUCCESS;
    }
    // mAction == "play"
    if (devCard == DevelopmentCardTypes::ONE_VICTORY_POINT)
    {
        aReturnMsg.emplace_back("No action for development card ONE_VICTORY_POINT");
        return ActionStatus::SUCCESS;
    }

    if (aMap.currentPlayerConsumeDevCard(devCard) == 1)
    {
        aReturnMsg.emplace_back("Insufficient development card: " + developmentCardTypesToStr(devCard));
        return ActionStatus::SUCCESS;
    }

    switch (devCard)
    {
        case DevelopmentCardTypes::KNIGHT:
        {
            aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(&mRobberMoveHandler));
            return ActionStatus::SUCCESS;
        }
        case DevelopmentCardTypes::ROAD_BUILDING:
        {
            aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(&mRoadBuilder));
            return ActionStatus::SUCCESS;
        }
        case DevelopmentCardTypes::YEAR_OF_PLENTY:
        {
            aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(&mYearOfPlentyHandler));
            return ActionStatus::SUCCESS;
        }
        case DevelopmentCardTypes::MONOPOLY:
        {
            aUi.pushCommandHelper(std::make_unique<CommandParameterReader>(&mMonopolyHandler));
            return ActionStatus::SUCCESS;
        }
        default:
        {
            return ActionStatus::SUCCESS;
        }
    }
}

ActionStatus DevelopmentCardHandler::onParameterReceive(GameMap& aMap, const std::string& aParam, Point_t aPoint, std::vector<std::string>& aReturnMsg)
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
            aReturnMsg.emplace_back("unknown parameter: " + aParam);
            return ActionStatus::FAILED;
        }
    }
    if (mAction == "play" && mDevCard == -1)
    {
        mDevCard = indexInVector(aParam, mPlayableDevCard);
        if (mDevCard == -1)
        {
            aReturnMsg.emplace_back("unknown parameter: " + aParam);
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
