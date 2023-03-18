#include "brut.h"
#include "ops.h"
#include <cassert>
#include <random>
#include <string>
#include <vector>
#include <map>

using std::rand, std::make_pair, std::swap, std::string, std::vector, std::map;
namespace PTH = PathsStructure;

const bool runPath = true;
const bool runBrut = true;
const bool dbg = false;
const pair<int, int> qRng = { 100, 200 };

int n; // vertices -> 1..n
extern int m; // number of edges
extern vector<list<int>> P; // paths maintaining by brut

pair<int, int> randEdge()
{
    int x = rand() % m;
    for (auto& p : P) {
        int s = p.size();
        if (x < s - 1) {
            auto ret = p.begin();
            advance(ret, x);
            return { *ret, *next(ret) };
        }

        x -= s - 1;
    }
    assert(false);
}

pair<int, int> randEdgeBorder()
{
    auto& path = P[rand() % P.size()];
    int fromRight = rand() % 2;

    auto first = path.begin();
    auto last = prev(path.end());
    return fromRight ? make_pair(*prev(last), *last) : make_pair(*next(first), *first);
}

pair<Iter, Iter> randPair()
{
    vector<int> inds;
    for (int i = 0; i < P.size(); i++)
        if (P[i].size() > 4)
            inds.push_back(i);

    if (inds.empty())
        throw "no long path!";

    int x = inds[rand() % inds.size()];
    int s = P[x].size();

    int i = 0, j = 0;
    while (i == 0 or j == s - 1 or i >= j - 1) {
        i = rand() % s, j = rand() % s;
        if (i > j)
            swap(i, j);
    }

    auto it = P[x].begin(), jt = P[x].begin();
    advance(it, i);
    advance(jt, j);
    return { it, jt };
}

bool isInner(int v)
{
    for (auto& p : P)
        for (Iter it = next(p.begin()); *it != p.back(); it++)
            if (*it == v)
                return true;
    return false;
}

bool isOuter(int v)
{
    for (auto& p : P)
        if (v == *p.begin() or v == p.back())
            return true;
    return false;
}

vector<int> getRandQuery()
{
    const int q = rand() % 6;

    if (q == 0) // newPath
    {
        return { q, ++n, ++n };
    }

    if (q == 1) // find
    {
        auto [a, b] = randEdge();
        auto [c, d] = randEdge();
        return { q, a, b, c, d };
    }

    if (q == 2) // insertVertex
    {
        auto [a, b] = randEdge();
        return { q, a, b, ++n };
    }

    if (q == 3) // addVertex
    {
        auto [a, b] = randEdgeBorder();

        if (isInner(b))
            return getRandQuery();
        return { q, a, b, ++n };
    }

    if (q == 4) // split
    {
        auto [a, b] = randEdge();

        if (!isOuter(a))
            return { q, a, b, a };
        else if (!isOuter(b))
            return { q, a, b, b };
        else
            return getRandQuery();
    }

    if (q == 5) // insertEdge
    {
        Iter a, b;
        try {
            tie(a, b) = randPair();
        } catch (char const* _) {
            return getRandQuery();
        }
        return { q, *a, *next(a), *a, *b, *next(b) };
    }
    assert(false);
}

void printOut()
{
    for (auto& p : P) {
        for (int v : p)
            printf("%d ", v);
        printf(" :;\n");
    }
    printf("\n==========================\n\n");
}

void fail(const vector<int>& Q, int seed)
{
    printf("\nWA!\nQ: %d-%d, %d-%d\nseed: %d\n\n", Q[1], Q[2], Q[3], Q[4], seed);

    printOut();
    exit(1);
}

int rnd(int a, int b)
{
    return rand() % (b - a) + a;
}

vector <pair <int, int> > EE;
std::map <pair <int, int>, int> M;

pair <int, int> getEdge(int i)
{   return EE[i];   }

int markEdge(int a, int b)
{
    auto it = M.find({a, b});
    if (it == M.end())
    {
        M[{a, b}] = M[{b, a}] = EE.size();
        EE.push_back({a, b});
    }
    return M[{a, b}];
}

int markEdge(pair <int, int> p)
{   return markEdge(p.first, p.second); }

void finalCheck(int seed);

const vector<string> name { "newPath", "find", "insertVertex", "addVertex", "split", "insertEdge" };

int main(int argc, char* argv[])
{
    if (argc < 3) {
        printf("No arguments provided. (seed and number of operations)\n");
        return 1;
    }
    int seed = atoi(argv[1]);
    int q = atoi(argv[2]);
    std::srand(seed);

    assert(markEdge(-1, -1) == 0);

    PTH::init(q * 2 + 1);
    PTH::newPath(markEdge(1, 2));
    BRUT::newPath({ 1, 2 });
    n = 2;
    q--;

    while (q--) {
        auto Q = getRandQuery();

        if (dbg) {
            for (int i : Q)
                printf("%d ", i);
            printf("\t%s\n", name[Q[0]].c_str());
        }

        switch (Q[0]) {
        case 0:
            if (runPath)
                PTH::newPath(markEdge( Q[1], Q[2] ));
            if (runBrut)
                BRUT::newPath({ Q[1], Q[2] });
            break;
        case 1: {
            pair<int, int> e = { Q[1], Q[2] };
            pair<int, int> f = { Q[3], Q[4] };
            bool same1 = runPath ? PTH::find(markEdge(e)) == PTH::find(markEdge(f)) : true;
            bool same2 = runBrut ? BRUT::find(e) == BRUT::find(f) : true;

            if (same1 != same2 and runPath and runBrut)
                fail(Q, seed);
            break;
        }
        case 2:
            if (runPath)
                PTH::insertVertex(markEdge(Q[1], Q[2]), markEdge(Q[1], Q[3]), markEdge(Q[2], Q[3]));
            if (runBrut)
                BRUT::insertVertex({ Q[1], Q[2] }, Q[3]);
            break;
        case 3:
            if (runPath)
                PTH::addVertex(markEdge(Q[1], Q[2]), markEdge(Q[2], Q[3]));
            if (runBrut)
                BRUT::addVertex({ Q[1], Q[2] }, Q[3]);
            break;
        case 4:
            if (runPath)
                PTH::split(markEdge(Q[1], Q[2]), Q[3]);
            if (runBrut)
                BRUT::split({ Q[1], Q[2] }, Q[3]);
            break;
        case 5:
            if (runPath)
                PTH::insertEdge(markEdge(Q[3], Q[4]), markEdge(Q[1], Q[2]), markEdge(Q[5], Q[4]));
            if (runBrut)
                BRUT::insertEdge({ Q[1], Q[2] }, Q[3], Q[4]);
            break;
        }
        if (dbg)
            printOut();
    }

    printf("FINAL CHECK\n");
    finalCheck(seed);
    printf("AC\n");
    return 0;
}

void finalCheck(int seed)
{
    for (auto& p : P) {
        int x = *(p.begin()), y = *next(p.begin());
        for (auto it = p.begin(); it != prev(p.end()); it++) {
            int a = *it, b = *next(it);
            if (runBrut)
                assert(BRUT::find({ a, b }) == BRUT::find({ x, y }));

            if (runPath and PTH::find(markEdge(a, b)) != PTH::find(markEdge(x, y)))
                fail({ x, y, a, b }, seed);
        }
    }
}