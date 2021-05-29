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

class Edge;

class Vertex : public Terrain
{
private:
    bool mIsCoastal;
    Harbour* mHarbour;
    int mOwner;
    ColonyType mColony;

    std::set<const Vertex*> mAdjacentVertices;
    std::set<const Edge*> mAdjacentEdges;

    int addAdjacency(GameMap& aMap, const size_t aPointX, const size_t aPointY);
public:
    const std::set<const Vertex*>& getAdjacentVertices() const;
    std::set<const Edge*> getOtherEdges(const Edge& aEdge) const; //get connected edges that is not the input

    int setOwner(int aPlayerId, ColonyType aColony);
    int getOwner() const;

    /**
     * @return true if none of the adjacent vertex is occupied
     */
    bool isAvailable() const;

    /**
     * @param aPlayerId the player who is about to buildColony on this vertex
     * @return true if at least one of the edge is owned by this player
     */
    bool isConnected(const int aPlayerId) const;

    ColonyType getColonyType() const;
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
