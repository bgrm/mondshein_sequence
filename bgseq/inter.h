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

template <typename TT>
void rev(vector <TT>& T);

namespace Interlock
{
    /* applies positional sort to I according to the coor */
    void sortPos(vector <Interval*>& I, int range, int coor);

    /* renums intervals in I so that no endpoint is shared by 2 intervals 
    and interlocking relation is preserved */
    void renum(vector <Interval*>& I, int& range);

    Graph makeEdges(vector <Interval*>& I, int range, int n);
}

#endif
