#ifndef INCLUDE_EDGE_HPP
#define INCLUDE_EDGE_HPP

#include <string>
#include "terrain.hpp"
#include "vertex.hpp"

class Edge : public Terrain
{
private:
    Point_t mOtherEnd; // the end other than mTopRight
    char mDirection;
    std::string mOwner;
    int populateNeighbour(GameMap& aMap, const size_t aPointX, const size_t aPointY);
public:
    static constexpr int HORIZONTAL_LENGTH = 9;
    const std::vector<Point_t> getAllPoints() const override;
    Edge(const int aId, const Point_t aTopRight, const char aDirection);
    Vertex* getVertex(Vertex* const aVertex); //get connected vertex that is not the input
    int populateNeighbours(GameMap& aMap) override;
    char getCharRepresentation(bool aUseId = false) const override;
    std::string getFullId() const override;
    virtual ~Edge();
};


#endif /* INCLUDE_EDGE_HPP */
