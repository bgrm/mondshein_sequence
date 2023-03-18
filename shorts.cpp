#include "shorts.h"
#include <cassert>

namespace {
struct EdgeIndexed {
    int first, second, ind;

    EdgeIndexed(const Edge& e = std::make_pair(0, 0), int i = 0)
        : first(e.first)
        , second(e.second)
        , ind(i)
    {
        if (first > second)
            std::swap(first, second);
    }

    Edge toEdge()
    {
        return Edge { first, second };
    }

    bool equal(const Edge& e)
    {
        return edgeEq(e, toEdge());
    }
};

vector<Edge> E;
bool* isShort;
}

vector<BGopEx> getBGopsExteneded(const vector<BGop>& base, int n)
{
    vector<EdgeIndexed> all;
    for (auto& [f, g, h] : base) {
        auto [v, w] = f;
        auto [a, b] = g;
        auto [c, d] = h;
#define PUSH(i, j) all.push_back(EdgeIndexed({ i, j }, SZ(all)))
        PUSH(v, w);
        PUSH(a, b);
        PUSH(c, d);
        PUSH(a, v);
        PUSH(v, b);
        PUSH(c, w);
        PUSH(w, d);
#undef  PUSH
    }
    sweep(all, n, 1), sweep(all, n, 0);

    vector<BGopEx> ret(SZ(base));
    E.push_back({ -1, -1 });

    for (auto& e : all) {
        int i = e.ind / BGOP_SIZE, j = e.ind % BGOP_SIZE;
        if (!e.equal(E.back()))
            E.push_back(e.toEdge());
        ret[i][j] = SZ(E) - 1;
    }
    for (int i = 0; i < SZ(base); i++) {
        if (ARG(base[i], 0) != getEdge(ret[i][0])) {
            std::swap(ret[i][1], ret[i][2]);
            std::swap(ret[i][3], ret[i][5]);
            std::swap(ret[i][4], ret[i][6]);
        }
    }

    all.clear();
    isShort = new bool[SZ(E)]();
    return ret;
}

Edge getEdge(int i)
{
    return E[i];
}

int getInd(const Edge& e, const BGopEx& op)
{
    for (int i : op)
        if (edgeEq(e, E[i]))
            return i;
    assert(false);
}

void insertShort(int i)
{
    isShort[i] = true;
}

void insertShort(const Edge& e, const BGopEx& op)
{
    insertShort(getInd(e, op));
}

void removeShort(int i)
{
    isShort[i] = false;
}

void removeShort(const Edge& e, const BGopEx& op)
{
    removeShort(getInd(e, op));
}

bool shortEar(int i)
{
    return isShort[i];
}

bool shortEar(const Edge& e, const BGopEx& op)
{
    return shortEar(getInd(e, op));
}

vector<Edge> getAllShorts()
{
    vector<Edge> ret;
    for (int i = 0; i < SZ(E); i++)
        if (isShort[i])
            ret.push_back(E[i]);
    return ret;
}

void clearShorts()
{
    E.clear();
    delete isShort;
}
