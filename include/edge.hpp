#ifndef INCLUDE_EDGE_HPP
#define INCLUDE_EDGE_HPP

#include <string>
#include "terrain.hpp"

class Edge : public Terrain
{
private:
    std::string mOwner;
public:
    Edge(int aId, Point_t aTopRight);
    ~Edge();
};


#endif /* INCLUDE_EDGE_HPP */
