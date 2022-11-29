#include "paths.h"
#include "lib.h"
#include <cassert>
#include <unordered_map>

vector<Path*> paths;
vector<Edge*> incidentEdge;

namespace {
/* A, B non empty.
	* Edges of both point to one of them.
	* we change path-pointers in shorter of them.
	*/
void renameShorter(Path& A, Path& B)
{
    Edge *a = A.start, *b = B.start;
    while (a != nullptr and b != nullptr)
        a = a->nxt, b = b->nxt;
    Path* C = a == nullptr ? &A : &B; // shorter path

    if (C->start->path == C) {
        A.swap(B);
        C = C == &A ? &B : &A;
    }
    for (Edge* e = C->start; e; e = e->nxt)
        e->path = C;
}

/* we increment e and f simultanousley until finding the end of the path or another edge */
bool inOrder(Edge* e, Edge* f) // whether e is on left of f
{
    Edge *e0 = e, *f0 = f;
    while (true) {
        if (e->nxt == f0 or f->nxt == nullptr)
            return true;
        if (e->nxt == nullptr or f->nxt == e0)
            return false;
        e = e->nxt;
        f = f->nxt;
    }
}

bool internalWith(int v, pair<int, int> e)
{
    auto it = incidentEdge[v];
    return it != nullptr and it->path->id == PathsStructure::find(e);
}

// exactly one of {u, v} is a border vertex
int insertEdgeCorner(pair<int, int> edge, int u, int v)
{
    if (internalWith(u, edge))
        std::swap(u, v);
    Edge* e = incidentEdge[v];
    int firstVertex = e->path->start->lv;
    edge = e->toPair();

    if (u == firstVertex) {
        edge = e->nxt->toPair();
        edge = { edge.second, edge.first };
    }

    int ret = PathsStructure::split(edge, v);
    PathsStructure::addVertex(edge, u);
    return ret;
}
}

extern std::unordered_map<int64_t, Edge*> pairToIterator;

namespace PathsStructure {
void setN(int n)
{
    incidentEdge.resize(n + 1, nullptr);
}

int newPath(pair<int, int> e)
{
    auto [a, b] = e;
    Edge* edge = new Edge(a, b);
    Path* newPath = new Path(edge);
    edge->path = newPath;
    return newPath->id;
}

int split(pair<int, int> _, int v)
{
    Edge* e = incidentEdge[v];
    Path* newPath = new Path(e->nxt);
    Path* oldPath = e->path;
    newPath->lastVertex = oldPath->lastVertex;
    oldPath->lastVertex = e->rv;
    e->nxt = nullptr;
    int ret = newPath->id;
    renameShorter(*newPath, *(e->path));
    return ret;
}

int insertEdge(pair<int, int> edge, int u, int v)
{
    if (!internalWith(u, edge) or !internalWith(v, edge))
        return insertEdgeCorner(edge, u, v);

    Edge *e = incidentEdge[u], *f = incidentEdge[v];
    if (!inOrder(e, f))
        std::swap(e, f), std::swap(u, v);

    // u is on left of v. u and v are the right vertices of e and f respectively

    Path* oldPath = e->path;
    Path* newPath = new Path(e->nxt);
    Edge* newEdge = new Edge(u, v, f->nxt, oldPath);
    newPath->lastVertex = v;
    e->nxt = newEdge;
    f->nxt = nullptr;
    int ret = newPath->id;
    incidentEdge[v] = newEdge;
    renameShorter(*newPath, *(e->path));
    return ret;
}

int find(pair<int, int> e)
{
    Edge* edge = getIter(e);
    return edge->path->id;
}

void insertVertex(pair<int, int> e, int v)
{
    getIter(e)->subdivide(v);
}

// v is to be neighboring with e.second
void addVertex(pair<int, int> e, int v)
{
    auto [_, b] = e;
    Edge* edge = getIter(e);
    Path* p = edge->path;

    if (edge->rv == b) // adding at the end of the path
    {
        edge->nxt = new Edge(b, v, nullptr, p);
        incidentEdge[b] = edge;
        p->lastVertex = v;
    } else // adding at the beggining
    {
        p->start = new Edge(v, b, edge, p);
        incidentEdge[b] = p->start;
    }
}

pair<int, int> borderEdge(int p)
{
    Edge* e = paths[p - 1]->start;
    return { e->lv, e->rv };
}

bool isSingleEdge(pair<int, int> e)
{
    return getIter(e)->path->start->nxt == nullptr;
}

vector<int> getPath(int p)
{
    Path* path = paths[p - 1];
    vector<int> ret { path->start->lv };

    for (Edge* e = path->start; e != nullptr; e = e->nxt)
        ret.push_back(e->rv);

    return ret;
}

void clear()
{
    for (auto p : paths)
        delete p;
    paths.clear();
    paths.shrink_to_fit();
    incidentEdge.clear();
    incidentEdge.shrink_to_fit();
    pairToIterator.clear();
}

bool isBorderVertex(pair<int, int> e, int v)
{
    Path* p = getIter(e)->path;
    return v == p->start->lv or v == p->lastVertex;
}

void eraseShort(pair<int, int> e)
{
    rmvIter(e.first, e.second);
}
}
