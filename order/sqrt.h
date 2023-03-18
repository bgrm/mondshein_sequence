#pragma once
#include "order.h"
#include <list>
#include <vector>
using std::vector, std::list;

struct OrderSqrt : public Order {
    int d;
    vector<list<int>> L;
    vector<int> P;

    OrderSqrt(int n);
    void insert(int x, int y) override;
    int predecessor(int x) override;
    int successor(int x) override;
    void insertBefore(int x, int y) override;
    bool compare(int x, int y) override;

    ~OrderSqrt() override;
};
