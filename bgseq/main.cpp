#include "decom.h"
#include <cassert>

void print(const vector <BGop>& bg)
{
    #define PRINT(e) printf("[%d-%d], ", e.first, e.second)

    for (auto [e, g, h] : bg)
    {
        PRINT(e), PRINT(g), PRINT(h);
        printf("\n");
    }

    printf("===================\n");
}

int main()
{
    int n, m;
    scanf ("%d%d", &n, &m);

    Graph G(n+1);

    while (m--)
    {
        int a, b;
        scanf("%d%d", &a, &b);
        G[a].push_back(b);
        G[b].push_back(a);
    }

    auto bg = BGdecomposition::compute(G, 1);

	auto [_, f, g] = bg[0];
	if (f.second > f.first)
		std::swap(f.first, f.second);
	if (g.second > g.first)
		std::swap(g.first, g.second);
	assert(f  == g);

    if (BGdecomposition::validate(G, 1, bg, false))
        printf("base validation \tOK\n");
    else
    {
        print(bg);
        return 1;
    }

    bg = BGdecomposition::simplify(bg, n);
    if (BGdecomposition::validate(G, 1, bg, true))
        printf("simplicity check \tOK\n");
    else
    {
        print(bg);
        return 1;
    }
}
