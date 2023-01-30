#include "inter.h"
#include <algorithm>
#include <cstdio>
#include <stack>

namespace {
vector<vector<Interval*>> T;

/* applies positional sort to I according to the coor */
void sortPos(vector<Interval*>& I, int range, int coor)
{
    T.resize(range);
    for (Interval* i : I)
        T[coor == 0 ? i->a : i->b].push_back(i);
    I.clear();
    for (auto& vec : T) {
        I.insert(I.end(), vec.begin(), vec.end());
        vec.clear();
    }
}

void sortAndReverse(vector<Interval*>& I, int range)
{
    sortPos(I, range, 1);
    sortPos(I, range, 0);
    reverse(I.begin(), I.end());
}

void reverseEqualSubsegments(vector<Interval*>& vec)
{
    decltype(vec.end()) jt;
    for (auto it = vec.begin(); it != vec.end(); it = jt) {
        jt = std::find_if(it, vec.end(), [it](Interval* i) { return i->b != (*it)->b; });
        std::reverse(it, jt);
    }
}

/* renums intervals in I so that no endpoint is shared by 2 intervals 
and interlocking relation is preserved */
void renum(vector<Interval*>& I, int& range)
{
    vector<vector<Interval*>> starts(range), ends(range);
    sortAndReverse(I, range);

    for (Interval* i : I)
        starts[i->a].push_back(i), ends[i->b].push_back(i);

    int offset = 0;

    for (int x = 0; x < range; x++) {
        reverseEqualSubsegments(starts[x]);
        // so that equal segments become nested instead of cutting

        for (Interval* i : ends[x])
            i->b += offset++;
        for (Interval* i : starts[x])
            i->a += offset++;
    }
    range += offset;
}
}

void Interval::print()
{
    printf("[%d-%d,  %d]\n", a, b, tag);
}

Graph Interlock::makeEdges(vector<Interval*>& I, int range, int n)
{
    Graph G(n);
#define CONN(it, jt) G[(it)->tag].push_back((jt)->tag), \
                     G[(jt)->tag].push_back((it)->tag)
    renum(I, range);
    sortAndReverse(I, range);

    Interval border = Interval(range, range, 0);
    std::stack<Interval*> S;
    S.push(&border);

    for (Interval* i : I) // making edges in the F tree
    {
        while (S.top()->b < i->b)
            S.pop();
        if (S.top()->a < i->b)
            CONN(S.top(), i);
        S.push(i);
    }

    sortPos(I, range, 1);
    while (S.size() > 1)
        S.pop();
    border.a = border.b = -1;

    for (Interval* i : I) // making edges in the F' tree
    {
        while (S.top()->a > i->a)
            S.pop();
        if (S.top()->b > i->a)
            CONN(i, S.top());
        S.push(i);
    }
    for (Interval* i : I)
        delete i;

    return G;
#undef CONN
}
