#ifndef __SHORTS__
#define __SHORTS__
#include "bgseq/decom.h"
#include <array>

constexpr auto BGOP_SIZE = 7;
using BGopEx = std::array<int, BGOP_SIZE>;

inline Edge flip(const Edge& e)
{
    return std::make_pair(e.second, e.first);
}

inline bool edgeEq(const Edge& e, const Edge& f)
{
    return e == f or e == flip(f);
}

template <typename TT>
inline void sweep(vector<TT>& T, int n, bool coor)
{
    vector<vector<TT>> bucket(n + 1);
    for (TT& e : T)
        bucket[coor ? e.second : e.first].push_back(e);
    T.clear();
    for (int v = 0; v <= n; v++) {
        T.insert(T.end(), bucket[v].begin(), bucket[v].end());
        bucket[v].clear();
    }
}

vector<BGopEx> getBGopsExteneded(const vector<BGop>& base, int n);

Edge getEdge(int i);

int getInd(const Edge& e, const BGopEx& op);

void insertShort(int i);

void insertShort(const Edge& e, const BGopEx& op);

void removeShort(int i);

void removeShort(const Edge& e, const BGopEx& op);

bool shortEar(int i);

bool shortEar(const Edge& e, const BGopEx& op);

vector<Edge> getAllShorts();

void clearShorts();

#endif // __SHORTS__
