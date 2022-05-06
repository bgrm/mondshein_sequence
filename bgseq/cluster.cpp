#include "vertex.h"
#include "chain.h"
#include "cluster.h"
#include "debug.h"

Cluster::Cluster(Chain* chain)
{
    chains.push_back(chain);
    covered = false;
}

Cluster* Cluster::father()
{   return chains.back()->father->cluster;  }

void Cluster::addCaterpillar(vector <BGpath>& paths)
{
    Chain* first = chains[0];
    Chain* last = chains.back();
    Chain* parent = last->father;
    int start = -1;
    
    if (first->s->inorder < parent->t->inorder)
    {
        first->add();
        paths.push_back({first->s, last->t});
        start = 1;
    }

    else
    {
        Vertex* ft = first->t;

        first->add(ft);
        chains[1]->add(ft);
        paths.push_back({first->s, last->s});

        Vertex::add(ft, ft->father, last->t);
        paths.push_back({ft, last->t});

        start = 2;
    }

    for (int i=start; i<chains.size(); i++)
    {
        Chain* d = chains[i];
        paths.push_back({d->s, d->add()});
    }
}

/*
'Adds' the cluster.
Inserts proper bgpaths to 'paths'
*/
void Cluster::add(vector <BGpath>& paths)
{
    assert(!chains.empty());

    covered = true;
    Chain* ch = chains[0];

    if (chains.size() == 1) //  handling chains of type 0, 1, 2A, 3A
        paths.push_back({ch->s, ch->add()});

    else // handling caterpillars
        addCaterpillar(paths);
}

void Cluster::addRec(vector <BGpath>& paths)
{
    if (covered)
        return;
    father()->addRec(paths);
    add(paths);
}
