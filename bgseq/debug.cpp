#include "debug.h"

void _debug(bool x)
{	if (x)	cout << "true";
	else	cout << "false";	}

void _debug(int x)
{	cout << x; }

void _debug(long long x)
{	cout << x; }

void _debug(float x)
{	cout << x; }

void _debug(double x)
{	cout << x; }

void _debug(long double x)
{	cout << x; }

void _debug(char c)
{	cout << c; }

void _debug(const string& s)
{	cout << s; }

void _debug(char* s)
{	printf("%s", s);	}


PLY2(_t1, _t2) void _debug(tuple<_t1, _t2>& x)
{	cout << "<";
	_debug(get<0>(x));
	cout << ", ";
	_debug(get<1>(x));
	cout << ">";
}

PLY3(_t1, _t2, _t3) void _debug(tuple<_t1, _t2, _t3>& x)
{	cout << "<";
	_debug(get<0>(x));
	cout << ", ";
	_debug(get<1>(x));
	cout << ", ";
	_debug(get<2>(x));
	cout << ">";
}

PLY2(_t1, _t2) void _debug(const pair <_t1, _t2>& p)
{	cout << "<";
	_debug(p.first);
	cout << ", ";
	_debug(p.second);
	cout << ">";
}

PLY(_t) void _debug(_t* p, int n)
{
	assert(n >= 0);
	cout << "[";
	while (n--)
		_debug(*p++), cout << ", ";
	cout << "]";
}

PLY(_t) void _debug(_t* p, int n, int m)
{
	assert(n >= 0);  assert(m >= 0);
	cout << "[";
	FOR(i, 0, n)
		cout << "\n   " << i << ": ", _debug(*p++, m);
	cout << "]";
}

#define BODY_CONT(v)\
{\
	cout << "[";\
	for (auto _i : v)\
	{\
		_debug(_i);\
		cout << ", ";\
	}\
	cout << "]";\
}
PLY(_t) void _debug(const vector <_t>& v)	BODY_CONT(v)

PLY(_t) void _debug(const list <_t>& L)	BODY_CONT(L)

PLY(_t) void _debug(const set <_t>& S)	BODY_CONT(S)

PLY(_t) void _debug(const multiset <_t>& S)	BODY_CONT(S)

PLY2(_t1, _t2) void _debug(const map <_t1, _t2>& M)	BODY_CONT(M)

void echh()
{
	vector <vector <int> > w;
	_debug(w);
}

#define BODY_UNORD(v, memb) \
{ \
	cout << "["; \
	while (!v.empty()) \
	{ \
		auto i = *(v.begin()); \
		_debug(i); \
		cout << ", "; \
		v.erase(memb);	\
	}\
	cout << "]"; \
}
PLY(_t) void _debug(unordered_set <_t> S)	BODY_UNORD(S, i)

PLY2(_t1, _t2) void _debug(unordered_map <_t1, _t2> M) BODY_UNORD(M, i.first)

#define BODY_QUE(Q, memb)	\
{	cout << "[";	\
	for (; !Q.empty(); Q.pop())	\
		_debug(Q.memb()), cout << ", ";	\
	cout << "]";	\
}
PLY(_t) void _debug(queue <_t> Q)	BODY_QUE(Q, front)

PLY(_t) void _debug(priority_queue <_t> H)	BODY_QUE(H, top)

PLY(_t) void _debug(stack <_t> S)	BODY_QUE(S, top)

PLY(_t) void _debug(deque <_t> Q)
{	
	cout << "[";
	for (; !Q.empty(); Q.pop_front())
		_debug(Q.front()), cout << ", ";
	cout << "]";
}

template <long unsigned int _t> void _debug(const bitset <_t>& B)
{	_debug(B.to_string());	}
