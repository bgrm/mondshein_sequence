#include "../order.h"
#include "sqrt.h"
#include <cassert>
#include <ctime>
#include <iostream>
//#include "dbg.h"
using std::cout;

vector<int> T;
vector<bool> inT;
Order* order;
OrderSqrt* brutForce;

const int checkConst = 3;
bool checkCorrectness = true;

void check()
{
    int i = rand() % T.size(), j = rand() % T.size();

    bool p = order->compare(T[i], T[j]);
    bool q = p;
    if (checkCorrectness)
        q = brutForce->compare(T[i], T[j]);

    if (p == q)
        return;

    order->print();
    cout << "T[" << i << "] = " << T[i] << ",  T[" << j << "] = " << T[j] << ";\n";
    cout << "order -> " << p << ",  sqrt -> " << q << ";;\n\n";

    delete order;
    delete brutForce;
    assert(false);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "usage: ./main '# inserts'\n";
        return 1;
    }

    int n = atoi(argv[1]);
    srand(n);
    T.resize(1, 0);
    inT.resize(n + 1, false);
    inT[0] = true;

    order = new Order(n);
    brutForce = new OrderSqrt(n);

    long long start = time(NULL), prv = start;

    for (int sz = 0; sz < n; sz++) {
        int i = rand() % T.size(), j = 0;
        while (inT[j])
            j = rand() % n + 1;

        int x = T[i + (T[i] == 0 and sz % 2 == 1)];
        sz % 2 == 0 ? order->insert(x, j) : order->insertBefore(x, j);

        if (checkCorrectness)
            sz % 2 == 0 ? brutForce->insert(x, j) : brutForce->insertBefore(x, j);

        T.push_back(j);
        inT[j] = true;
        for (int step = 0; step < checkConst; step++)
            check();

        if (sz % 500 == 0) {
            long long cr = time(NULL);
            cout << "n = " << sz << "; dt =  " << cr - prv << ", t = " << cr - start << "\n";
            prv = cr;
        }
    }
    cout << "reached size of " << n << ", passed all tests, total time: " << time(NULL) - start << "s.\n";

    delete order;
    delete brutForce;
    return 0;
}
