#pragma once
#include "minor.h"
#include "major.h"
#include <list>
#include <vector>

struct TagListMinor;

using Minor_it = std::list <TagListMinor*> :: iterator;

struct Order {
    
    TagListMajor major;
    std::list<TagListMinor*> minors;
    int minorMax; // max minor's size; after reaching this size minor splits

    std::vector<Minor_it> minorIt; // pointer to elem's minorTagList
    std::vector<Tag_it> minorTag; // pointer to elem's tag in proper minorTagList
    std::vector<Int_it> indexIt; // pointer to elem's index in proper minorTagList

    /* n - max number of elements */
    Order(int n = 0);

    std::pair<Tag, Tag> tag(int i);

    /* returns order[i] < order[j] */
    virtual bool compare(int i, int j); // O(1)

    virtual int predecessor(int i);
    virtual int successor(int i);

    /* Inserts j = 1..n after i. When inserting j at the begining, set i=0 */
    virtual void insert(int i, int j); // O(1 amortized)

    virtual void insertBefore(int i, int j);

    virtual ~Order();

    void print();
};
