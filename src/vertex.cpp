#include "game_map.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "blank.hpp"

int Vertex::populateNeighbours(GameMap& aMap)
{
    // reset vertex
    mOwner = "";
    mBuilding = BuildingType::NONE;
    mIsCoastal = false;
    mAdjacentVertices.clear();
    mNeighbour.clear();

    int rc = 0;
    rc |= populateNeighbour(aMap, mTopRight.x - 1, mTopRight.y);
    rc |= populateNeighbour(aMap, mTopRight.x - 1, mTopRight.y - 1);
    rc |= populateNeighbour(aMap, mTopRight.x + 1, mTopRight.y - 1);
    rc |= populateNeighbour(aMap, mTopRight.x + 1, mTopRight.y);
    rc |= populateNeighbour(aMap, mTopRight.x + 1, mTopRight.y + 1);
    rc |= populateNeighbour(aMap, mTopRight.x - 1, mTopRight.y + 1);
    if (rc)
    {
        ERROR_LOG("Failed to populate neighbours of ", getFullId());
    }
    else
    {
        INFO_LOG("Successfully populated neighbours of ", getFullId());
    }
    return rc;
}

int Vertex::populateNeighbour(GameMap& aMap, const size_t aPointX, const size_t aPointY)
{
    Terrain* const pTerrain = aMap.getTerrain(aPointX, aPointY);
    if (Edge* pEdge = dynamic_cast<Edge*>(pTerrain))
    {
        // is edge
        mNeighbour.push_back(pTerrain);
        Vertex* const pAdjacentVertex = pEdge->getVertex(this);
        mAdjacentVertices.push_back(pAdjacentVertex);
        DEBUG_LOG("Added adjacent " + pAdjacentVertex->getFullId() + " for " + getFullId());
    }
    else if (dynamic_cast<Blank*>(pTerrain))
    {
        mIsCoastal = true;
    }
    return 0;
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

char Vertex::getCharRepresentation(bool aUseId) const
{
    if (aUseId)
    {
        return static_cast<char>(mId % 10) + '0';
    }
    else
    {
        return '+';
    }
}

std::string Vertex::getFullId() const
{
    return Logger::formatString("Vertex#", mId);
}

Vertex::Vertex(const int aId, const Point_t aTopRight) :
    Terrain(aId, aTopRight),
    mIsCoastal(false)
{
    mOwner = "None";
    mBuilding = BuildingType::NONE;
}

Vertex::~Vertex()
{
}
