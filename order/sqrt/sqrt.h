#ifndef __SQRT__
#define __SQRT__

#include <vector>
#include <list>
#include "../order.h"
using std::vector, std::list;

struct OrderSqrt : public Order
{
    int d;
    vector <list <int> > L;
    vector <int> P;

    OrderSqrt(int n);
    void insert(int x, int y) override;
    int predecessor(int x) override;
    int successor(int x) override;
    void insertBefore(int x, int y) override;
    bool compare(int x, int y) override;

    ~OrderSqrt() override;
};

#endif