#pragma once
#include "split.h"

struct Microset;
struct Path;

struct Mezzoset : SplitList<Microset, Path, Mezzoset>
{
    static int MezzosetSize;

    void anectMicroset(SonIter_t micro, bool atEnd = true);
    
    void halve();
};
