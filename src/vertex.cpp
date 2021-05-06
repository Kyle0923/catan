/**
 * Project: catan
 * @file vertex.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "logger.hpp"
#include "game_map.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "blank.hpp"
#include "utility.hpp"

int Vertex::populateAdjacencies(GameMap& aMap)
{
    // reset vertex
    mOwner = -1;
    mColony = ColonyType::NONE;
    mIsCoastal = false;
    mHarbour = nullptr;
    mAdjacentVertices.clear();
    mAdjacentEdges.clear();

    int rc = 0;

    // no need to add {x, y+1} & {x,y-1}, because we don't have vertical edge
    rc |= addAdjacency(aMap, mTopLeft.x - 1, mTopLeft.y);
    rc |= addAdjacency(aMap, mTopLeft.x - 1, mTopLeft.y - 1);
    rc |= addAdjacency(aMap, mTopLeft.x + 1, mTopLeft.y - 1);
    rc |= addAdjacency(aMap, mTopLeft.x + 1, mTopLeft.y);
    rc |= addAdjacency(aMap, mTopLeft.x + 1, mTopLeft.y + 1);
    rc |= addAdjacency(aMap, mTopLeft.x - 1, mTopLeft.y + 1);

    if (mAdjacentVertices.size() == 0 || mAdjacentVertices.size() == 0)
    {
        WARN_LOG("Dangling vertex found at ", mTopLeft);
        rc = 1;
    }

    DEBUG_LOG_L0("Populated Adjacent Vertices for " + getStringId() + " ", mAdjacentVertices);
    DEBUG_LOG_L0("Populated Adjacent Edges for " + getStringId() + " ", mAdjacentEdges);

    (rc != 0) ?
        WARN_LOG("Failed to populate adjacencies of ", getStringId(), " at ", mTopLeft)
        :
        DEBUG_LOG_L1("Successfully populated adjacencies of ", getStringId());
    return rc;
}

int Vertex::addAdjacency(GameMap& aMap, const size_t aPointX, const size_t aPointY)
{
    const Terrain* const pTerrain = aMap.getTerrain(aPointX, aPointY);
    if (const Edge* const pEdge = dynamic_cast<const Edge*>(pTerrain))
    {
        // is edge
        mAdjacentEdges.emplace(pEdge);
        const Vertex* const pAdjacentVertex = pEdge->getOtherVertex(aMap, *this);
        if (!pAdjacentVertex)
        {
            WARN_LOG("Error when adding adjacent vertex for " + getStringId() \
                        + " from " + pEdge->getStringId());
            return 1;
        }
        mAdjacentVertices.emplace(pAdjacentVertex);
    }
    else if (pTerrain == Blank::getBlank())
    {
        mIsCoastal = true;
    }
    return 0;
}

const std::set<const Vertex*>& Vertex::getAdjacentVertices() const
{
    return mAdjacentVertices;
}

std::set<const Edge*> Vertex::getOtherEdges(const Edge& aEdge) const
{
    if (mAdjacentEdges.count(&aEdge) != 1)
    {
        WARN_LOG("Unknown adjacent edge: " + aEdge.getStringId() + " at ", aEdge.getTopLeft());
        return std::set<const Edge*>{};
    }
    std::set<const Edge*> otherEdges = mAdjacentEdges;
    otherEdges.erase(&aEdge);
    return otherEdges;
}

int Vertex::setOwner(int aPlayerId, ColonyType aColony)
{
    mOwner = aPlayerId;
    mColony = aColony;
    mColorIndex = static_cast<ColorPairIndex>(mOwner + ColorPairIndex::PLAYER_START);
    INFO_LOG("Successfully set " + colonyTypesToStr(aColony) + " for Player#", mOwner, " for " + getStringId());
    return 0;
}

int Vertex::getOwner() const
{
    return mOwner;
}

ColonyType Vertex::getColonyType() const
{
    return mColony;
}

bool Vertex::isCoastal() const
{
    return mIsCoastal;
}

bool Vertex::hasHarbour() const
{
    return (mHarbour != nullptr);
}

int Vertex::setHarbour(Harbour* const aHarbour)
{
    if (aHarbour == nullptr)
    {
        DEBUG_LOG_L0("Clearing harbour for " + getStringId() +", existing: " + (mHarbour ? mHarbour->getStringId() : "nullptr"));
        mHarbour = nullptr;
        return 0;
    }
    if (mHarbour)
    {
        INFO_LOG("Reseting harbour for " + getStringId() +" current: " + mHarbour->getStringId() + " new: " + aHarbour->getStringId());
    }
    mHarbour = aHarbour;
    return 0;
}

char Vertex::getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    switch (mColony)
    {
    case ColonyType::SETTLEMENT:
        return 'S';
    case ColonyType::CITY:
        return 'C';
    case ColonyType::NONE:
    default:
        return '+';
    }
}

std::string Vertex::getStringId() const
{
    return Logger::formatString("Vertex#", mId);
}

Vertex::Vertex(const int aId, const Point_t aTopLeft) :
    Terrain(aId, aTopLeft),
    mIsCoastal(false),
    mHarbour(nullptr),
    mOwner(-1),
    mColony(ColonyType::NONE)
{
    // empty
}

Vertex::~Vertex()
{
}
