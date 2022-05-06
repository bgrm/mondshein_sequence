#ifndef __DBG
#define __DBG
#define cout cerr

#include <bits/stdc++.h>
using namespace std;
#define PLY(x) template <class x> 
#define PLY2(x, y) template <class x, class y>
#define PLY3(x, y, z) template <class x, class y, class z>
#define dbg(...) cout << "[#" << #__VA_ARGS__ << "]<< ", _debug(__VA_ARGS__), cout << "\n"
#define FOR(i, a, b) for (int i=a; (i)<(b); i++)


void _debug(bool x);
void _debug(int x);
void _debug(long long x);
void _debug(float x);
void _debug(double x);
void _debug(long double x);
void _debug(char c);
void _debug(const string& s);
void _debug(char* s);
PLY2(_t1, _t2) void _debug(tuple<_t1, _t2>& x);
PLY3(_t1, _t2, _t3) void _debug(tuple<_t1, _t2, _t3>& x);
PLY2(_t1, _t2) void _debug(const pair <_t1, _t2>& p);

PLY(_t) void _debug(_t* p, int n);
PLY(_t) void _debug(_t* p, int n, int m);

PLY(_t) void _debug(const vector <_t>& v);
PLY(_t) void _debug(const list <_t>& L);
PLY(_t) void _debug(const set <_t>& S);
PLY(_t) void _debug(const multiset <_t>& S);
PLY2(_t1, _t2) void _debug(const map <_t1, _t2>& M);

PLY(_t) void _debug(unordered_set <_t> S);
PLY2(_t1, _t2) void _debug(unordered_map <_t1, _t2> M);

PLY(_t) void _debug(queue <_t> Q);
PLY(_t) void _debug(priority_queue <_t> H);
PLY(_t) void _debug(stack <_t> S);
PLY(_t) void _debug(deque <_t> Q);

template <int _t> void _debug(const bitset <_t>& B);

#endif // __DBG