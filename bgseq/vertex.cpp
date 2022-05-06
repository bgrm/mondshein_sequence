#include "vertex.h"
#include "bgedge.h"
#include "debug.h"

Vertex::Vertex(int _id)
{
    id = _id;
    inorder = 0;
    father = NULL;
    chain = NULL;
}

extern vector <list <Edge_priv*> > H;
extern vector <Vertex*> Vordered;

int Vertex::deg()
{   return H[id].size();    }

bool Vertex::real()
{   return deg() >= 3;    }

Vertex* Vertex::add(Vertex* u, Vertex* v, Vertex* stop)
{
    do
    {
        makeBGedge(u->id, v->id);
        u = v;
        v = v->father;
    }
    while (u != stop and u->deg() < 2);
    return u;
}

void Vertex::print()
{
    printf("id=%d, inord=%d, fat=%d;\n", id, inorder, father ? father->id : -1);
}