#ifndef __MOND__
#define __MOND__

#include <vector>
#include <algorithm>

using std::vector, std::pair;

using Graph = vector <vector <int> >;
using Edge = pair <int32_t, int32_t>;
using MSeq = vector <vector <int> >;

namespace Mondshein
{
    MSeq compute(Graph& G, int rr, int tt, int uu);

    bool validate(Graph& G, int r, int t, int u, MSeq& M);
};

#endif // __MOND__
