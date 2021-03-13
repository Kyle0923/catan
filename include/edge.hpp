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
    Edge(int aId, Point_t aTopRight, char aDirection);
    char getCharRepresentation(bool useId = false) const override;
    ~Edge();
};


#endif /* INCLUDE_EDGE_HPP */
