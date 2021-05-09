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

bool Player::consumeResources(ResourceTypes aResource, size_t aAmount)
{
    if (mResourcesOnHand.at(static_cast<size_t>(aResource)) >= aAmount)
    {
        mResourcesOnHand.at(static_cast<size_t>(aResource)) -= aAmount;
        return true;
    }
    return false;
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

const std::array<size_t, CONSUMABLE_RESOURCE_SIZE>& Player::getResources() const
{
    return mResourcesOnHand;
}

bool Player::hasResources(ResourceTypes aResource, size_t aAmount) const
{
    if (mResourcesOnHand.at(static_cast<size_t>(aResource)) >= aAmount)
    {
        return true;
    }
    return false;
}

bool Player::hasResourceForRoad() const
{
    // 1 clay, 1 wood
    return hasResources(ResourceTypes::CLAY, 1) && hasResources(ResourceTypes::WOOD, 1);
}

bool Player::hasResourceForSettlement() const
{
    // 1 clay, 1 wood, 1 wheat, 1 sheep
    return hasResources(ResourceTypes::CLAY, 1) && \
           hasResources(ResourceTypes::WOOD, 1) && \
           hasResources(ResourceTypes::WHEAT, 1) && \
           hasResources(ResourceTypes::SHEEP, 1);
}

bool Player::hasResourceForCity() const
{
    // 2 wheat, 3 ore
    return hasResources(ResourceTypes::WHEAT, 2) && \
           hasResources(ResourceTypes::ORE, 3);
}

Player::Player(int aId) :
    mId(aId),
    mLargestArmy(false),
    mLongestRoad(false)
{
    // empty
    mResourcesOnHand[(int)ResourceTypes::CLAY] = 10;
    mResourcesOnHand[(int)ResourceTypes::SHEEP] = 10;
    mResourcesOnHand[(int)ResourceTypes::WHEAT] = 10;
    mResourcesOnHand[(int)ResourceTypes::WOOD] = 10;
    mResourcesOnHand[(int)ResourceTypes::ORE] = 10;
}