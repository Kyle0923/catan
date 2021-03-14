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
    char getCharRepresentation(bool aUseId = false) const override;
    virtual ~Blank();
};


#endif /* INCLUDE_BLANK_HPP */