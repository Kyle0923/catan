/**
 * Project: catan
 * @file blank.hpp
 * @brief singleton class, handles the blank points on the map
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#ifndef INCLUDE_BLANK_HPP
#define INCLUDE_BLANK_HPP

#include "terrain.hpp"

// singleton
class Blank : public Terrain
{
private:
    static Blank* mBlank;
    Blank(const int aId, const Point_t aTopRight);
public:
    static Blank* getBlank();
    char getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId = false) const override;
    std::string getStringId() const override;
    virtual ~Blank();
};


#endif /* INCLUDE_BLANK_HPP */