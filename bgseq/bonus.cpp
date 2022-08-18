#include "decom.h"
#include <set>
#include <list>
#include <queue>
#include <cassert>
#include <cstdio>
using std::set, std::list, std::multiset, std::queue;

namespace
{
	set <int> getK4(const BGop& op0)
	{
		set <int> K;
		auto [e, g, _] = op0;
		for (int v : {e.first, e.second, g.first, g.second})
		    K.insert(v);
		return K;
	}

	using Tup_t = tuple <int, int, int, list<BGop>::iterator>;
	// created/broken, u, v, iterator  [u < v]

	int arg(const Tup_t& t, int i)
	{
		if (i == 0)
		    return ARG(t, 0);
		if (i == 1)
		    return ARG(t, 1);
		if (i == 2)
		    return ARG(t, 2);
		assert(false);
	}

	// posSort of T by x coordinate
	void posSort(vector <Tup_t>& T, int n, int x)
	{
		static vector <vector <Tup_t> > bucket;
		bucket.resize(n+1);

		for (Tup_t& t : T)
		    bucket[arg(t, x)].push_back(t);
		T.clear();
		for (int i=1; i<=n; i++)
		{
		    T.insert(T.end(), bucket[i].begin(), bucket[i].end());
		    bucket[i].clear();
		}
	}

	Edge edge(const Tup_t& t)
	{   return {ARG(t, 1), ARG(t, 2)};  }

	void pushTuple(vector <Tup_t>& tups, int event, Edge e, list<BGop>::iterator it)
	{
		auto [u, v] = e;
		if (u > v)
		    std::swap(u, v);
		tups.push_back({event, u, v, it});
	}

	vector <Tup_t> getTuples(const vector <BGop>& ops, list <BGop>& L, int n)
	{
		vector <Tup_t> tups;
		
		set <int> K4 = getK4(ops[0]);
		for (int v : K4) for (int u : K4) if (v < u) // we init 'tups' with edges from K4
		    pushTuple(tups, 1, {v, u}, L.end());

		for (int i=1; i<SZ(ops); i++) // first op is just representation of K4
		{
		    L.push_back(ops[i]);
		    auto it = prev(L.end());
		    auto [e, g, h] = ops[i];
		    pushTuple(tups, 1, e, it);

		    for (Edge f : {g, h}) if (f.first != 0)
		    {
		        int v = f == g ? e.first : e.second;
		        pushTuple(tups, 1, {v, f.first}, it);
		        pushTuple(tups, 1, {v, f.second}, it);
		        pushTuple(tups, 2, f, it);
		    }
		}
		pushTuple(tups, n, {n, n}, L.end()); // buffor
		return tups;
	}
}

namespace BGdecomposition
{
	/*
	* We make sure that t and u are covered by inital K4 with r being the root.
	* I.e. t and u will be in some link of which K4 consists.
	* The reason is that we want t and u to 'occure'
	* in a graph as a result of breaking some edge incident with r.
	*/ 
	void preprocess(Graph& G, int r, int t, int u)
	{
		vector <int>& adj = G[r];
		const auto t_it = std::find(adj.begin(), adj.end(), t);
		std::swap(adj[1], *t_it);
		const auto u_it = std::find(adj.begin(), adj.end(), u);
		std::swap(adj[2], *u_it);
	}

	bool validate(const Graph& G, int root, const vector <BGop>& ops, bool simple)
	{
		#define FAIL(x) { printf("WA: %s\n", x); return false; }
		#define KI(e) std::make_pair(std::min(e.first, e.second), std::max(e.first, e.second))
		#define IT(e) E.find(KI(e))
		#define EQL(e, f) (KI(e) == KI(f))
		#define EXS(e) (IT(e) != E.end())
		#define INS(a, b) (E.insert(KI(std::make_pair(a, b))))
		#define DEL(e) (E.erase(IT(e)))

		set <int> K4 = getK4(ops[0]);
		multiset <Edge> E;
		E.insert({0, 0});
		
		if (SZ(K4) != 4)
		    FAIL("invalid initial K4");

		if (K4.find(root) == K4.end())
		    FAIL("root not in initial K4");
		
		for (int v : K4) for (int u : K4)
		    if (v < u)
		        INS(v, u);
		
		for (int i=1; i<SZ(ops); i++)
		{
		    auto [e, g, h] = ops[i];
		    if (e.first == e.second)
		        FAIL("loop");
		    if (e.first == h.first or e.first == h.second or e.second == g.first or e.second == g.second)
		        FAIL("broken edge incidents with connected vertex");
		    if (g.first != 0 and EQL(g, h))
		        FAIL("operation on paralel/equal edges");
		    if (!EXS(g) or !EXS(h))
		        FAIL("operation on nonexisting edge");
		    if (g.first != 0)
		        DEL(g), INS(e.first, g.first), INS(e.first, g.second);
		    if (h.first != 0)
		        DEL(h), INS(e.second, h.first), INS(e.second, h.second);

		    if (simple and EXS(e))
		        FAIL("multigraph");
		    INS(e.first, e.second);
		}

		for (int v=1; v<SZ(G); v++) for (int u : G[v]) if (u > v)
		{
		    Edge e = {v, u};
		    if (!EXS(e))
		        FAIL("wrong graph obtained");
		    DEL(e);
		}

		E.erase({0, 0});
		if (!E.empty())
		    FAIL("edges left");
		return true;
	}

	// returns ops modified so that temporary graphs are always simple
	vector <BGop> simplify(const vector <BGop>& ops, int n)
	{
		list <BGop> L;
		#define HOP(it, jt) (L.insert(next(jt), *it), L.erase(it))
		// moving it after jt

		vector <Tup_t> tups = getTuples(ops, L, n);
		for (int x=2; x>=0; x--)
		    posSort(tups, n, x);
		
		int j = 0;
		while (ARG(tups[j], 0) == 1)
		    j++;

		/* we put every bgop creating e - except the first one -
		* after an operation in which e is broken.
		* All relocated bgops are vv-ops, so they don't break any edge */
		for (int i = 0; ARG(tups[i], 0) == 1; i++)
		{
		    while (edge(tups[j]) < edge(tups[i]))
		        j++;
		    auto it = ARG(tups[i], 3), jt = ARG(tups[j], 3);

		    // if tups[i] is not the first occ of the edge
		    if (i != 0 and edge(tups[i-1]) == edge(tups[i]))
		        HOP(it, jt), j++;
		}

		L.insert(L.begin(), ops[0]); // K4
		return vector <BGop> (L.begin(), L.end());
	}
} //BGdecomposition

