#ifndef INCLUDE_EDGE_HPP
#define INCLUDE_EDGE_HPP

#include <string>
#include "terrain.hpp"

class Edge : public Terrain
{
private:
    Point_t mOtherEnd; // the end other than mTopRight
    char mDirection;
    std::string mOwner;
    static const int HORIZONTAL_LENGTH;
public:
    std::vector<Point_t> getAllPoints() override;
    Edge(const int aId, const Point_t aTopRight, const char aDirection);
    char getCharRepresentation(bool aUseId = false) const override;
    virtual ~Edge();
};


#endif /* INCLUDE_EDGE_HPP */
