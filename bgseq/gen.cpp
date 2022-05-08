#include <vector>
#include <algorithm>
#include <functional>
using std::vector, std::pair, std::max, std::min, std::fill, std::make_pair, std::less;

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template<typename T>
using ordered_set = tree <T, null_type, less<T>, rb_tree_tag,
	tree_order_statistics_node_update>;		//find_by_order() / order_of_key()

using Edge = pair <int, int>;

int los(int a, int b) // [a, b]
{   return a + rand() % (b-a+1);    }

const int maxN = 1 << 20;

bool vis[maxN];
vector <int> G[maxN];

void dfs(int v)
{
    vis[v] = true;
    for (int u : G[v])
        if (!vis[u])
            dfs(u);
}

void gen(int s, int seed) // s - # bgops
{
    srand(seed);
    #define KI(e) make_pair(min(e.first, e.second), max(e.first, e.second))
    #define IT(e) E.find(KI(e))
    #define EQL(e, f) (KI(e) == KI(f))
    #define EXS(e) (IT(e) != E.end())
    #define INS(a, b) (E.insert(KI(make_pair(a, b))))
    #define DEL(e) (E.erase(KI(e)))
    #define SZ(x) ((int)(x).size())

    ordered_set <Edge> E;
    int n = 4;
    for (int i=2; i<=4; i++) for (int j=1; j<i; j++)
        INS(i, j);
    
    while (s--)
    {
        int ev = los(0, 2), m = SZ(E);
        while (ev == 0 and m > 1ll * n * (n-1) / 2 - 5)
            ev = los(0, 2);

        if (ev == 0)
        {
            int a = 0, b = 0;
            while (a == b or EXS(make_pair(a, b)))
                a = los(1, n), b = los(1, n);
            INS(a, b);
        }
        if (ev == 1)
        {
            auto it = E.begin();
            int v = it->first;

            while (v == it->first or v == it->second)
                it = E.find_by_order(los(0, m-1)), v = los(1, n);
            auto [a, b] = *it;
            E.erase(it);
            n += 1;
            INS(a, n);
            INS(b, n);
            INS(v, n);
        }
        if (ev == 2)
        {
            int i = 0, j = 0;
            while (i == j)
                i = los(0, m-1), j = los(0, m-1);
            
            auto it = E.find_by_order(i), jt = E.find_by_order(j);
            auto [a, b] = *it;
            auto [c, d] = *jt;
            E.erase(it);
            E.erase(jt);
            n += 1;
            INS(a, n), INS(b, n);
            n += 1;
            INS(c, n), INS(d, n);
            INS(n-1, n);
        }
    }

    printf("%d %d\n", n, SZ(E));
    for (auto [a, b] : E)
        printf("%d %d\n", a, b), G[a].push_back(b), G[b].push_back(a);
    
    //return;

    // check for 3-connectivity of the generated graph

    for (int a=1; a<=n; a++) for (int b=1; b<=n; b++)
    {
        fill(vis, vis+n+1, false);
        vis[a] = vis[b] = true;
        int r = 1;
        while (vis[r])
            r++;
        dfs(r);
        for (int v=1; v<=n; v++)
            assert(vis[v]);
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("usage: %s #bgops, optionally seed, optionally any character for printing rtu\n", argv[0]);
        return 1;
    }
    int s = atoi(argv[1]);
    int seed = argc > 2 ? atoi(argv[2]) : 42;
    gen(s, seed);

    if (argv[3]) // print r t u
    {
        printf("%d %d %d\n", 1, G[1][0], G[1][1]);
    }
}