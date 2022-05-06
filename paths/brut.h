#include <algorithm>
#include <list>
using std::pair, std::list;

using Iter = list <int> :: iterator;

namespace BRUT
{
	int newPath(pair <int, int> e);
	int find(pair <int, int> e);
	void insertVertex(pair <int, int> e, int v);
	void addVertex(pair <int, int> e, int v);
	void split(pair <int, int> e, int v);
	void insertEdge(pair <int, int> _, int a, int b);
}
