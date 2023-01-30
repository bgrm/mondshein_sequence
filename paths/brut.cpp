#include "brut.h"
#include "paths.h"
#include <cassert>
#include <vector>
using std::vector;

vector<list<int>> P;
int m;

bool match(Iter it, pair<int, int> e)
{
    auto [a, b] = e;
    return (*it == a and *next(it) == b) or (*it == b and *next(it) == a);
}

bool seek(list<int>& p, pair<int, int> e, int newV = -1)
{
    for (Iter it = p.begin(); next(it) != p.end(); it++) {
        if (match(it, e)) {
            if (newV != -1)
                p.insert(next(it), newV);
            return true;
        }
    }
    return false;
}

void extract(list<int>& p, Iter it, Iter jt)
{
    list<int> newp;
    newp.splice(newp.begin(), p, next(it), jt);
    newp.push_front(*it);
    if (jt != p.end())
        newp.push_back(*jt);
    P.push_back(newp);
}

int BRUT::find(pair<int, int> e)
{
    for (int i = 0; i < (int)P.size(); i++)
        if (seek(P[i], e))
            return i;
    assert(false);
}

int BRUT::newPath(pair<int, int> e)
{
    m++;
    auto [a, b] = e;
    P.push_back(list<int> { a, b });
    return P.size() - 1;
}

void BRUT::insertVertex(pair<int, int> e, int v)
{
    m++;
    for (auto& p : P)
        if (seek(p, e, v))
            return;
    assert(false);
}

void BRUT::addVertex(pair<int, int> e, int v)
{
    m++;
    auto [a, b] = e;
    for (auto& p : P) {
        if (match(p.begin(), e) and *p.begin() == b) {
            p.insert(p.begin(), v);
            return;
        }

        Iter last = prev(p.end());
        if (match(prev(last), e) and *last == b) {
            p.insert(p.end(), v);
            return;
        }
    }
    assert(false);
}

void BRUT::split(pair<int, int> e, int v)
{
    for (auto& p : P)
        if (seek(p, e)) {
            Iter it = std::find(p.begin(), p.end(), v);
            extract(p, it, p.end());
            return;
        }
    assert(false);
}

void BRUT::insertEdge(pair<int, int> e, int a, int b)
{
    m++;
    for (auto& p : P)
        if (seek(p, e)) {
            Iter at = p.end(), bt = p.end();

            for (Iter it = p.begin(); it != p.end(); it++) {
                if (*it == a or *it == b)
                    (at == p.end() ? at : bt) = it;
            }

            extract(p, at, bt);
            return;
        }
    assert(false);
}
