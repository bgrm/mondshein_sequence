#ifndef __INTER__
#define __INTER__

#include <vector>
using std::vector;

using Graph = vector <vector <int> >;

struct Interval
{
    int a, b, tag;

    Interval(int _a, int _b, int _tag)
    : a(_a), b(_b), tag(_tag) {};
    void print();
};

namespace Interlock
{
    Graph makeEdges(vector <Interval*>& I, int range, int n);
}

#endif
