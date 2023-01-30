#ifndef __DECOM__
#define __DECOM__

#include <algorithm>
#include <tuple>
#include <vector>
using std::vector, std::pair, std::tuple;

#define SZ(x) ((int)(x).size())
#define ARG(x, i) (std::get<i>(x))

struct Vertex;
struct Chain;
struct Cluster;
struct Interval;

using BGpath = pair<Vertex*, Vertex*>;
using Segment_t = vector<Chain*>;
using Edge = pair<int32_t, int32_t>;
using BGop = tuple<Edge, Edge, Edge>; // <newEdge, sourceOf1vertex, sourceOf22vertex>. source={0,0} for vertex that already exists
using Graph = vector<vector<int>>;

namespace BGdecomposition {
void preprocess(Graph& G, int root, int t, int u);

vector<BGop> compute(Graph& G, int root);

bool validate(const Graph& G, int root, const vector<BGop>& ops, bool simple);

vector<BGop> simplify(const vector<BGop>& ops, int n);

void clear();
}

#endif // __DECOM__
