#include <cassert>
#include "vertex.h"
#include "bgedge.h"
#include "cluster.h"
#include "chain.h"

int Chain::id()
{   return (long long) this % 10'000;   }

void Chain::print()
{
    printf("hhain %d:\n", id());
    printf("s->%d, z->%d, t->%d \t %s\n", s->id, z->id, t->id, covered() ? "VIS" : "");
    printf("%s\n", type.c_str());
    if (segment)
        printf("%d\n", segment->id());
    printf("======\n");
}

Chain::Chain(Vertex* v, Vertex* u)
: s(v), z(u), t(u), segment(NULL)
{
    while (!t->chain and t->father)
        t->chain = this, t = t->father;
    father = t->chain;
    setType();
    
    if (type == "2B")
        mark();
    else          // chain of type 2B will be in a caterpillar declared by a former 3B chain 
        cluster = new Cluster(this);

    if (type == "3B")
    {
        for (Chain* ch = father; ch->marked(); ch = ch->father)
        {
            ch->unmark();
            ch->cluster = cluster;
            cluster->chains.push_back(ch);
        }
    }
}

void Chain::setType()
{
    if (father == NULL)
        type = "0";
    else if (father->t->inorder <= s->inorder)
        type = "1";
    else if (s == father->s)
        type = t == z ? "2A" : "2B";
    else
        type = father->marked() ? "3B" : "3A";
}

void Chain::mark()
{    type.push_back('M');    }

bool Chain::marked()
{    return !type.empty() and type.back() == 'M';    }

void Chain::unmark()
{    type.pop_back();    }

bool Chain::covered()
{   return cluster->covered;    }

bool Chain::inGoodSeg(Chain* root) // if 'this' is in good seg when processing 'root'
{
    Chain* s = segment; assert(s);
    return s->type[0] == '3' or s->father != root;
}

Chain* Chain::setSegment()
{
    assert(!covered());
    assert(father);
    if (father->covered())
        segment = this;
    if (segment == NULL)
        segment = father->setSegment();

    return segment;
}

Chain* Chain::extractC0(Vertex* v) // breaking this==C1 into C0 and C1
{
    for (Vertex* u = v; u; u = u->father)
        u->chain = NULL;
    Chain* C0 = new Chain(v, v->father);
    
    v->chain = C0;
    t = v;
    father = C0;
    type = "1";
    return C0;
}

/* Updates H in path starting with chain's s->z.
Returns last updated vertex */
Vertex* Chain::add(Vertex* stop) 
{
    return Vertex::add(s, z, stop);
}

void Chain::mapVertices(vector <int>& M)
{
    M[s->id] = 0;
    int len = 1;
    for (Vertex* v = z; v != t->father; v = v->father)
        M[v->id] = len++;
}

vector <Vertex*> Chain::reals()
{
    vector <Vertex*> ret;
    if (s->real())
        ret.push_back(s);
    for (Vertex* v = z; v != t->father; v = v->father)
        if (v->real())
            ret.push_back(v);
    return ret;
}
