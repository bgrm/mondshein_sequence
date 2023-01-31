#ifndef __LIB__
#define __LIB__

#include <algorithm>
using std::pair;

struct Edge;

struct Path // STL-list doesn't allow splitting in O(1)
{
    Edge* start;
    int id, lastVertex;

    Path(Edge* const e);
    void swap(Path& other);
    void print();
    ~Path();
};

struct Edge {
    int lv, rv;
    Edge* nxt;
    Path* path;

    Edge(int a = 0, int b = 0, Edge* c = nullptr, Path* d = nullptr);
    void subdivide(int v);
    pair<int, int> toPair();
    void print();
};

void saveIter(int a, int b, Edge* e);
void rmvIter(int a, int b);
Edge* getIter(pair<int, int> e);
void saveIncEdge(int v, Edge* e);

#endif // __LIB__