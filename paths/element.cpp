#include "element.h"
#include "micro.h"
#include "mezzo.h"
#include "path.h"
#include "../shorts.h"

extern Path::SelfIter_t nullPath;
extern Mezzoset::SelfIter_t unlabeledMicrosets;
extern Microset::SelfIter_t nullMicroset;
    
std::vector <Element> E;


bool Element::isMarginal() const
{   return pathPtr != nullPath;    }

bool Element::isLeftMarginal() const
{   return left == 0;   }

bool Element::isRightMarginal() const
{   return right == 0;  }

bool Element::isLeftMarginalInMicro() const
{
    return father->getLeft(indexInMicro) == indexInMicro;
}

bool Element::isRightMarginalInMicro() const
{
    return father->getRight(indexInMicro) == indexInMicro;
} 

Iter_t <Path> Element::find() const
{
    return father->edgeLabel(indexInMicro);
}

bool Element::isInConsistentMezzo() const
{
    return father->labelConsistent(indexInMicro);
}

int Element::positionInSubpath() const
{
    int mask = father->getStateMask(indexInMicro);
    return Microset::preprocessingPosition[mask];
}

bool Element::earlierThan(const Element& other) const
{
    assert(find() == other.find());

    if (isInConsistentMezzo() and other.isInConsistentMezzo())
    {   
        if (mezzo() != other.mezzo())
            return mezzo()->earlierThan(*other.mezzo());
        if (father != other.father)
            return father->earlierThan(*other.father);
        
        // at this point this and other are in the same micro, which is in consistent mezzo
    }

    // at least one edge is in marginal micro, but they're in diffrent micros
    else if (father != other.father)
    {
        auto path = find();
        return father == path->leftMicro() or other.father == path->rightMicro();
    }

    // the same micro, either marginal or nor
    return positionInSubpath() < other.positionInSubpath();
}

Mezzoset::SelfIter_t Element::mezzo() const
{   return father->father;  }

std::pair <int, int> Element::getVertices() const
{   return getEdge(id); }

bool Element::incident(int v) const
{
    auto [a, b] = getVertices();
    return a == v or b == v;
}

bool Element::incident(const Element& other) const
{
    auto [v, w] = other.getVertices();
    return incident(v) or incident(w);
}

int Element::getLinkingVertex(const Element& other) const
{
    auto [v, w] = other.getVertices();

    if (incident(v))
        return v;
    if (incident(w))
        return w;

    assert(false);
}

void Element::unmarkAsMarginal()
{
    if (!isLeftMarginal() and !isRightMarginal())
        pathPtr = nullPath;
}

void Element::markAsMarginal(Iter_t <Path> p, bool leftEnd)
{
    pathPtr = p;
    (leftEnd ? p->leftEdge : p->rightEdge) = id;
    (leftEnd ? left : right) = 0;
}

void Element::clear()
{
    father = nullMicroset;
    pathPtr = nullPath;
    indexInMicro = left = right = 0;
}

void Element::placeInMicro(Iter_t <Microset> micro, int index)
{
    if (index == -1)
    {
        index = micro->edges.size();
        micro->edges.push_back(id);
        micro->L.push_back(index);
        micro->R.push_back(index);
    }
    else
        micro->edges[index] = id;
    
    father = micro;
    indexInMicro = index;
}

void Element::replaceInMicroWith(Element& other)
{
    other.placeInMicro(father, indexInMicro);
}

void Element::breakPath(bool leftDirection, bool allowMicroRelabel)
{
    father->breakPath(indexInMicro, leftDirection, allowMicroRelabel);
}
