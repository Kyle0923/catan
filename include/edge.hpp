/**
 * Project: catan
 * @file edge.hpp
 * @brief abstraction of the links (roads)
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_EDGE_HPP
#define INCLUDE_EDGE_HPP

#include <string>
#include "terrain.hpp"
#include "vertex.hpp"

class Edge : public Terrain
{
private:
    Point_t mOtherEnd; // the end other than mTopLeft
    char mDirection;
    std::string mOwner;
    int addAdjacency(GameMap& aMap, const size_t aPointX, const size_t aPointY);
public:
    static constexpr int HORIZONTAL_LENGTH = 9;
    std::vector<Point_t> getAllPoints() const override;
    Edge(const int aId, const Point_t aTopLeft, const char aDirection);
    const Vertex* getVertex(Vertex* const aVertex) const; //get connected vertex that is not the input
    int populateAdjacencies(GameMap& aMap) override;
    char getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const override;
    std::string getStringId() const override;
    virtual ~Edge();
};


#endif /* INCLUDE_EDGE_HPP */
