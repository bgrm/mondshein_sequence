#include "sqrt.h"
#include <algorithm>
#include <cassert>
#include <cmath>

using std::next;

OrderSqrt::OrderSqrt(int n)
{
    d = std::max(4, (int)std::sqrt(++n));
    L.push_back({ 0 });
    P.resize(n);
    P[0] = 0;
}

void OrderSqrt::insert(int x, int y)
{
    auto* t = &L[P[x]];
    assert(!t->empty());
    auto it = std::find(t->begin(), t->end(), x);
    t->insert(next(it), y);
    P[y] = P[x];

    for (int i = P[x]; i < (int)L.size(); i++) {
        if (i == (int)L.size() - 1)
            L.push_back({});

        t = &L[i];
        if ((int)t->size() != d)
            break;
        int c = t->back();
        t->erase(prev(t->end()));
        P[c]++;
        L[i + 1].insert(L[i + 1].begin(), c);
    }
}

int OrderSqrt::predecessor(int x)
{
    auto& t = L[P[x]];
    if (t.front() == x)
        return L[P[x] - 1].back();
    else
        return *prev(std::find(t.begin(), t.end(), x));
}

int OrderSqrt::successor(int x)
{
    auto& t = L[P[x]];
    if (t.back() == x)
        return L[P[x] + 1].front();
    else
        return *next(std::find(t.begin(), t.end(), x));
}

void OrderSqrt::insertBefore(int x, int y)
{
    insert(predecessor(x), y);
}

bool OrderSqrt::compare(int x, int y)
{
    if (P[x] != P[y])
        return P[x] < P[y];
    bool ret = false;
    for (int z : L[P[x]]) {
        if (z == y)
            return ret;
        if (z == x)
            ret = true;
    }
    assert(false);
}

OrderSqrt::~OrderSqrt()
{
    for (auto& l : L)
        l.clear();
    L.clear();
    L.shrink_to_fit();
    P.clear();
    P.shrink_to_fit();
}