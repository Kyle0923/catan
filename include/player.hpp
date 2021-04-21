/**
 * Project: catan
 * @file player.hpp
 * @brief abstraction of player
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_PLAYER_HPP
#define INCLUDE_PLAYER_HPP

#include <array>
#include <set>
#include "common.hpp"
#include "vertex.hpp"
#include "edge.hpp"

class Player
{
private:
    const int mId;
    bool mLargestArmy;
    bool mLongestRoad;
    std::array<size_t, CONSUMABLE_RESOURCE_SIZE> mResourcesOnHand;
    std::array<size_t, DEVELOPMENT_CARD_TYPE_SIZE> mDevCard;
    std::array<size_t, DEVELOPMENT_CARD_TYPE_SIZE> mDevCardUsed;
    std::set<const Vertex*> mBuilding;
    std::set<const Edge*> mRoad;

public:
    void drawDevelopmentCard(DevelopmentCardTypes aCard, size_t aAmount = 1);
    int useDevelopmentCard(DevelopmentCardTypes aCard);
    void addResources(ResourceTypes aResource, size_t aAmount);
    void addBuilding(const Vertex& aVertex);
    void addRoad(const Edge& aEdge);
    void setLargestArmy(const bool aLargestArmy);
    void setLongestRoad(const bool aLongestRoad);
    size_t getPlayerLongestRoadSize() const;
    int getId() const;

    // aPublic: to show to public? when set to true, exclude devCard(Victory Point Card)
    size_t getVictoryPoint(bool aPublic);

    Player(int aId);
};

#endif /* INCLUDE_PLAYER_HPP */
