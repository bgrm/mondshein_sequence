#include <vector>
#include <cstdio>
#include <cassert>
#include "bgedge.h"
extern std::vector <list <EdgeBG*> > H;

EdgeBG::EdgeBG(int a, int b)
: v(a), u(b)
{
    H[v].push_back(this);
    H[u].push_back(this);
    pv = prev(H[v].end());
    pu = prev(H[u].end());
}

int& EdgeBG::other(int x)
{   return x == v ? u : v;  }

list <EdgeBG*> ::iterator& EdgeBG::getIt(int x)
{   return x == v ? pv : pu;    }

// x has deg 2: this & e
void EdgeBG::smooth(EdgeBG* e, int x)
{
    int a = other(x), b = e->other(x);
    auto newIt = H[b].insert(e->getIt(b), this);
    delete e;
    getIt(x) = newIt;
    other(a) = b;
    assert(H[x].size() == 1);
    H[x].clear();
}

void EdgeBG::print()
{
    printf("<%d-%d>\n", u, v);
}

EdgeBG::~EdgeBG()
{
    H[v].erase(pv);
    H[u].erase(pu);
}

void makeBGedge(int v, int u)
{
    new EdgeBG(v, u);
}
