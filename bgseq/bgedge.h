#ifndef __BGEDGE__
#define __BGEDGE__

#include <list>
using std::list;

struct EdgeBG
{
    int v, u;
    list <EdgeBG*> :: iterator pv, pu;

    EdgeBG(int a, int b);

    int& other(int x);

    list <EdgeBG*> ::iterator& getIt(int x);

    void smooth(EdgeBG* e, int x);

    void print();

    ~EdgeBG();
};

void makeBGedge(int a, int b);

#endif // __BGEDGE__
