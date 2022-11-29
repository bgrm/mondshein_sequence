#ifndef __ORDER__
#define __ORDER__

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <list>
#include <vector>
using std::pair, std::vector, std::list;

using Tag = int64_t;
using Tag_it = list<Tag>::iterator;
using Int_it = list<int>::iterator;

template <typename TT>
void moveHalf(list<TT>& a, list<TT>& b);

struct TagList {
    list<Tag> L; // list of tags

    Tag_it begin();
    Tag_it end();
    size_t size();

    /* renumrating tags of elements [a, b) into s, s+dx, s+dx*2, ... */
    void renum(Tag_it a, Tag_it b, Tag s, Tag dx);
};

struct TagListMinor : TagList {
    Tag_it fatherTag; // pointer to the tag representing 'this' in higher instance
    list<int> index; // list of elem's whose tags are in L
    static int offset; // default offset between consecutive tags in minor TagLists

    TagListMinor(Tag_it fat);
    static void setOffset(int n);

    /* extracts second half of 'this' list into new returned TagListMinor.
    New TagList will be a succ of 'this' in higher instance */
    TagListMinor* split(Tag_it newFatTag); // O(this' length)

    /* new element's tag is after it, and new's index is after idxit */
    Tag_it insert(Tag_it it, Int_it idxit, int key); // O(1)

    void print();
};
using Minor_it = list<TagListMinor*>::iterator;

struct TagListMajor : TagList {
    TagListMajor();

    /* finding sparse enough segment and relabeling tags within this segment */
    void relabel(Tag_it it); // O(this' length)

    /* inserts new element after it; returns pointer to new element's tag */
    Tag_it insert(Tag_it it); // O(log (this' length) amortized)

    void print();
};

struct Order {
    TagListMajor major;
    list<TagListMinor*> minors;
    size_t minorMax; // max minor's size; after reaching this size minor splits

    vector<Minor_it> minorIt; // pointer to elem's minorTagList
    vector<Tag_it> minorTag; // pointer to elem's tag in proper minorTagList
    vector<Int_it> indexIt; // pointer to elem's index in proper minorTagList

    /* n - max number of elements */
    Order(int n = 0);

    pair<Tag, Tag> tag(int i);

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

#endif
