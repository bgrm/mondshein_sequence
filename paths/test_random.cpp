#include "brut.h"
#include "paths.h"
#include <random>
#include <cassert>
#include <string>

using std::rand, std::make_pair, std::swap, std::string;
namespace PTH = PathsStructure;

int n; // vertices -> 1..n
extern int m; // number of edges
extern vector <list <int> > P; // paths maintaining by brut

pair <int, int> randEdge()
{
	int x = rand() % m;
	for (auto& p : P)
	{
		int s = p.size();
		if (x < s-1)
		{
			auto ret = p.begin();
			advance(ret, x);
			return {*ret, *next(ret)};
		}
		
		x -= s-1;
	}
	assert(false);
}

pair <int, int> randEdgeBorder()
{
	auto& path = P[rand() % P.size()];
	int fromRight = rand() % 2;

	auto first = path.begin();
	auto last = prev(path.end());
	return fromRight ? make_pair(*prev(last), *last) : make_pair(*next(first), *first);
}

pair <Iter, Iter> randPair()
{
	vector <int> inds;
	for (int i=0; i<P.size(); i++)
		if (P[i].size() > 4)
			inds.push_back(i);
	
	if (inds.empty())
		throw "no long path!";
		
	int x = inds[rand() % inds.size()];
	int s = P[x].size();
	
	int i = 0, j = 0;
	while (i == 0 or j == s-1 or i >= j-1)
	{
		i = rand() % s, j = rand() % s;
		if (i > j)
			swap(i, j);
	}
	
	auto it = P[x].begin(), jt = P[x].begin();
	advance(it, i);
	advance(jt, j);
	return {it, jt};
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

vector <int> getRandQuery()
{
	const int q = rand() % 6;
	
	if (q == 0) // newPath
	{
		return {q, ++n, ++n};
	}
	
	if (q == 1) // find
	{
		auto [a, b] = randEdge();
		auto [c, d] = randEdge();
		return {q, a, b, c, d};
	}
	
	if (q == 2) // insertVertex
	{
		auto [a, b] = randEdge();
		return {q, a, b, ++n};
	}	
	
	if (q == 3) // addVertex
	{
		auto [a, b] = randEdgeBorder();

		if (isInner(b))
			return getRandQuery();
		return {q, a, b, ++n};
	}
	
	if (q == 4) // split
	{
		auto [a, b] = randEdge();
		
		if (!isOuter(a))
			return {q, a, b, a};
		else if (!isOuter(b))
			return {q, a, b, b};
		else
			return getRandQuery();
	}
	
	if (q == 5) // insertEdge
	{
		Iter a, b;
		try
		{
			tie(a, b) = randPair();
		}
		catch (char const* _)
		{
			return getRandQuery();
		}
		return {q, *a, *next(a), *a, *b};
	}
	assert(false);
}

void printOut()
{
	for (auto& p : P)
	{
		for (int v : p)
			printf("%d ", v);
		printf(" :;\n");
	}
	printf("\n==========================\n\n");
}

void fail(const vector <int>& Q, int seed)
{
	printf("\nWA!\nQ: %d-%d, %d-%d\nseed: %d\n\n", Q[1], Q[2], Q[3], Q[4], seed);

	printOut();
	PTH::clear();
	exit(1);
}

int rnd(int a, int b)
{	return rand() % (b - a) + a;	}

void finalCheck(int seed);


const bool runPth = true;
const bool runBrt = true;
const bool dbg = true;

const pair <int, int> qRng = {100, 200};

const vector <string> name {"newPath", "find", "insertVertex", "addVertex", "split", "insertEdge"};

int main(int argc, char* argv[])
{
	int seed = argv[1] ? atoi(argv[1]) : 42;
	std::srand(seed);	

	int q = rnd(qRng.first, qRng.second);
	
	PTH::setN(q+1);
	PTH::newPath({1, 2});
	BRUT::newPath({1, 2});
	n = 2;
	q--;

	while (q--)
	{
		auto Q = getRandQuery();
		
		if (dbg)
		{
			for (int i : Q)
				printf("%d ", i);
			printf("\t%s\n", name[Q[0]].c_str());
		}

		switch(Q[0])
		{
			case 0:
				if (runPth) PTH::newPath({Q[1], Q[2]});
				if (runBrt) BRUT::newPath({Q[1], Q[2]});
				break;
			case 1:
			{
				pair <int, int> e = {Q[1], Q[2]};
				pair <int, int> f = {Q[3], Q[4]};
				bool same1 = runPth ? PTH::find(e) == PTH::find(f) : true;
				bool same2 = runBrt ? BRUT::find(e) == BRUT::find(f) : true;
				
				if (same1 != same2 and runPth and runBrt)
					fail(Q, seed);
				break;
			}	
			case 2:
				if (runPth) PTH::insertVertex({Q[1], Q[2]}, Q[3]);
				if (runBrt) BRUT::insertVertex({Q[1], Q[2]}, Q[3]);
				break;
			case 3:
				if (runPth) PTH::addVertex({Q[1], Q[2]}, Q[3]);
				if (runBrt) BRUT::addVertex({Q[1], Q[2]}, Q[3]);
				break;
			case 4:
				if (runPth) PTH::split({Q[1], Q[2]}, Q[3]);
				if (runBrt) BRUT::split({Q[1], Q[2]}, Q[3]);
				break;
			case 5:
				if (runPth) PTH::insertEdge({Q[1], Q[2]}, Q[3], Q[4]);
				if (runBrt) BRUT::insertEdge({Q[1], Q[2]}, Q[3], Q[4]);
				break;
		}
		if (dbg)
			printOut();
	}
	finalCheck(seed);
	PTH::clear();
	printf("AC...\n");
	return 0;
}

void finalCheck(int seed)
{
	for (auto& p : P)
	{
		int x = *(p.begin()), y = *next(p.begin());
		for (auto it = p.begin(); it != prev(p.end()); it++)
		{
			int a = *it, b = *next(it);
			if (runBrt)
				assert(BRUT::find({a, b}) == BRUT::find({x, y}));
			
			if (runPth and PTH::find({a, b}) != PTH::find({x, y}))
				fail({x, y, a, b}, seed);				
		}
	}
}