#pragma once
#include <list>
#include <utility>

struct Microset;
struct Mezzoset;
struct Path;

template <typename T>
using Iter_t = typename std::list <T> :: iterator;

extern Iter_t <Path> nullPath;
extern Iter_t <Microset> nullMicroset;

struct Element
{
    Iter_t <Microset> father = nullMicroset;
    Iter_t <Path> pathPtr = nullPath;
    const int id;
    int indexInMicro = 0, left = 0, right = 0;

    Element(int i)
    : id(i) {}

    bool isMarginal() const;

    bool isLeftMarginal() const;

    bool isRightMarginal() const;

    bool isLeftMarginalInMicro() const;

    bool isRightMarginalInMicro() const;

    Iter_t<Path> find() const;

    bool isInConsistentMezzo() const;

    int positionInSubpath() const;

    bool earlierThan(const Element& other) const;

    Iter_t<Mezzoset> mezzo() const;

    std::pair <int, int> getVertices() const;

    bool incident(int v) const;

    bool incident(const Element& other) const;

    int getLinkingVertex(const Element& other) const;
    

    void unmarkAsMarginal();

    void markAsMarginal(Iter_t <Path> p, bool leftEnd);

    void clear();

    void placeInMicro(Iter_t <Microset> micro, int index = -1);

    void replaceInMicroWith(Element& other);

    void breakPath(bool leftDirection, bool allowMicroRelabel);
};
