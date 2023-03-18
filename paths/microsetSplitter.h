#pragma once
#include "split.h"
#include <vector>

using std::vector;
struct Microset;
struct Path;

class MicrosetSplitter
{
    static int targetSize;

    Iter_t <Microset> oldMicro, newMicro;
    Iter_t <Path> oldLabel, longestsLabel;
    bool oldLabOnLeft, oldLabOnRight;
    bool prefixIntoNew;

    vector <vector <int> > extractPaths() const;

    void addSubpath(Iter_t <Microset> micro, const vector <int>& p, int start, int len);

    void addSubpath(Iter_t <Microset> micro, const vector <int>& p);

    void updateMezzo(Iter_t <Microset> micro, Iter_t <Path> newLabel, Iter_t <Path> oldLabel);

    void setupNewMicrosetsPlacement();

    void distributePaths(vector <vector <int> >& paths, int longestPathIndex);

    void manageLongestPath(vector <int>& longest);


    public:

    static void init(int microsetSize);

    MicrosetSplitter(Iter_t <Microset> micro);
    
    void split();
};
