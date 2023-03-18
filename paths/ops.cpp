#include "ops.h"
#include "element.h"
#include "micro.h"
#include "mezzo.h"
#include "path.h"
#include <cassert>

extern std::vector <Element> E;

Iter_t <Universe> universe;
Path::SelfIter_t nullPath;
Mezzoset::SelfIter_t unlabeledMicrosets;
Microset::SelfIter_t nullMicroset;

std::vector <Path::SelfIter_t> pathPointers;
std::list <Universe> universeWrapper { Universe() };

void setupMarginalEdges(Path::SelfIter_t path, int left, int right)
{
    E[left].markAsMarginal(path, true);
    E[right].markAsMarginal(path, false);
}

int labelNewPath(Path::SelfIter_t p, Path::SelfIter_t q)
{
    if (p->id != 0)
        swap(p, q);
    pathPointers.push_back(p);
    return p->id = pathPointers.size();
}

void connect(int leftId, int rightId, bool connectInMicro = false)
{
    auto& edgeLeft = E[leftId];
    auto& edgeRight = E[rightId];
    edgeLeft.right = rightId;
    edgeRight.left = leftId;

    if (connectInMicro and edgeLeft.father == edgeRight.father)
    {
        auto microset = edgeLeft.father;
        int i1 = edgeLeft.indexInMicro;
        int i2 = edgeRight.indexInMicro;
        microset->connect(i1, i2);
    }
}

void replace(Element& edgeOld, Element& edgeFresh)
{
    auto path = edgeOld.pathPtr;

    assert(edgeFresh.id != 0);

    if (edgeOld.isLeftMarginal())
        edgeFresh.markAsMarginal(path, true);
    else
        connect(edgeOld.left, edgeFresh.id);

    if (edgeOld.isRightMarginal())
        edgeFresh.markAsMarginal(path, false);
    else
        connect(edgeFresh.id, edgeOld.right);

    edgeOld.replaceInMicroWith(edgeFresh);
    edgeOld.clear();
}

void insertAfter(Element& edgeOld, Element& edgeFresh)
{
    auto path = edgeOld.pathPtr;

    if (edgeOld.isRightMarginal())
        edgeFresh.markAsMarginal(path, false);
    else
        connect(edgeFresh.id, edgeOld.right);

    connect(edgeOld.id, edgeFresh.id);
    edgeOld.unmarkAsMarginal();

    auto microset = edgeOld.father;
    microset->insertAfter(edgeOld.indexInMicro, edgeFresh);
}

void insertBefore(Element& edgeOld, Element& edgeFresh)
{
    auto path = edgeOld.pathPtr;

    if (edgeOld.isLeftMarginal())
        edgeFresh.markAsMarginal(path, true);
    else
        connect(edgeOld.left, edgeFresh.id);

    connect(edgeFresh.id, edgeOld.id);
    edgeOld.unmarkAsMarginal();

    auto microset = edgeOld.father;
    microset->insertBefore(edgeOld.indexInMicro, edgeFresh);
}

Element& createFakeEdge()
{
    int edgeId = E.size();
    E.push_back(Element(edgeId));
    return E.back();
}

namespace PathsStructure {

int newPath(int e)
{
    if (unlabeledMicrosets->empty())
        Microset::create(unlabeledMicrosets);

    auto newPath = Path::create(universe);
    auto microset = unlabeledMicrosets->last();
    E[e].placeInMicro(microset);
    setupMarginalEdges(newPath, e, e);
    
    if (microset->size() == Microset::MicrosetSize)
        microset->split();

    return labelNewPath(newPath, newPath);
}

void init(int m)
{
    Microset::init(m);
    Mezzoset::MezzosetSize = BITS_USED(m) / Microset::MicrosetSize;
    Mezzoset::MezzosetSize = std::max(Mezzoset::MezzosetSize, 2);

    universe = universeWrapper.begin();
    nullPath = Path::create(universe);
    unlabeledMicrosets = Mezzoset::create(nullPath);
    nullMicroset = unlabeledMicrosets->end();
    nullPath->id = -1;
    
    for (int i=0; i<m; i++)
        E.push_back(Element(i));
}

int find(int e)
{
    auto path = E[e].find();
    return path->id;
}

void insertVertex(int e, int f, int g) // e -> f|g
{
    auto& oldEdge = E[e];
    int leftId = oldEdge.left, rightId = oldEdge.right;

    bool swapNeeded = E[leftId].incident(E[g]) or E[rightId].incident(E[f]);
    if (swapNeeded)
        std::swap(f, g);

    auto& freshLeft = E[f];
    auto& freshRight = E[g];

    replace(oldEdge, freshLeft);
    insertAfter(freshLeft, freshRight);
}

void addVertex(int e, int f)
{
    auto& oldEdge = E[e];
    auto& freshEdge = E[f];
    bool insertingAfter = oldEdge.isRightMarginal();
    assert(oldEdge.isMarginal());

    if (insertingAfter)
        insertAfter(oldEdge, freshEdge);
    else
        insertBefore(oldEdge, freshEdge);
}

std::vector <int> getPath(int id)
{
    auto path = pathPointers[--id];
    auto& e0 = E[path->leftEdge];
    auto [a, b] = e0.getVertices();

    int second = e0.right;
    if (!E[second].incident(b))
        std::swap(a, b);

    std::vector <int> vertices {a, b};
    for (int i = second; i != 0; i = E[i].right)
    {
        auto [a, b] = E[i].getVertices();
        vertices.push_back(a ^ b ^ vertices.back());
    }
    return vertices;
}

bool isSingleEdge(int e)
{   return E[e].isLeftMarginal() and E[e].isRightMarginal();    }

bool isBorderVertex(int e, int v)
{
    auto path = E[e].find();
    auto& left = E[path->leftEdge];
    auto& right = E[path->rightEdge];

    if (left.incident(v) and !E[left.right].incident(v))
        return true;
    if (right.incident(v) and !E[right.left].incident(v))
        return true;
    return false;
}

void eraseShort(int e)
{
    auto& edge = E[e];
    auto micro = edge.father;
    assert(isSingleEdge(e));

    if (micro->size() == 1)
    {
        assert(micro->father == unlabeledMicrosets);
        micro->destroy();
    }
    else
        replace(edge, createFakeEdge());
}
}
