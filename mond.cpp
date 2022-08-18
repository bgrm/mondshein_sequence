#include "mond.h"
#include "bgseq/decom.h"
#include "paths/paths.h"
#include "order/order.h"
#include "order/sqrt/sqrt.h"
#include "shorts.h"

#include <cassert>
#include <set>
using std::swap, std::tie;
namespace PTH = PathsStructure;

namespace
{
	vector <BGopEx> bgops;
	Order* order;
	bool* removedId;
	std::array<int, 2>* I; // indices of edges incident to v in it's birth-ear
	int *nextT, *nextU;
	int r, t, u, n;

	inline int zeroEarId()
	{	return order->successor(0);	}

	pair <int, int> E(int v)
	{
		bool which = edgeEq(getEdge(I[v][0]), {r, t});
		return getEdge(I[v][which]);
	}

	// e cannot be short!
	int pathId(const Edge& e)
	{	return edgeEq(e, {r, t}) ? zeroEarId() : PTH::find(e);	}

	int pathId(int v)
	{	return PTH::find(E(v));	}
	
	void print()
	{
		printf("\n=====================\n");

		for (int v=1; v<=n; v++)
		{
			int p = I[v][0] == 0 ? -1 : pathId(v);
			printf("%d:: \t", v);
			assert(p != 0);		
			
			if (p != -1)
				printf("pid-%d\t\t[pred: %d]", p, order->predecessor(p));
			else
				printf("pending...");

			printf("\n");
		}
		printf("r t u: %d %d %d\n", r, t, u);
		printf("\nORDER:\n");
		for (int p = pathId(u); p != 0; p = order->predecessor(p))
		{
			printf("%d:\t", p);
			for (int v : PTH::getPath(p))
				printf("%d, ", v);
			printf("\n");
		}
		printf("\nSHORTS:\n");
		for (auto e : getAllShorts())
			printf("[%d-%d],  ", e.first, e.second);
		printf("\n=====================\n\n");
	}

	template <typename T1, typename T2>
	bool birthEq(T1 a, T2 b)
	{   return pathId(a) == pathId(b);  }

	//  no short ear allowed!
	template <typename T1, typename T2>
	bool birthLow(T1 a, T2 b)
	{   return order->compare(pathId(a), pathId(b));    }

	//	 younger i.e. greater
	int youngerId(int x, int y)
	{	return birthLow(x, y) ? pathId(y) : pathId(x);	}

	void initI(int v, int a, int b, const BGopEx& op)
	{	I[v] = {getInd({a, v}, op), getInd({v, b}, op)};	}

	void updateI(int v, int newInd, int only = 0)
	{
		for (int& i : I[v])
			if (only == i or (only == 0 and !birthEq(getEdge(i), getEdge(newInd))))
				i = newInd;
	}

	void enlong(const Edge& e, int end, int v, const BGopEx& op = empty_bgop)
	{
		PTH::addVertex(e.second == end ? e : flip(e), v);
		if (!EMPTY_BGOP(op))
			initI(end, e.first ^ e.second ^ end, v, op);
	}

	int createPath(const vector <int>& vs, const BGopEx& op)
	{
		assert(SZ(vs) > 1);
		int ret = PTH::newPath({vs[0], vs[1]});
		for (int i=2; i<SZ(vs); i++)
			enlong({vs[i-2], vs[i-1]}, vs[i-1], vs[i], op);
		return ret;
	}

	void insertIntoOrder(int old, int fresh, bool after = true)
	{
		if (after)
			order->insert(old, fresh);
		else
			order->insertBefore(old, fresh);
	}

	void subdivide(Edge e, int v, const BGopEx& op)
	{
		assert(!edgeEq(e, {r, u}));
		auto [a, b] = e;
		int i = getInd(e, op);
		int ia = getInd({a, v}, op);
		int ib = getInd({b, v}, op);

		assert(!shortEar(i));

		if (edgeEq(e, {r, t}))
		{
			int ir = a == r ? ia : ib;
			int it = a == t ? ia : ib;

			if (nextT[t] != v)  
			{
				// we use in advance the BGop that will reconnect r with t later (but it will happen before relabeling t)
				int p0 = createPath({t, v, r}, op);
				insertIntoOrder(0, p0);
				initI(r, t, v, op);
				initI(t, r, v, op);
			}
			else
			{
				enlong(E(t), t, v);
				updateI(r, ir, i);;
				updateI(t, it, i);
				initI(v, r, t, op);
				t = v;
			}
		}
		else
		{
			if (shortEar(i))
			{
				removeShort(i);
				int newPath = PTH::newPath(e);
				insertIntoOrder(youngerId(a, b), newPath);
			}
			PTH::insertVertex(e, v);
			initI(v, a, b, op);
			updateI(a, ia, i);
			updateI(b, ib, i);
		}
	}

	void leg(Edge e, const BGopEx& op)
	{
		auto [x, y] = e;
		int i = getInd(e, op);
		int former = pathId(y);
		int latter = PTH::split(E(y), y);

		Edge toEnlong = getEdge(I[y][0]), other = getEdge(I[y][1]);
		int otherInd = I[y][1];
		if (PTH::isSingleEdge(other) or PTH::isBorderVertex(toEnlong, x))
		{
			swap(toEnlong, other);
			otherInd = I[y][0];
		}

		removeShort(i);
		enlong(toEnlong, y, x);
		updateI(y, i);
		insertIntoOrder(former, latter, former == pathId(e));

		if (PTH::isSingleEdge(other))
		{
			removedId[PTH::find(other)] = true;
			PTH::eraseShort(other);
			insertShort(otherInd);
		}
	}

	void belly(Edge e, const BGopEx& op)
	{
		auto [x, y] = e;
		int i = getInd(e, op);
		removeShort(i);
	
		int former = pathId(y);
		int latter = PTH::insertEdge(E(y), x, y);
		insertIntoOrder(former, latter, former == pathId(e));
		updateI(x, i);
		updateI(y, i);
	}

	void intoLong(Edge e, const BGopEx& op)
	{	
		if (birthLow(e.second, e.first))
			e = flip(e);
		birthEq(e.first, e.second) ? belly(e, op) : leg(e, op);
	}

	void standarize(int& v, int& w, int& a, int& b, int& c, int& d, const BGopEx& op)
	{
		for (Edge& e : vector <Edge> {{c, d}, {a, b}})
			if (shortEar(e, op) and !edgeEq(e, {r, u}))
				intoLong(e, op);

		if (a == 0 or edgeEq({c, d}, {r, u}))
			swap(v, w), swap(a, c), swap(b, d);
	}

	void updateKeyVertex(int& x, vector <BGop>& bgopsBase, int* nxt)
	{
		for (int i=SZ(bgops)-1; i>0; i--)
		{
			auto [v, w] = ARG(bgopsBase[i], 0);
		    auto [a, b] = ARG(bgopsBase[i], 1);
		    auto [c, d] = ARG(bgopsBase[i], 2);

			int y = x;

		    if (v == x and a != 0) // v is being created
		    {
				assert(a == r or b == r);
				y = a ^ b ^ r;
			}
		    else if (w == x and c != 0)
		    {
				assert(c == r or d == r);
				y = c ^ d ^ r;
			}
			if (y != x)
				nxt[y] = x, x = y;
		}
	}
	
	int getFourth(BGop& p)
	{
		auto [e, f, g] = p;
		std::set <int> S {e.first, e.second, f.first, f.second, g.first, g.second};
		assert(SZ(S) == 4);
		for (int x : {r, t, u})
			S.erase(x);
		return *S.begin();			
	}

	void init(Graph& G)
	{
		n = SZ(G) - 1;
		BGdecomposition::preprocess(G, r, t, u);
		vector <BGop> bgopsBase = BGdecomposition::compute(G, r);
		assert(BGdecomposition::validate(G, r, bgopsBase, false));
		bgopsBase = BGdecomposition::simplify(bgopsBase, n);
		assert(BGdecomposition::validate(G, r, bgopsBase, true));
		bgops = getBGopsExteneded(bgopsBase, n);

		int m = SZ(bgops) + n + 1;
		PTH::setN(n);
		order = new Order(m);
		removedId = new bool[m * 2 + 1]();
		I = new std::array <int, 2> [n+1]();
		nextT = new int [n+1]();
		nextU = new int [n+1]();

		updateKeyVertex(t, bgopsBase, nextT);
		updateKeyVertex(u, bgopsBase, nextU);
		int x = getFourth(bgopsBase[0]);
		
		BGopEx& op = bgops.front();
		int p0 = createPath({t, x, r}, op);
		int p1 = createPath({t, u, x}, op);
		initI(r, t, x, op);
		initI(t, r, x, op);
		insertShort({r, u}, op);
		insertIntoOrder(0, p0);
		insertIntoOrder(p0, p1);
	}

	void caseRU(int v, int w, int a, int b, int c, int d, const BGopEx& op)
	{
		assert(edgeEq({a, b}, {r, u}));
		assert(!shortEar({c, d}, op));
		if (c != 0)
		    subdivide({c, d}, w, op);

		if (birthEq(w, u) and nextU[u] != v)
		{
			leg({v, w}, op);
			enlong({w, v}, v, r, op);
			insertShort({v, u}, op);
		}
		else if (nextU[u] != v)
		{
			int p = createPath({r, v, w}, op);
			insertIntoOrder(pathId(w), p);
			insertShort({v, u}, op);
		}
		else
		{
			removeShort({r, u}, op);
			int p = createPath({w, v, u}, op);
			insertIntoOrder(pathId(u), p);	
			u = v;
			insertShort({r, u}, op);
		}
	}

	void caseVV(int v, int w, const BGopEx& op)
	{
		if (!edgeEq({v, w}, {r, t}) and !edgeEq({v, w}, {r, u}))	// this BGop has already been used
			insertShort({v, w}, op);
	}

	void caseVE(int v, int w, int a, int b, const BGopEx& op)
	{
		subdivide({a, b}, v, op);
		Edge e {v, w};
		if (birthLow(v, w))
		    insertShort(e, op);
		else
			intoLong(e, op);
	}

	void caseEE(int v, int w, int a, int b, int c, int d, const BGopEx& op)
	{
		assert(!shortEar({a, b}, op));

		if (!shortEar({c, d}, op)) 
		{
		    subdivide({a, b}, v, op);
		    subdivide({c, d}, w, op);
		    intoLong({v, w}, op);
		}
		else	// {c,d} is a 'strict' leg of {a,b}'s long ear 
		{
			if (birthLow(d, c))
				swap(c, d);
		    removeShort({c, d}, op);
		    subdivide({a, b}, v, op);
		    leg({w, v}, op);
		    enlong({v, w}, w, c, op);
		    insertShort({w, d}, op);
		}
	}

	MSeq extractResult()
	{
		MSeq ret;
		for (Edge e : getAllShorts())	// short ears
			ret.push_back({e.first, e.second});
		for (int p=pathId(u); p!=0; p=order->predecessor(p))	// long ears
			if (!removedId[p])
				ret.push_back(PTH::getPath(p));
		reverse(ret.begin(), ret.end());
		return ret;
	}
	
	void clear()
	{		
		BGdecomposition::clear();
		PTH::clear();
		clearShorts();
		delete order;
		delete removedId;
		delete I;
		delete nextT;
		delete nextU;
	}
} //namespace

namespace Mondshein
{
	MSeq compute(Graph& G, int rr, int tt, int uu)
	{
		r = rr, t = tt, u = uu;
		init(G);

		for (int i=1; i<SZ(bgops); i++)
		{
			auto [v, w] = getEdge(bgops[i][0]);
		    auto [a, b] = getEdge(bgops[i][1]);
		    auto [c, d] = getEdge(bgops[i][2]);

			BGopEx& op = bgops[i];
		    standarize(v, w, a, b, c, d, op);

		    if (edgeEq({a, b}, {r, u}))
		        caseRU(v, w, a, b, c, d, op);
		    else if (a == 0 and c == 0)
		        caseVV(v, w, op);
		    else if (c == 0)
		        caseVE(v, w, a, b, op);
		    else
		        caseEE(v, w, a, b, c, d, op);
		}

		MSeq ret = extractResult();
		clear();
		return ret;
	}

	bool validate(const Graph& G, int r, int t, int u, const MSeq& M)
	{
		#define FAIL(x) return printf("ERROR: %s\n", x), false

		int n = SZ(G) - 1, m = 0;
		vector <int> B(n+1, -1);
		vector <int> it(n+1, -1);

		for (int p=0; p<SZ(M); p++)
		{
		    if (SZ(M[p]) < 2)
		        FAIL("ear with less than 2 vertices");
		        
		    bool notCycle = p != 0;

		    for (int i = notCycle; i < SZ(M[p]) - notCycle; i++)
		    {
		        int v = M[p][i];
				if (v < 1 or v > n)
					FAIL("invalid vertex");
		        if (B[v] != -1)
		            FAIL("multioccurence of a vertex");
		        B[v] = p;
				it[v] = i;
		    }
		}

		for (int v=1; v<=n; v++)
		{
		    if (B[v] == -1)  
		        FAIL("not all vertices covered");
		    if (v != u and B[v] >= B[u])
		        FAIL("u is not the youngest vertex");
		    m += SZ(G[v]);

			bool hasYounger = v == u;

		    for (int x : G[v])
				if (B[x] > B[v])
					hasYounger = true;

			if (!hasYounger)
		        FAIL("a vertex without younger neighbor");
		}
		m /= 2;

		if (SZ(M) != m-n+1)
		    FAIL("wrong number of ears");

		Edge rt {M[0].front(), M[0].back()};
		if (!edgeEq(Edge(r, t), rt))
		    FAIL("rt does not create the 0 ear");

		const vector <int>& pu = M[B[u]];
		if (SZ(pu) != 3)
		    FAIL("invalid u's ear's size");
		if (pu[0] == r or pu[2] == r)
		    FAIL("ru is not short");

		for (int v=1; v<=n; v++)
		{
			auto& ear = M[B[v]];
			vector <int> S;
			if (it[v] != 0)
				S.push_back(ear[it[v] - 1]);
			if (it[v] != SZ(ear)-1)
				S.push_back(ear[it[v] + 1]);
			
			for (int x : S)
				if (std::find(G[v].begin(), G[v].end(), x) == G[v].end())
					FAIL("some long ear is not connected");
		}

		vector <Edge> E, shorts;
		for (int v=1; v<=n; v++)
			for (int u : G[v])
				if (v < u)
					E.push_back({v, u});
		for (auto& p : M) if (SZ(p) == 2)
		{
			int a = p.front(), b = p.back();
			shorts.push_back({std::min(a, b), std::max(a, b)});
		}
		sweep<Edge>(E, n, 1), sweep<Edge>(E, n, 0);
		sweep<Edge>(shorts, n, 1), sweep<Edge>(shorts, n, 0);
		E.push_back({n, n});
		auto s = shorts.begin();

		for (Edge& e : E)
			if (s != shorts.end() and *s == e)
				s++;
		if (s != shorts.end())
			FAIL("invalid short ear");
		return true;
	}
} // Mondshein
