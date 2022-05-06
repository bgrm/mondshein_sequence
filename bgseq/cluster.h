#ifndef __CLUSTER__
#define __CLUSTER__

#include <algorithm>
#include <vector>
using std::vector, std::pair;

struct Vertex;
struct Chain;
using BGpath = pair <Vertex*, Vertex*>;

struct Cluster
{
    vector <Chain*> chains;
    bool covered;

    Cluster(Chain* ch);
    Cluster* father();
    void addCaterpillar(vector <BGpath>& paths);
    void add(vector <BGpath>& paths);
    void addRec(vector <BGpath>& paths);
};


#endif // __CLUSTER__
