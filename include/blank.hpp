#ifndef INCLUDE_BLANK_HPP
#define INCLUDE_BLANK_HPP

#include "terrain.hpp"

// singleton
class Blank : public Terrain
{
private:
    static Blank* mBlank;
    Blank(int aId, Point_t aTopRight);
public:
    static Blank* getBlank();
    ~Blank();
};


#endif /* INCLUDE_BLANK_HPP */