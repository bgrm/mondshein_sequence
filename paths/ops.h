#pragma once
#include <vector>


namespace PathsStructure {

int newPath(int e);

void init(int m);

int find(int e);

void insertVertex(int e, int f, int g);

void addVertex(int e, int f);

int split(int edgeId, int v);

int insertEdge(int fresh, int oldLeft, int oldRight);


std::vector <int> getPath(int id);

bool isSingleEdge(int e);

bool isBorderVertex(int e, int v);

void eraseShort(int e);

}