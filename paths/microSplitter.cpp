#include "microsetSplitter.h"
#include "element.h"
#include "micro.h"
#include "mezzo.h"
#include "path.h"
#include <cassert>


extern vector <Element> E;
extern Path::SelfIter_t nullPath;
extern Mezzoset::SelfIter_t unlabeledMicrosets;

int MicrosetSplitter::targetSize;

MicrosetSplitter::MicrosetSplitter(Microset::SelfIter_t micro)
: oldMicro(micro)
, newMicro(micro->split(micro->end())) // this call does not move any edges, it just creates a new microset in this' mezzoset
, oldLabel(micro->microLabel())
, oldLabOnLeft(false)
, oldLabOnRight(false)
{}


vector <vector <int> > MicrosetSplitter::extractPaths() const
{
    vector <vector <int> > paths;
    auto micro = oldMicro;
    auto& edges = micro->edges;

    for (int i = 0; i < edges.size(); i++)
    {
        if (i != micro->getLeft(i))
            continue;

        vector <int> path {edges[i]};

        for (int j = i; j != micro->getRight(j); )
        {
            j = micro->getRight(j);
            path.push_back(edges[j]);
        }
        paths.push_back(path);
    }
    return paths;
}

void MicrosetSplitter::addSubpath(Microset::SelfIter_t micro, const vector <int>& p, int start, int len)
{
    if (len == 0)
        return;
    int e0 = p[start];
    E[e0].placeInMicro(micro);
    
    for (int i = start + 1; i < start + len; i++)
    {
        int currEdgeId = p[i];
        micro->insertAfter(micro->size() - 1, E[currEdgeId]);
    }
}

void MicrosetSplitter::addSubpath(Microset::SelfIter_t micro, const vector <int>& p)
{   addSubpath(micro, p, 0, p.size()); }

void MicrosetSplitter::updateMezzo(Microset::SelfIter_t micro, Iter_t <Path> newLabel, Iter_t <Path> oldLabel)
{
    if (newLabel == oldLabel)
        return;
    if (newLabel == nullPath)
        unlabeledMicrosets->anectMicroset(micro);
    else
        newLabel->anectMicroset(micro);
}

void MicrosetSplitter::setupNewMicrosetsPlacement()
{
    Iter_t <Path> leftLabel = nullPath;
    Iter_t <Path> rightLabel = nullPath;

    if (oldLabOnLeft)
        leftLabel = oldLabel;
    else if (!oldMicro->lastsSubpathMarginal())
        leftLabel = longestsLabel;

    if (oldLabOnRight)
        rightLabel = oldLabel;
    else if (!newMicro->lastsSubpathMarginal())
        rightLabel = longestsLabel;

    updateMezzo(oldMicro, leftLabel, oldLabel);
    updateMezzo(newMicro, rightLabel, oldLabel);
}

void MicrosetSplitter::distributePaths(vector <vector <int> >& paths, int longestPathIndex)
{
    auto currMicro = oldMicro;

    for (int i=0; i<paths.size(); i++) if (i != longestPathIndex)
    {
        auto& path = paths[i];
        if (currMicro->size() + path.size() > targetSize)
            currMicro = newMicro;
        addSubpath(currMicro, path);

        // checking whether is it subpath, by which the microset was labeled
        bool pathLabeled = !currMicro->lastsSubpathMarginal();
        if (pathLabeled)
            (currMicro == newMicro ? oldLabOnRight : oldLabOnLeft) = true;            
    }
}

void MicrosetSplitter::manageLongestPath(vector <int>& longest)
{
    // Natural solution is to assign prefix of the longest path into this, and suffix into new microset.
    // But there are some cases, when we want to do the opposite way:
    prefixIntoNew = 
        (!E[longest.back()].isMarginal() and oldLabOnRight)  // 2 open subpaths would end up in new microset
        or
        (!E[longest.front()].isMarginal() and oldLabOnLeft); // 2 open subpaths would end up in this microset

    // in this case path is not being split
    if (oldMicro->size() == targetSize)
        prefixIntoNew = false;

    int intoOldLen = targetSize - oldMicro->size();
    int intoNewLen = targetSize - newMicro->size();

    int intoOldStart = prefixIntoNew ? longest.size() - intoOldLen : 0;
    int intoNewStart = prefixIntoNew ? 0 : longest.size() - intoNewLen;

    addSubpath(oldMicro, longest, intoOldStart, intoOldLen);
    addSubpath(newMicro, longest, intoNewStart, intoNewLen);
}

void MicrosetSplitter::split()
{
    auto paths = extractPaths();
    int longestPathIndex = 0;
    for (int i=0; i<paths.size(); i++)
        if (paths[longestPathIndex].size() < paths[i].size())
            longestPathIndex = i;

    auto& longest = paths[longestPathIndex];
    longestsLabel = E[longest.front()].find();

    oldMicro->clear();
    distributePaths(paths, longestPathIndex);
    manageLongestPath(longest);
    setupNewMicrosetsPlacement();
}

void MicrosetSplitter::init(int microsetSize)
{   targetSize = microsetSize / 2;  }
