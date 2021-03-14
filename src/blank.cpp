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

char Blank::getCharRepresentation(bool aUseId) const
{
    if (aUseId)
    {
        return ' ';
    }
    else
    {
        return 'o';
    }
}

Blank::Blank(const int aId, const Point_t aTopRight) :
    Terrain(aId, aTopRight)
{
    // empty
}

Blank::~Blank()
{
}
