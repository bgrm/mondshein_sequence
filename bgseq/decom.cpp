#include "decom.h"
#include "vertex.h"
#include "bgedge.h"
#include "chain.h"
#include "cluster.h"
#include "inter.h"
#include <algorithm>

#include "debug.h"

vector <Vertex*> V, Vordered;
vector <Chain*> C;
vector <list <EdgeBG*> > H;

namespace
{
	void dfsTree(Vertex* v)
	{
		static int inCtr = 0;
		v->inorder = ++inCtr;
		Vordered.push_back(v);
		for (Vertex* u : v->adj) if (u->inorder == 0)
		{
		    u->father = v;
		    dfsTree(u);
		}
	}

	void initGraph(Graph& G, int n, int root)
	{
		V.resize(n+1);
		for (int i=1; i<=n; i++)
		    V[i] = new Vertex(i);
		for (int v=1; v<=n; v++)
		    for (int u : G[v])
		        V[v]->adj.push_back(V[u]); // making edges
		dfsTree(V[root]);  // setting up 'father' and 'inorder'
	}

	void initChains()
	{
		C.resize(1); // making room for C0
		for (Vertex* v : Vordered) for (Vertex* u : v->adj)
		{
		    if (u->inorder > v->inorder and u->father != v) // non-tree down-edges
		    {
		        C.push_back(new Chain(v, u));
		        if (SZ(C) == 3)
		            C[2]->father = C[0] = C[1]->extractC0(C[2]->t);
		    }
		}

		for (int i=1; i<SZ(C); i++) // filling ch12 and tp3
		{
		    if (C[i]->type[0] == '3')
		    {
		        Chain* ancestor = C[i]->s->chain;
		        ancestor->tp3.push_back(C[i]);
		    }
		    else
		    {
		        Chain* ancestor = C[i]->father;
		        ancestor->ch12.push_back(C[i]);
		    }
		    assert(!C[i]->marked());
		}
	}

	// i.e. top chain from each (bad) segment
	vector <Chain*> getSegmentsRepresentants(Chain* ch)
	{
		// 0 segment is simply the ch, because he'll contribute some intervals as well
		vector <Chain*> ret(1, ch);
		for (Chain* d : ch->ch12)
		    if (!d->covered())
		        assert(d->segment == d), ret.push_back(d->segment);
		return ret;
	}

	vector <Segment_t> getSegmentsChains(const vector <Chain*>& reprs)
	{
		vector <Segment_t> ret;
		for (Chain* d : reprs)
		{
		    d->segId = SZ(ret);
		    ret.push_back({d}); //  we start with adding representants
		}
		Chain* ch = reprs[0];
		for (Chain* d : ch->tp3) if (!d->covered())
		{
		    assert(!d->inGoodSeg(ch)); 
		    ret[d->segment->segId].push_back(d);
		}
		return ret;
	}

	/* The attachment vertices are the begins of all chains ('s') plus end of the first chain.
	We want to have their pathids ordered and with no duplicates. */
	vector <int> attachmentVerticesPathids(Segment_t& chains, const vector <int>& M)
	{
		vector <int> ret;
		bool zeroPresent = false;
		for (Chain* d : chains) // collecting pathids of 's' of chains (others than 0) in decreasing order
		{
		    int x = M[d->s->id];
		    if (x == 0)
		        zeroPresent = true;
		    else if (ret.empty() or ret.back() > x)
		        ret.push_back(x);
		}
		if (zeroPresent)
		    ret.push_back(0);
		rev(ret);

		/* handling chains[0].t: */
		int x = M[chains[0]->t->id];
		auto it = lower_bound(ret.begin(), ret.end(), x);
		if (it == ret.end() or *it != x) 
		    ret.insert(it, x);
		return ret;
	}

	//  adding intervals from artificial 'segment 0'
	void initIntervals(Chain* ch, vector <Interval*>& I, vector <int>& M)
	{
		ch->mapVertices(M);
		int len = M[ch->t->id] + 1;
		for (Vertex* v : ch->reals())
		    I.push_back(new Interval(M[v->id], len, 0));
	}

	void addIntervals(Segment_t& chains, vector <Interval*>& I, vector <int>& M, int tag)    //  adding intervals
	{
		vector <int> pathids = attachmentVerticesPathids(chains, M);
		int first = pathids[0], last = pathids.back();
		for (int x : pathids)
		{
		    if (x != first)
		        I.push_back(new Interval(first, x, tag));
		    if (x != last)
		        I.push_back(new Interval(x, last, tag));
		}
		I.pop_back(); // dupliacte of [first, last]
	}

	void dfs(Graph& G, int v, vector <Segment_t>& segs, vector <BGpath>& bgPaths)
	{
		for (Chain* d : segs[v])
		    d->cluster->addRec(bgPaths);
		segs[v].clear();
		for (int u : G[v])
		    if (!segs[u].empty())
		        dfs(G, u, segs, bgPaths);
	}

	vector <BGpath> bgPaths(int n)
	{
		vector <BGpath> ret;
		for (int i=0; i<3; i++)
		    C[i]->cluster->add(ret);
		vector <int> idToPathid(n+1); // mapping of vertices to id on path

		for (Chain* ch : C)
		{
		    for (Chain* d : ch->ch12)
		        if (!d->covered() and d->type == "2A" and d->t->real())
		            d->cluster->add(ret);
		        
		    for (Chain* d : ch->ch12)
		        if (!d->covered())
		            d->setSegment();

		    for (Chain* d : ch->tp3)
		        if (!d->covered())
		            d->setSegment();

		    for (Chain* d : ch->tp3)   //  adding chains from good segments
		        if (!d->covered() and d->inGoodSeg(ch))
		            d->cluster->addRec(ret);

		    // we store info about segments that need to be sorted (i.e. the 'bad' ones):
		    vector <Chain*> segRepr = getSegmentsRepresentants(ch);         // representant of the segment (top chain)
		    vector <Segment_t> segChains = getSegmentsChains(segRepr);       // chains of the segment

		    vector <Interval*> I; // we construct necessary intervals
		    initIntervals(ch, I, idToPathid);
		    int tag = 0;
		    for (Segment_t& chains : segChains)
		        if (chains[0] != ch) // we ommit artificial 0 segment
		            addIntervals(chains, I, idToPathid, ++tag);


		    /* we construct the graph on the set of the bad segments,
		    dfs-order of witch will determine the order of 'adding'
		    the clusters from these segments */
		    
		    int chLen = idToPathid[ch->t->id] + 1;
		    Graph GI = Interlock::makeEdges(I, chLen + 1, tag + 1);
		    dfs(GI, 0, segChains, ret); // dfs 'adds' clusters of all segments in order of visiting
		
		    for (Chain* d : ch->ch12)
		        assert(d->covered());
		}

		return ret;
	}
		
	vector <BGop> getBGops(vector <BGpath>& paths)
	{
		vector <BGop> ret;
		reverse(paths.begin(), paths.end());
		for (int _=0; _<3; _++)
		    paths.pop_back();   //  initial S3
		
		for (auto [r, t] : paths)
		{
		    int a = r->id, b = t->id;
		      
		    assert(!H[a].empty() and !H[b].empty() and H[a].back() == H[b].back());
		   	delete H[a].back();
		    
		    Edge e = {a, b};
		    Edge ea = {0, 0};
		    Edge eb = {0, 0};

		    // deleting vertices of deg 2 and updating their neigbor's H
		    for (int v : {a, b}) if (SZ(H[v]) == 2)
		    {
		        EdgeBG *g = H[v].front(), *h = H[v].back();
		        g->smooth(h, v);
		        (v == a ? ea : eb) = {g->v, g->u};
		    }
		    ret.push_back({e, ea, eb});
		}

		reverse(ret.begin(), ret.end());
		return ret; //  ret[0] contains initial K4
	}
} // namespace

namespace BGdecomposition
{
	vector <BGop> compute(Graph& G, int root)
	{
		int n = SZ(G) - 1;
		initGraph(G, n, root);
		H.resize(n+1);
		initChains();
		
		vector <BGpath> paths = bgPaths(n);
		vector <BGop> ret = getBGops(paths);

		return ret; // size = m-n-2 + 1 for k4
	}
	
	void clear()
	{
		for (Chain* c : C)
		{
		    if (c->type != "2B")
		        delete c->cluster;
		    delete c;
		}
		C.clear();
		int root = Vordered[0]->id;
		while (!H[root].empty())
		    delete H[root].back();

		for (Vertex* v : Vordered)
		    delete v;
		V.clear();
		Vordered.clear();
		H.clear();
	}
} // BGdecomposition

#if 0

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
        dbg("OK");

    bg = BGdecomposition::simplify(bg, n);
    if (BGdecomposition::validate(G, 1, bg, true))
        dbg("OK");
}

#endif
