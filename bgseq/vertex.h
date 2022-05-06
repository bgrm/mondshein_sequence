#ifndef __VERT__
#define __VERT__

#include <vector>
using std::vector;

struct Chain;
struct Edge_priv;

struct Vertex
{
    int id;         // vertex's number in the input
    int inorder;    // dfs entrance order
    Vertex* father; // null for the root
    vector <Vertex*> adj;
    Chain* chain;   // chain of the edge to the father

    Vertex(int id);
    int deg();
    bool real();
    static Vertex* add(Vertex* u, Vertex* v, Vertex* stop);

    void print();
};

#endif // __VERT__
