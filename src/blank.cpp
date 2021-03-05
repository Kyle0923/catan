#include "blank.hpp"

Blank* Blank::mBlank = nullptr;

Blank* Blank::getBlank()
{
    static Blank blank(0, Point_t{0,0});
    if (!mBlank)
    {
        mBlank = &blank;
    }
    return &blank;
}

Blank::Blank(int aId, Point_t aTopRight) :
    Terrain(aId, aTopRight)
{
    // empty
}

Blank::~Blank()
{
}
