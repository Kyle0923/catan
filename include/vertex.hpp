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
    std::string mOwner;
    BuildingType mBuilding;
public:
    void setOwner(std::string aOwner, BuildingType aBuilding = BuildingType::SETTLEMENT);
    std::string getOwner();
    char getCharRepresentation(bool aUseId = false) const override;
    Vertex(const int aId, const Point_t aTopRight);
    virtual ~Vertex();
};

#endif /* INCLUDE_VERTEX_HPP */
