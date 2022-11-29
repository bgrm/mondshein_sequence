#ifndef __PATHS__
#define __PATHS__

#include <algorithm>
#include <vector>
using std::vector, std::pair;

/*
* in our case of usage of that structure we may make an assumption, that:
* EACH VERTEX IS INNER WITHIN AT MOST ONE PATH
*/

namespace PathsStructure {
int newPath(pair<int, int> e);
int find(pair<int, int> e);
void insertVertex(pair<int, int> e, int v);
void addVertex(pair<int, int> e, int v);
int split(pair<int, int> e, int v);
int insertEdge(pair<int, int> e, int a, int b);

void setN(int n);
pair<int, int> borderEdge(int p);
vector<int> getPath(int p);
bool isSingleEdge(pair<int, int> e);
bool isBorderVertex(pair<int, int> e, int v);
void eraseShort(pair<int, int> e);
void clear();
}

#endif // __PATHS__
