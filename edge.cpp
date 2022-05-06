#include "edge.h"
#include <cassert>

namespace
{
    struct EdgeExtended
    {
        int a, b, ind;

        EdgeExtended(const Edge& e = std::make_pair(0,0), int i = 0)
        : a(e.first), b(e.second), ind(i)
        {
            if (a > b)
                std::swap(a, b);
        }

        Edge toEdge()
        {   return Edge {a, b}; }

        bool equal(const Edge& e)
        {   return edgeEq(e, toEdge()); }
    };

    void sweep(vector <EdgeExtended>& E, int n, bool coor)
    {
        vector <vector <EdgeExtended> > bucket (n+1);
        for (auto e : E)
            bucket[coor ? e.b : e.a].push_back(e);
        E.clear();
        for (int v=0; v<=n; v++)
        {
            E.insert(E.end(), bucket[v].begin(), bucket[v].end());
            bucket[v].clear();
        }
    }

    vector <Edge> E;
    bool* isShort;
}

vector <BGopEx> getBGopsExteneded(const vector <BGop>& base, int n)
{
    vector <EdgeExtended> all;
    for (auto& [f, g, h] : base)
    {
        #define PUSH(i, j) all.push_back(EdgeExtended({i, j}, SZ(all)))
        auto [v, w] = f;
        auto [a, b] = g;
        auto [c, d] = h;
        PUSH(v, w);
        PUSH(a, b);
        PUSH(c, d);
        PUSH(a, v);
        PUSH(v, b);
        PUSH(c, w);
        PUSH(w, d);
        #undef PUSH
    }
    sweep(all, n, 1), sweep(all, n, 0);

    vector <BGopEx> ret(SZ(base));
    E.push_back({-1, -1});

    for (auto& e : all)
    {
        int i = e.ind / bgopSize, j = e.ind % bgopSize;
        if (!e.equal(E.back()))
            E.push_back(e.toEdge());
        ret[i][j] = SZ(E) - 1;
    }
    all.clear();
    isShort = new bool [SZ(E)]();
    return ret;
}

Edge getEdge(int i)
{   return E[i];    }

int getInd(const Edge& e, BGopEx* op)
{
    for (int i : *op)
        if (edgeEq(e, E[i]))
            return i;
    assert(false);
}

void insertShort(int i)
{	isShort[i] = true;	}

void insertShort(const Edge& e, BGopEx* op)
{	insertShort(getInd(e, op));	}

void removeShort(int i)
{	isShort[i] = false;	}

void removeShort(const Edge& e, BGopEx* op)
{	removeShort(getInd(e, op));	}

bool shortEar(int i)
{	return isShort[i];	}

bool shortEar(const Edge& e, BGopEx* op)
{	return shortEar(getInd(e, op));	}

vector <Edge> getAllShorts()
{
    vector <Edge> ret;
    for (int i=0; i<SZ(E); i++)
        if (isShort[i])
            ret.push_back(E[i]);
    return ret;
}

void clearShorts()
{
    E.clear();
    delete isShort;
}
