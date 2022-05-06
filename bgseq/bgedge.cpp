#include "bgedge.h"
#include "debug.h"

extern vector <list <Edge_priv*> > H;


int& Edge_priv::other(int x)
{   return x == v ? u : v;  }

list <Edge_priv*> ::iterator& Edge_priv::getIt(int x)
{   return x == v ? pv : pu;    }

void Edge_priv::erase()
{
    H[v].erase(pv);
    H[u].erase(pu);
    delete this;
}

// x has deg 2: this & e
void Edge_priv::smooth(Edge_priv* e, int x)
{
    int a = other(x), b = e->other(x);
    getIt(x) = e->getIt(b);
    *getIt(x) = this;
    other(a) = b;

    delete e;
    H[x].clear();
}

void Edge_priv::print()
{
    printf("<%d-%d>\n", u, v);
}

void makeBGedge(int v, int u)
{
    Edge_priv* e = new Edge_priv(v, u);
    H[v].push_back(e);
    H[u].push_back(e);
    e->pv = prev(H[v].end());
    e->pu = prev(H[u].end());
}