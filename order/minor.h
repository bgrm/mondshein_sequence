#pragma once
#include "tagList.h"

template <typename TT>
void moveHalf(std::list<TT>& a, std::list<TT>& b);

struct TagListMinor : TagList {

    static int offset;  // default offset between consecutive tags in minor TagLists
    
    Tag_it fatherTag;   // pointer to the tag representing 'this' in higher instance
    std::list<int> index;    // list of elem's whose tags are in L

    TagListMinor(Tag_it fat)
    : fatherTag(fat) {}

    static void setOffset(int n)
    {   offset = n; }


    /* extracts second half of 'this' list into new returned TagListMinor.
    New TagList will be a succ of 'this' in higher instance */
    TagListMinor* split(Tag_it newFatTag) // O(this' length)
    {
        TagListMinor* ret = new TagListMinor(newFatTag);
        moveHalf(L, ret->L);
        moveHalf(index, ret->index);

        renum(begin(), end(), 0, offset);
        ret->renum(ret->begin(), ret->end(), 0, offset);
        return ret;
    }

    /* new element's tag is after it, and new's index is after idxit */
    Tag_it insert(Tag_it it, Int_it idxit, int key) // O(1)
    {
        Tag newTag;
        if (std::next(it) == L.end())
            newTag = *it + offset;
        else
            newTag = (*it + *next(it)) / 2;

        L.insert(next(it), newTag);
        index.insert(std::next(idxit), key);
        return std::next(it);
    }

    void print();
};

