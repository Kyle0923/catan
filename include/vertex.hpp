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
#include "terrain.hpp"
#include "harbour.hpp"

enum BuildingType
{
    NONE = 0,
    SETTLEMENT = 1,
    CITY = 2
};

class Vertex : public Terrain
{
private:
    bool mIsCoastal;
    Harbour* mHarbour;
    std::string mOwner;
    BuildingType mBuilding;
    std::vector<const Vertex*> mAdjacentVertices;  //mAdjacencies only store edges, adjacent vertices are store here
    int addAdjacency(GameMap& aMap, const size_t aPointX, const size_t aPointY);
public:
    const std::vector<const Vertex*>& getAdjacentVertices() const;
    void setOwner(std::string aOwner, BuildingType aBuilding = BuildingType::SETTLEMENT);
    std::string getOwner();
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
