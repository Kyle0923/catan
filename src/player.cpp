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
    mDevCard.at(static_cast<size_t>(aCard)) += aAmount;
}

int Player::consumeDevelopmentCard(DevelopmentCardTypes aCard)
{
    if (mDevCard.at(static_cast<size_t>(aCard)) == 0)
    {
        WARN_LOG("Unable to use development card: " + developmentCardTypesToStr(aCard));
        return 1;
    }

    INFO_LOG("Used development card " + developmentCardTypesToStr(aCard));
    --mDevCard.at(static_cast<size_t>(aCard));
    ++mDevCardUsed.at(static_cast<size_t>(aCard));
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

size_t Player::getVictoryPoint(bool aPublic) const
{
    size_t vicPoint = (mLargestArmy ? 2 : 0) + \
        (mLongestRoad ? 2 : 0) + \
        (aPublic ? 0 : mDevCard.at(static_cast<size_t>(DevelopmentCardTypes::ONE_VICTORY_POINT)));
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

const std::array<size_t, DEVELOPMENT_CARD_TYPE_SIZE>& Player::getDevCards() const
{
    return mDevCard;
}

const std::array<size_t, DEVELOPMENT_CARD_TYPE_SIZE>& Player::getUsedDevCards() const
{
    return mDevCardUsed;
}

bool Player::hasResources(ResourceTypes aResource, size_t aAmount) const
{
    if (mResourcesOnHand.at(static_cast<size_t>(aResource)) >= aAmount)
    {
        return true;
    }
    return false;
}

bool Player::hasResources(const std::map<ResourceTypes, size_t>& aResourceConfig) const
{
    bool rc = true;
    for (auto iter : aResourceConfig)
    {
        rc &= hasResources(iter.first, iter.second);
    }
    return rc;
}

Player::Player(int aId) :
    mId(aId),
    mLargestArmy(false),
    mLongestRoad(false),
    mResourcesOnHand({0}),
    mDevCard({0}),
    mDevCardUsed({0})
{
    // empty
    // TODO: the following is for testing only
    mResourcesOnHand[(int)ResourceTypes::CLAY] = 10;
    mResourcesOnHand[(int)ResourceTypes::SHEEP] = 10;
    mResourcesOnHand[(int)ResourceTypes::WHEAT] = 10;
    mResourcesOnHand[(int)ResourceTypes::WOOD] = 10;
    mResourcesOnHand[(int)ResourceTypes::ORE] = 10;
}