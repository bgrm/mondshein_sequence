#pragma once
#include "split.h"

struct Universe;
struct Mezzoset;
struct Microset;

struct Path : SplitList <Mezzoset, Universe, Path>
{
    int id = 0;
    int leftEdge = 0;
    int rightEdge = 0;

    void anectMicroset(Iter_t <Microset> micro);

    Iter_t <Microset> leftMicro() const;

    Iter_t <Microset> rightMicro() const;
};

struct Universe : SplitList <Path, Universe, Universe>
{};
