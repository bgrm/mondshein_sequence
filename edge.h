#ifndef __EDGE__
#define __EDGE__
#include "bgseq/decom.h"
#include <array>

inline Edge flip(const Edge& e)
{   return std::make_pair(e.second, e.first);   }

inline bool edgeEq(const Edge& e, const Edge& f)
{   return e == f or e == flip(f);  }

constexpr auto bgopSize = 7;
using BGopEx = std::array <int, bgopSize>;
    
vector <BGopEx> getBGopsExteneded(const vector <BGop>& base, int n);

Edge getEdge(int i);

int getInd(const Edge& e, BGopEx* op);

void insertShort(int i);

void insertShort(const Edge& e, BGopEx* op);

void removeShort(int i);

void removeShort(const Edge& e, BGopEx* op);

bool shortEar(int i);

bool shortEar(const Edge& e, BGopEx* op);

vector <Edge> getAllShorts();

void clearShorts();

#endif // __EDGE__
