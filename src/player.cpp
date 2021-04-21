/**
 * Project: catan
 * @file player.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "logger.hpp"
#include "utility.hpp"
#include "player.hpp"

void Player::drawDevelopmentCard(DevelopmentCardTypes aCard, size_t aAmount)
{
    mDevCard.at(aCard) += aAmount;
}

int Player::useDevelopmentCard(DevelopmentCardTypes aCard)
{
    if (mDevCard.at(aCard) == 0)
    {
        WARN_LOG("Unable to use development card: " + developmentCardTypesToStr(aCard));
        return 1;
    }

    INFO_LOG("Used development card " + developmentCardTypesToStr(aCard));
    --mDevCard.at(aCard);
    ++mDevCardUsed.at(aCard);
    return 0;
}

void Player::addResources(ResourceTypes aResource, size_t aAmount)
{
    mResourcesOnHand.at(static_cast<size_t>(aResource)) += aAmount;
}

void Player::addColony(const Vertex& aVertex)
{
    mColony.emplace(&aVertex);
}

void Player::addRoad(const Edge& aEdge)
{
    mRoad.emplace(&aEdge);
}

void Player::setLargestArmy(const bool aLargestArmy)
{
    mLargestArmy = aLargestArmy;
}

void Player::setLongestRoad(const bool aLongestRoad)
{
    mLongestRoad = aLongestRoad;
}

size_t Player::getPlayerLongestRoadSize() const
{
    // TODO: implement an algorithm to do this
    return 0;
}

size_t Player::getVictoryPoint(bool aPublic)
{
    size_t vicPoint = (mLargestArmy ? 2 : 0) + \
        (mLongestRoad ? 2 : 0) + \
        (aPublic ? 0 : mDevCard.at(DevelopmentCardTypes::VICTORY_POINT));
    for (const Vertex* const pVertex : mColony)
    {
        vicPoint += pVertex->getColonyType();
    }
    return vicPoint;
}

int Player::getId() const
{
    return mId;
}

Player::Player(int aId) :
    mId(aId),
    mLargestArmy(false),
    mLongestRoad(false)
{
    // empty
}