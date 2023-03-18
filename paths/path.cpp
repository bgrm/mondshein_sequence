#include "path.h"
#include "mezzo.h"
#include "micro.h"
#include "element.h"
#include <vector>

extern std::vector <Element> E;

void Path::anectMicroset(Iter_t <Microset> micro)
{
    int leftEdgeIndex = E[leftEdge].indexInMicro;
    int borderEdge = leftMicro()->borderEdgeWithinSubpath(leftEdgeIndex, false);
    int microLeftmostEdge = micro->leftmostOfLabelSubpath();
    bool atStart = E[borderEdge].incident(E[microLeftmostEdge]);

    if (empty())
        Mezzoset::create(selfptrInFather);

    (atStart ? first() : last())->anectMicroset(micro, !atStart);
}

Iter_t <Microset> Path::leftMicro() const
{   return E[leftEdge].father;  }

Iter_t <Microset> Path::rightMicro() const
{   return E[rightEdge].father; }
