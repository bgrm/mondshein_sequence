#include "lib.h"
#include <unordered_map>
#include <vector>
<<<<<<< HEAD
#include <cstdio>
#include "lib.h"
=======
>>>>>>> 32adb5708db2bc4b13a5a929ffb8e1ad6aa1c65b
using std::vector;

extern vector<Path*> paths;
extern vector<Edge*> incidentEdge;

Path::Path(Edge* const e)
    : start(e)
    , id(paths.size() + 1)
    , lastVertex(e->rv)
{
    paths.push_back(this);
}

void Path::swap(Path& other)
{
    std::swap(start, other.start);
    std::swap(id, other.id);
    std::swap(lastVertex, other.lastVertex);
    std::swap(paths[id - 1], paths[other.id - 1]);
}

void Path::print()
{
    printf("PATH at %lld:\n", (long long)this);

    for (Edge* e = start; e; e = e->nxt)
        e->print();
    printf("\n======================================\n");
}

Path::~Path()
{
    while (start) {
        Edge* nxt = start->nxt;
        delete start;
        start = nxt;
    }
}

/*************************************************************/

#define MRG(a, b) ((int64_t)(a) << 32 | (b))

std::unordered_map<int64_t, Edge*> pairToIterator; // there's no default hash function for pairs

void saveIter(int a, int b, Edge* e)
{
    int64_t ab = MRG(a, b);
    pairToIterator[ab] = e;
}

void rmvIter(int a, int b)
{
    int64_t ab = MRG(a, b);
    int64_t ba = MRG(b, a);
    pairToIterator.erase(ab);
    pairToIterator.erase(ba);
}

Edge* getIter(pair<int, int> e)
{
    auto [a, b] = e;
    int64_t ab = MRG(a, b);
    int64_t ba = MRG(b, a);
    auto ret = pairToIterator.find(ab);
    if (ret == pairToIterator.end())
        ret = pairToIterator.find(ba);
    return ret->second;
}

/*************************************************************/

Edge::Edge(int a, int b, Edge* c, Path* d)
    : lv(a)
    , rv(b)
    , nxt(c)
    , path(d)
{
    saveIter(lv, rv, this);
}

void Edge::subdivide(int v)
{
    nxt = new Edge(v, rv, nxt, path);

    incidentEdge[v] = this;
    if (nxt->nxt)
        incidentEdge[rv] = nxt;

    rmvIter(lv, rv);
    rv = v;
    saveIter(lv, rv, this);
}

pair<int, int> Edge::toPair()
{
    return { lv, rv };
}

void Edge::print()
{
    printf("%lld: [%d-%d, nxt=%lld, pth=%lld]\n", (long long)this, lv, rv, (long long)nxt, (long long)path);
}
