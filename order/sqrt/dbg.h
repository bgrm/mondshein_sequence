#include "../order.h"
#include <iostream>
using std::cout;

template <typename TT>
void dbg(list <TT>& L)
{
    cout << "[";
    for (TT x : L)
        cout << x << ", ";
    cout << "];\n";
}

template <typename TT>
void dbg(vector <TT>& L)
{
    cout << "[";
    for (TT x : L)
        cout << *x << ", ";
    cout << "];\n";
}

void TagListMinor::print()
{
    cout << "faTag = " << *fatherTag << "\n";
    cout << "L:\t"; dbg(L);
    cout << "\nind:\t"; dbg(index);
    cout << "\n=================\n";
}

void TagListMajor::print()
{
    cout << "MAJOR:\n";
    cout << "L:" ;dbg(L);
    cout << "\n= = = = = = = = = = = = = \n\n";
} 

void Order::print()
{
    cout << "minorMax = " << minorMax << ";\t minorOffset = " << TagListMinor::offset << ";\n";
    major.print();
    for (auto& l : minors)
        l->print();

    cout << "\nminorIt:\t"; dbg(minorIt);
    cout << "\nminorTag:\t"; dbg(minorTag);
    cout << "\nindexIt:\t"; dbg(indexIt);

    cout << "\n#!# #!# #!# #!# #!# #!# #!# #!# #!# #!# #!# #!#\n\n\n";
}

