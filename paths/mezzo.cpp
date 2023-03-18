#include "mezzo.h"
#include "micro.h"
#include "path.h"
#include <cassert>

extern Mezzoset::SelfIter_t unlabeledMicrosets;

int Mezzoset::MezzosetSize;

void Mezzoset::anectMicroset(SonIter_t micro, bool atEnd)
{
    auto oldMezzo = micro->father;
    auto destination = atEnd ? end() : first();

    splice(*oldMezzo, micro, micro->next(), destination, false);

    if (oldMezzo->empty() and oldMezzo != unlabeledMicrosets)
        oldMezzo->destroy();

    if (size() == MezzosetSize)
        halve();
}

void Mezzoset::halve()
{
    if (selfptrInFather == unlabeledMicrosets)
        return;
    assert(size() > 1);
    auto pivot = first();
    std::advance(pivot, size() / 2);
    split(pivot);
}
