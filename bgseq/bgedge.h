#ifndef __EBGE__
#define __EBGE__

#include <list>
using std::list;

struct Edge_priv
{
    int v, u;
    list <Edge_priv*> :: iterator pv, pu;

    Edge_priv(int a, int b)
    : v(a), u(b) {}

    int& other(int x);

    list <Edge_priv*> ::iterator& getIt(int x);

    void erase();

    void smooth(Edge_priv* e, int x);

    void print();
};

void makeBGedge(int a, int b);

#endif // __EBGE__
