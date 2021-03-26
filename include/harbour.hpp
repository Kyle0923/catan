/**
 * Project: catan
 * @file harbour.hpp
 * @brief abstraction of the harbours
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_HARBOUR_HPP
#define INCLUDE_HARBOUR_HPP

#include "terrain.hpp"

class Harbour : public Terrain
{
private:
    ResourceTypes mResourceType;
    Point_t mVertex1;   //the vertex with smaller y or x
    Point_t mVertex2;
    std::vector<Point_t> mLinks; // the points of the links to vertices
public:
    // based on vertex1 and vertex2, calculate points that belongs to this harbour
    int calculatePoints(GameMap& aMap);

    ResourceTypes getResourceType();
    void setResourceType(ResourceTypes aResource);

    std::string getStringId() const override;
    char getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const override;
    std::vector<Point_t> getAllPoints() const override;

    Harbour(const int aId, const ResourceTypes aResource, const Point_t aVertex1, const Point_t aVertex2);
    virtual ~Harbour();
};


#endif /* INCLUDE_HARBOUR_HPP */
