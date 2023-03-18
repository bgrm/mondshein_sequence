#include "micro.h"
#include "element.h"
#include "mezzo.h"
#include "path.h"
#include "microsetSplitter.h"
#include <cassert>


extern vector <Element> E;
extern Path::SelfIter_t nullPath;
extern Mezzoset::SelfIter_t unlabeledMicrosets;

int Microset::MicrosetSize;
int Microset::bitsPerEntry;
Mask_t Microset::emptyMicroMask;
vector <int> Microset::preprocessingLeft, Microset::preprocessingRight, Microset::preprocessingPosition;


void roundupToEven(int& x)
{   x += x & 1; }

bool ithBit(Mask_t m, int i)
{   return m >> i & 1;    }


void Microset::updateMask(Mask_t& m, int pos, int val) // static
{
    auto fullEntryMask = (1u << bitsPerEntry) - 1u;
    auto offset = pos * bitsPerEntry;
    auto frame = ~(fullEntryMask << offset);
    m &= frame;
    m |= val << offset;
}

Mask_t Microset::getPos(Mask_t m, int pos) // static
{
    auto fullEntryMask = (1u << bitsPerEntry) - 1u;
    int offset = pos * bitsPerEntry;
    return (m >> offset) & fullEntryMask;
}

Mask_t Microset::getStateMask(Mask_t m, int i) // static
{
    assert(BITS_USED(i) <= bitsPerEntry);
    return m << bitsPerEntry | i;
}

int Microset::preprocessingRec(Mask_t m, int i, int rightEnd, Mask_t visMask) // static
{
    auto stateMask = getStateMask(m, i);
    int left = getPos(m, i);

    bool cycleDetected = ithBit(visMask, i);
    bool treeDetected = preprocessingLeft[stateMask] != -1;
    bool invalidEntry = left >= MicrosetSize; 
    
    // m is invalid mask
    if (cycleDetected or treeDetected or invalidEntry)
        return -1;

    visMask |= 1u << i;
    auto leftStateMask = getStateMask(m, left);
    
    preprocessingLeft[stateMask] = left == i ? i : preprocessingRec(m, left, rightEnd, visMask);
    preprocessingRight[stateMask] = rightEnd;
    preprocessingPosition[stateMask] = preprocessingPosition[leftStateMask] + 1;
    
    return preprocessingLeft[stateMask];
}

void Microset::init(int m) // static
{
    MicrosetSize = BITS_USED(BITS_USED(m));
    roundupToEven(MicrosetSize);
    bitsPerEntry = BITS_USED(MicrosetSize);
    MicrosetSplitter::init(MicrosetSize);

    int bitsPerMicroset = bitsPerEntry * MicrosetSize;
    int bitsPerState = bitsPerMicroset + bitsPerEntry;

    preprocessingLeft.resize(1 << bitsPerState, -1);
    preprocessingRight.resize(1 << bitsPerState, -1);
    preprocessingPosition.resize(1 << bitsPerState, -1);

    for (Mask_t m = 0; m < (1 << bitsPerMicroset); m++)
    {
        Mask_t notRightMarginalsMask = 0;
        for (int i = 0; i < MicrosetSize; i++)
        {
            int left = getPos(m, i);
            if (left < MicrosetSize and left != i)
                notRightMarginalsMask |= 1u << left;
        }
        for (int i = 0; i < MicrosetSize; i++)
            if (!ithBit(notRightMarginalsMask, i))
                preprocessingRec(m, i, i, 0);
    }
    for (int i = 0; i < MicrosetSize; i++)
        updateMask(emptyMicroMask, i, i);
}


void Microset::setLeft(int pos, int val)
{
    updateMask(mask, pos, val);
    L[pos] = val;
}

void Microset::setRight(int pos, int val)
{   R[pos] = val;   }

Mask_t Microset::getStateMask(int i) const
{   return getStateMask(mask, i);   }

int Microset::getLeft(int pos) const
{   return L[pos];  }

int Microset::getRight(int pos) const
{   return R[pos];  }

int Microset::size() const
{   return edges.size();    }

Iter_t <Path> Microset::microLabel() const
{   return father->father;  }

Element& Microset::getEdge(int index) const
{   return E[edges[index]]; }

bool Microset::lastsSubpathMarginal() const
{
    int leftId = borderEdgeWithinSubpath(size() - 1, true);
    int rightId = borderEdgeWithinSubpath(size() - 1, false);
    return E[leftId].isMarginal() or E[rightId].isMarginal();
}

Iter_t <Path> Microset::edgeLabel(int index) const
{
    int stateMask = getStateMask(index);
    int leftIndex = preprocessingLeft[stateMask];
    int rightIndex = preprocessingRight[stateMask];
    Element& left = getEdge(leftIndex);
    Element& right = getEdge(rightIndex);

    if (left.isMarginal())
        return left.pathPtr;
    if (right.isMarginal())
        return right.pathPtr;
    return microLabel();
}

bool Microset::labelConsistent(int index) const
{   return microLabel() == edgeLabel(index);    }

int Microset::borderEdgeWithinSubpath(int index, bool leftSide) const
{
    vector <int>& preproc = leftSide ? preprocessingLeft : preprocessingRight;
    auto m = getStateMask(index);
    int borderIndex = preproc[m];
    return edges[borderIndex];
}

int Microset::leftmostOfLabelSubpath() const
{
    for (int i = 0; i < edges.size(); i++) if (getLeft(i) == i)
    {
        int leftId = edges[i];
        int rightId = borderEdgeWithinSubpath(i, false);
        if (!E[leftId].isMarginal() and !E[rightId].isMarginal())
            return leftId;
    }
    assert(false);
}


void Microset::connect(int l, int r)
{
    setLeft(r, l);
    setRight(l, r);
}

void Microset::insertAfter(int index, Element& fresh)
{
    fresh.placeInMicro(selfptrInFather);
    int indexFresh = fresh.indexInMicro;
    
    if (!getEdge(index).isRightMarginalInMicro())
        connect(indexFresh, getRight(index));

    connect(index, indexFresh);

    if (size() == MicrosetSize)
        split();
}

void Microset::insertBefore(int index, Element& fresh)
{
    fresh.placeInMicro(selfptrInFather);
    int indexFresh = fresh.indexInMicro;
    
    if (!getEdge(index).isLeftMarginalInMicro())
        connect(getLeft(index), indexFresh);

    connect(indexFresh, index);

    if (size() == MicrosetSize)
        split();
}

void Microset::clear()
{
    mask = emptyMicroMask;
    edges.resize(0);
    L.resize(0);
    R.resize(0);
}

void Microset::split()
{
    MicrosetSplitter(selfptrInFather).split();
}

void Microset::breakPath(int index, bool leftDirection, bool allowMicroRelabel)
{
    if (labelConsistent(index) and allowMicroRelabel)
        unlabeledMicrosets->anectMicroset(selfptrInFather);

    if (leftDirection)
    {
        int left = getLeft(index);
        if (left != index)
            setRight(left, left);
        setLeft(index, index);
    }
    else
    {
        int right = getRight(index);
        if (right != index)
            setLeft(right, right);
        setRight(index, index);
    }
}
