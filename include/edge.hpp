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
#include <set>
#include <utility>
#include "terrain.hpp"
#include "vertex.hpp"

class Vertex;

class Edge : public Terrain
{
private:
    Point_t mOtherEnd; // the end other than mTopLeft
    int mOwner;
    char mDirection;

    std::set<const Vertex*> mAdjacentVertices;
    std::set<const Edge*> mAdjacentEdges;

    std::pair<Point_t, Point_t> getAdjacentVertexPoints() const;
    int addAdjacency(GameMap& aMap, const Point_t aPoint);
public:
    static constexpr int HORIZONTAL_LENGTH = 9;
    std::vector<Point_t> getAllPoints() const override;
    const Vertex* getOtherVertex(const GameMap& aMap, const Vertex& aVertex) const; //get connected vertex that is not the input
    int populateAdjacencies(GameMap& aMap) override;
    char getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const override;
    std::string getStringId() const override;

    bool isAvailable(int aPlayerId) const;

    int setOwner(int aPlayerId);
    int getOwner() const;

    Edge(const int aId, const Point_t aTopLeft, const char aDirection);
    virtual ~Edge();
};

#endif /* INCLUDE_EDGE_HPP */
