/**
 * Project: catan
 * @file vertex.hpp
 * @brief abstraction of the vertices (where players will build settlements and cities)
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_VERTEX_HPP
#define INCLUDE_VERTEX_HPP

#include <string>
#include <set>
#include "terrain.hpp"
#include "harbour.hpp"

enum BuildingType
{
    NONE = 0,
    SETTLEMENT = 1,
    CITY = 2
};

class Edge;

class Vertex : public Terrain
{
private:
    bool mIsCoastal;
    Harbour* mHarbour;
    std::string mOwner;
    BuildingType mBuilding;

    std::set<const Vertex*> mAdjacentVertices;
    std::set<const Edge*> mAdjacentEdges;

    int addAdjacency(GameMap& aMap, const size_t aPointX, const size_t aPointY);
public:
    const std::set<const Vertex*>& getAdjacentVertices() const;
    std::set<const Edge*> getOtherEdges(const Edge& aEdge) const; //get connected edges that is not the input
    void setOwner(std::string aOwner, BuildingType aBuilding = BuildingType::SETTLEMENT);
    std::string getOwner() const;
    BuildingType getBuildingType() const;
    bool isCoastal() const;
    bool hasHarbour() const;
    int setHarbour(Harbour* const aHarbour);
    int populateAdjacencies(GameMap& aMap) override;
    char getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const override;
    std::string getStringId() const override;
    Vertex(const int aId, const Point_t aTopLeft);
    virtual ~Vertex();
};

#endif /* INCLUDE_VERTEX_HPP */
