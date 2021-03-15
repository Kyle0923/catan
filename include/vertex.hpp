#ifndef INCLUDE_VERTEX_HPP
#define INCLUDE_VERTEX_HPP

#include <string>
#include "terrain.hpp"

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
    std::string mOwner;
    BuildingType mBuilding;
    std::vector<Vertex*> mAdjacentVertices;  //neighbours only store edges, adjacentVertices are store here
    int populateNeighbour(GameMap& aMap, const size_t aPointX, const size_t aPointY);
public:
    void setOwner(std::string aOwner, BuildingType aBuilding = BuildingType::SETTLEMENT);
    std::string getOwner();
    int populateNeighbours(GameMap& aMap) override;
    char getCharRepresentation(bool aUseId = false) const override;
    std::string getFullId() const override;
    Vertex(const int aId, const Point_t aTopRight);
    virtual ~Vertex();
};

#endif /* INCLUDE_VERTEX_HPP */
