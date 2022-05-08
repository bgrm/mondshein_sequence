#include "mond.h"
#include <cassert>

void print(Graph& G)
{
	for (int v=1; v<(int)G.size(); v++)
	{
		printf("G[%d]:  ", v);
		for (int x : G[v])
			printf("%d ", x);
		printf(";\n");
	}
	printf("===================\n\n");
}

int main()
{
	int n, m;
	scanf ("%d%d", &n, &m);
	Graph G(n+1);
	while (m--)
	{
		int a, b;
		scanf ("%d%d", &a, &b);
		G[a].push_back(b);
		G[b].push_back(a);
	}

	int r, t, u;
	scanf ("%d%d%d", &r, &t, &u);
	MSeq M = Mondshein::compute(G, r, t, u);
	assert(Mondshein::validate(G, r, t, u, M));
	printf("CORRECT\n");
	return 0;
}
