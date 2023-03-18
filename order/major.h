#pragma once
#include "tagList.h"
#include <cassert>

struct TagListMajor : TagList {

    TagListMajor()
    {   L.push_back(0); }

    /* finding sparse enough segment and relabeling tags within this segment */
    void relabel(Tag_it it) // O(this' length)
    {
        Tag a = *it, b = a + 1; // tags' range
        Tag len = 1, pow3 = 1; // len - power of 2

        Tag_it ita = it, itb = next(it); // elements' range with tags within [a, b)
        int count = 1; // #elements between ita and itb

        /* walking up the infinite segment tree spanned upon the tags' universe until finding sparse segment */
        while (count >= pow3 / len) // sprase constant equal 3/4
        {
            assert(a % len == 0);
            a / len % 2 == 1 ? a -= len : b += len;
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
    Tag_it insert(Tag_it it) // O(log (this' length) amortized)
    {
        if (next(it) != end() and *next(it) == *it + 1)
            relabel(it);
        L.insert(next(it), *it + 1);
        return next(it);
    }

    void print();
};
