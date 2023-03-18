#include "order.h"

int TagListMinor::offset;

//most significant bit
#define MSB(x) (sizeof(long long) * 8 - 1 - __builtin_clzll(x))

template <typename TT>
void moveHalf(std::list<TT>& a, std::list<TT>& b)
{
    auto it = a.begin();
    advance(it, a.size() / 2);
    b.splice(b.end(), a, it, a.end());
}


Order::Order(int n) // max number of elements
{
    minorMax = std::max(4, (int)MSB(n));
    TagListMinor::setOffset(std::max(++n, 1 << minorMax));
    TagListMinor* minor0 = new TagListMinor(major.begin()); // first TagListMinor
    minor0->L.push_back(0);
    minor0->index.push_back(0);
    minors.push_back(minor0);

    minorIt.resize(n, minors.begin()); // initing with parameters for 0
    minorTag.resize(n, minor0->begin());
    indexIt.resize(n, minor0->index.begin());
}

std::pair<Tag, Tag> Order::tag(int i)
{
    TagListMinor* myMinorList = *minorIt[i];
    return { *(myMinorList->fatherTag), *minorTag[i] };
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
    if (index == (*minor)->index.begin()) {
        Minor_it prv = prev(minor);
        return (*prv)->index.back();
    } else {
        return *prev(index);
    }
}

int Order::successor(int i)
{
    Int_it index = next(indexIt[i]);
    Minor_it minor = minorIt[i];
    int ret;
    if (index == (*minor)->index.end()) {
        Minor_it nxt = next(minor);
        ret = (*nxt)->index.front();
    } else {
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

    if (minorList->size() == minorMax) // serving split
    {
        Tag_it newMajorTag = major.insert(minorList->fatherTag); // updating major
        TagListMinor* newMinor = minorList->split(newMajorTag); // splitting minor
        minors.insert(next(minorIter), newMinor); // updating minors

        for (int k : newMinor->index) // updating minorIt
            if (k != 0)
                minorIt[k] = next(minorIter);
    }
}

void Order::insertBefore(int i, int j)
{
    insert(predecessor(i), j);
}

Order::~Order()
{
    for (auto p : minors) {
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
