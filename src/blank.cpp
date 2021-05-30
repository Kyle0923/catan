/**
 * Project: catan
 * @file blank.cpp
 *
 * @author Zonghao Huang <kyle0923@qq.com>
 *
 * All right reserved.
 */

#include "blank.hpp"

Blank* Blank::mBlank = nullptr;

Blank* Blank::getBlank()
{
    static Blank blank(0, Point_t{0,0});
    if (!mBlank)
    {
        mBlank = &blank;
    }
    return mBlank;
}

char Blank::getCharRepresentation(const size_t aPointX, const size_t aPointY, const bool aUseId) const
{
    return ' ';
}

std::string Blank::getStringId() const
{
    return "Blank";
}

Blank::Blank(const int aId, const Point_t aTopLeft) :
    Terrain(aId, aTopLeft)
{
    // empty
}

Blank::~Blank()
{
}
