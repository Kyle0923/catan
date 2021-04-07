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

int Vertex::populateAdjacencies(GameMap& aMap)
{
    // reset vertex
    mOwner = "";
    mBuilding = BuildingType::NONE;
    mIsCoastal = false;
    mHarbour = nullptr;
    mAdjacentVertices.clear();
    mAdjacencies.clear();

    int rc = 0;
    rc |= addAdjacency(aMap, mTopLeft.x - 1, mTopLeft.y);
    rc |= addAdjacency(aMap, mTopLeft.x - 1, mTopLeft.y - 1);
    rc |= addAdjacency(aMap, mTopLeft.x + 1, mTopLeft.y - 1);
    rc |= addAdjacency(aMap, mTopLeft.x + 1, mTopLeft.y);
    rc |= addAdjacency(aMap, mTopLeft.x + 1, mTopLeft.y + 1);
    rc |= addAdjacency(aMap, mTopLeft.x - 1, mTopLeft.y + 1);

    (rc != 0) ?
        WARN_LOG("Failed to populate adjacencies of ", getStringId())
        :
        DEBUG_LOG_L2("Successfully populated adjacencies of ", getStringId());
    return rc;
}

int Vertex::addAdjacency(GameMap& aMap, const size_t aPointX, const size_t aPointY)
{
    const Terrain* const pTerrain = aMap.getTerrain(aPointX, aPointY);
    if (const Edge* const pEdge = dynamic_cast<const Edge*>(pTerrain))
    {
        // is edge
        mAdjacencies.push_back(pTerrain);
        const Vertex* const pAdjacentVertex = pEdge->getVertex(this);
        mAdjacentVertices.push_back(pAdjacentVertex);
        DEBUG_LOG_L1("Added adjacent " + pAdjacentVertex->getStringId() + " for " + getStringId());
    }
    else if (dynamic_cast<const Blank*>(pTerrain))
    {
        mIsCoastal = true;
    }
    return 0;
}

const std::vector<const Vertex*>& Vertex::getAdjacentVertices() const
{
    return mAdjacentVertices;
}

void Vertex::setOwner(std::string aOwner, BuildingType aBuilding)
{
    mOwner = aOwner;
    mBuilding = aBuilding;
}

std::string Vertex::getOwner()
{
    return mOwner;
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
    switch (mBuilding)
    {
    case BuildingType::NONE:
        return '+';
    case BuildingType::SETTLEMENT:
        return 'S';
    case BuildingType::CITY:
        return 'C';
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
    mHarbour(nullptr)
{
    mOwner = "None";
    mBuilding = BuildingType::NONE;
}

Vertex::~Vertex()
{
}
