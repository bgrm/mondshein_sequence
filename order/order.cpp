#include "order.h"
#include <cassert>
#include <iostream>
using std::cout;

//most significant bit
#define MSB(x) (sizeof(long long) * 8 - 1 - __builtin_clzll(x))

template <typename TT>
void moveHalf(list <TT>& a, list <TT>& b)
{
    auto it = a.begin();
    advance(it, a.size() / 2);
    b.splice(b.end(), a, it, a.end());
}

/********************************** TagList **********************************/
/*****************************************************************************/


Tag_it TagList::begin()
{   return L.begin();   }

Tag_it TagList::end()
{   return L.end(); }

size_t TagList::size()
{   return L.size();    }

/* renumrating tags of elements [a, b) into s, s+dx, s+dx*2, ... */
void TagList::renum(Tag_it a, Tag_it b, Tag s, Tag dx)
{
    for (Tag_it it = a; it != b; it++)
        *it = s, s += dx;
}

/********************************** TagListMinor **********************************/
/**********************************************************************************/

int TagListMinor::offset;

TagListMinor::TagListMinor(Tag_it fat)
{
    fatherTag = fat;
}

void TagListMinor::setOffset(int n)
{   offset = n; }

/* extracts second half of 'this' list into new returned TagListMinor.
New TagList will be a succ of 'this' in higher instance */
TagListMinor* TagListMinor::split(Tag_it newFatTag) // O(this' length)
{
    assert((int)size() > 1);

    TagListMinor* ret = new TagListMinor(newFatTag);        
    moveHalf(L, ret->L);
    moveHalf(index, ret->index);

    renum(begin(), end(), 0, offset);
    ret->renum(ret->begin(), ret->end(), 0, offset);
    return ret;
}

/* new element's tag is after it, and new's index is after idxit */
Tag_it TagListMinor::insert(Tag_it it, Int_it idxit, int key) // O(1)
{
    Tag newTag;
    if (next(it) == L.end())
        newTag = *it + offset;
    else
        newTag = (*it + *next(it)) / 2;

    L.insert(next(it), newTag);
    index.insert(next(idxit), key);
    return next(it);
}

/********************************** TagListMajor **********************************/
/**********************************************************************************/

TagListMajor::TagListMajor()
{   L.push_back(0); }

/* finding sparse enough segment and relabeling tags within this segment */
void TagListMajor::relabel(Tag_it it) // O(this' length)
{
    assert(next(it) != L.end());

    Tag a = *it, b = a + 1;       // tags' range
    Tag len = 1, pow3 = 1;        // len - power of 2

    Tag_it ita = it, itb = next(it);      // elements' range with tags within [a, b)
    int count = 1;                      // #elements between ita and itb

    /* walking up the infinite segment tree spanned upon the tags' universe until finding sparse segment */
    while (count >= pow3 / len) // sprase constant equal 3/4
    {
        assert(a % len == 0);
        a / len % 2 == 1 ?  a -= len : b += len;
        len *= 2;
        pow3 *= 3;

        while (ita != begin() and *prev(it) >= a)
            ita--, count++;
        while (itb != end() and *itb <= b)
            itb++, count++;
    }

    renum(ita, itb, a, len / count);
}

/* inserts new element after it; returns pointer to new element's tag */
Tag_it TagListMajor::insert(Tag_it it) // O(log (this' length) amortized)
{
    if (next(it) != end() and *next(it) == *it + 1)
        relabel(it);
    L.insert(next(it), *it + 1);
    return next(it);
}

/********************************** Order **********************************/
/***************************************************************************/
 
Order::Order(int n) // max number of elements
{
    minorMax = std::max(4, (int)MSB(n));
    TagListMinor::setOffset(std::max(++n, 1 << minorMax));
    TagListMinor* minor0 = new TagListMinor(major.begin()); // first TagListMinor
    minor0->L.push_back(0);
    minor0->index.push_back(0);
    minors.push_back(minor0);

    minorIt.resize(n, minors.begin());              // initing with parameters for 0
    minorTag.resize(n, minor0->begin());
    indexIt.resize(n, minor0->index.begin());
}

pair <Tag, Tag> Order::tag(int i)
{
    TagListMinor* myMinorList = *minorIt[i];
    return {*(myMinorList->fatherTag), *minorTag[i]};
}

/* returns order[i] < order[j] */
bool Order::compare(int i, int j) // O(1)
{
    return tag(i) < tag(j);
}

int Order::predecessor(int i)
{
    Int_it index = indexIt[i];
    Minor_it minor = minorIt[i];
    if (index == (*minor)->index.begin())
    {
        Minor_it prv = prev(minor);
        return (*prv)->index.back();
    }
    else
    {
        return *prev(index);
    }
}

int Order::successor(int i)
{
    Int_it index = next(indexIt[i]);
    Minor_it minor = minorIt[i];
    int ret;
    if (index == (*minor)->index.end())
    {
        Minor_it nxt = next(minor);
        ret = (*nxt)->index.front();
    }
    else
    {
        ret = *index;
    }
    return ret;
}

/* Insert j = 1..n after i. When inserting j at the begining, set i=0 */
void Order::insert(int i, int j) // O(1 amortized)
{
    Minor_it minorIter = minorIt[i];
    TagListMinor* minorList = *minorIter;

    minorIt[j] = minorIter;
    minorTag[j] = minorList->insert(minorTag[i], indexIt[i], j);
    indexIt[j] = next(indexIt[i]);

    if (minorList->size() == minorMax)  // serving split 
    {
        Tag_it newMajorTag = major.insert(minorList->fatherTag);    // updating major
        TagListMinor* newMinor = minorList->split(newMajorTag);     // splitting minor
        minors.insert(next(minorIter), newMinor);                   // updating minors

        for (int k : newMinor->index)                               // updating minorIt
            if (k != 0)
                minorIt[k] = next(minorIter);
    }
}

void Order::insertBefore(int i, int j)
{   insert(predecessor(i), j);  }

Order::~Order()
{
	for (auto p : minors)
	{
		p->L.clear();
		p->index.clear();
		delete p;
	}
	minors.clear();
	major.L.clear();
    minorIt.clear();
    minorTag.clear();
    indexIt.clear();
}


/********************************** Debug **********************************/
/***************************************************************************/


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
