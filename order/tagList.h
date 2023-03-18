#pragma once
#include <list>
#include <cstdint>

using Tag = int64_t;
using Tag_it = std::list <Tag> :: iterator;
using Int_it = std::list <int> :: iterator;

struct TagList {

    std::list<Tag> L; // list of tags

    Tag_it begin()
    {   return L.begin();   }

    Tag_it end()
    {   return L.end(); }

    int size()
    {   return L.size();    }
        
    /* renumrating tags of elements [a, b) into s, s+dx, s+dx*2, ... */
    void renum(Tag_it a, Tag_it b, Tag s, Tag dx)
    {
        for (Tag_it it = a; it != b; it++)
            *it = s, s += dx;
    }
};
