#ifndef __CHAIN__
#define __CHAIN__

#include <string>
#include <vector>

struct Vertex;
struct Cluster;

extern std::vector<Vertex*> Vordered;

struct Chain {
    Vertex *s, *t, *z; // z - second vertex in chain
    std::string type;
    Cluster* cluster;
    Chain* father;
    std::vector<Chain*> ch12, tp3; // children12, type3
    Chain* segment; // segment representant
    int segId; // valid for a bad segment's root - info for type3-chains from it's segment

    Chain(Vertex* v, Vertex* u);
    int id();
    void setType();
    void mark();
    bool marked();
    void unmark();
    bool covered();
    bool inGoodSeg(Chain* seg);
    Chain* setSegment();
    Chain* extractC0(Vertex* v);
    Vertex* add(Vertex* stop = Vordered[0]);
    void mapVertices(std::vector<int>& M);
    std::vector<Vertex*> reals();

    void print();
};

#endif // __CHAIN__
