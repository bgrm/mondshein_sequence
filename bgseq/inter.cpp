#include "inter.h"
#include "decom.h"
#include "debug.h"

template <typename TT>
void rev(vector <TT>& T)
{   reverse(T.begin(), T.end());    }

void revInstances()
{
    vector <int> a;
    rev(a);
    vector <pair <Vertex*, Vertex* > > b;
    rev(b);
}

vector <vector <Interval*> > T;

void Interval::print()
{
    printf("[%d-%d,  %d]\n", a, b, tag);
}

/* applies positional sort to I according to the coor */
void Interlock::sortPos(vector <Interval*>& I, int range, int coor)
{
    T.resize(range);
    for (Interval* i : I)
        T[coor == 0 ? i->a : i->b].push_back(i);
    I.clear();
    for (int c=0; c<range; c++)
    {
        for (Interval* i : T[c])
            I.push_back(i);
        T[c].clear();
    }
}

/* renums intervals in I so that no endpoint is shared by 2 intervals 
and interlocking relation is preserved */
void Interlock::renum(vector <Interval*>& I, int& range)
{
    sortPos(I, range, 0);
    sortPos(I, range, 1);
    vector <vector <Interval*> > starts(range), ends(range);

    for (Interval* i : I)
        starts[i->a].push_back(i), ends[i->b].push_back(i);

    int offset = 0;

    for (int x=0; x<range; x++)
    {
        rev(starts[x]);
        rev(ends[x]);
        for (Interval* i : ends[x])
            i->b += offset++;
        for (Interval* i : starts[x])
            i->a += offset++;
    }
    range += offset;
}

Graph Interlock::makeEdges(vector <Interval*>& I, int range, int n)
{
    Graph G(n);
    #define CONN(it, jt) G[(it) -> tag].push_back((jt) -> tag), \
                        G[(jt) -> tag].push_back((it) -> tag)
    renum(I, range);
    sortPos(I, range, 0);
    rev(I);
    
    Interval border = Interval(range, range, 0);
    stack <Interval*> S;
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
