#pragma once
#include "split.h"
#include <stdint.h>
#include <vector>

#define BITS_USED(x) (32 - __builtin_clz(x))

struct Path;
struct Mezzoset;
struct Microset;
struct Element;

using std::vector;
using Mask_t = unsigned int32_t;

struct MicroChild
{
    Iter_t <Microset> father;
};

struct Microset : SplitList<MicroChild, Mezzoset, Microset>
{
    static int MicrosetSize;
    static int bitsPerEntry;
    static vector <int> preprocessingLeft, preprocessingRight, preprocessingPosition;
    static Mask_t emptyMicroMask;

    Mask_t mask = emptyMicroMask;
    vector <int> edges, L, R;

    static void updateMask(Mask_t& m, int pos, int val);

    static Mask_t getPos(Mask_t m, int pos);

    static Mask_t getStateMask(Mask_t m, int i);

    static int preprocessingRec(Mask_t m, int i, int rightEnd, Mask_t visMask);

    static void init(int m);


    void setLeft(int pos, int val);

    void setRight(int pos, int val);

    Mask_t getStateMask(int i) const;

    int getLeft(int pos) const;

    int getRight(int pos) const;

    int size() const override;

    Iter_t <Path> microLabel() const;

    Element& getEdge(int index) const;

    bool lastsSubpathMarginal() const;

    Iter_t <Path> edgeLabel(int index) const;

    bool labelConsistent(int index) const;

    int borderEdgeWithinSubpath(int index, bool leftSide) const;

    int leftmostOfLabelSubpath() const;
    

    void connect(int l, int r);

    void insertAfter(int index, Element& fresh);

    void insertBefore(int index, Element& fresh);

    void clear();

    void split();
    using SplitList::split; // base's split has different arity

    void breakPath(int index, bool leftDirection, bool allowMicroRelabel = true);
};
