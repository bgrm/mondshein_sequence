#include "ops.h"
#include "element.h"
#include "micro.h"
#include "mezzo.h"
#include "path.h"
#include "../shorts.h"
#include <cassert>

extern std::vector <Element> E;
extern Mezzoset::SelfIter_t unlabeledMicrosets;

void setupMarginalEdges(Path::SelfIter_t path, int left, int right);
int labelNewPath(Path::SelfIter_t p, Path::SelfIter_t q);
void connect(int l, int r, bool connectInMicro);
void insertAfter(Element& edgeOld, Element& edgeFresh);
bool isBorderVertex(int e, int v);

namespace {

template <typename T, typename... S>
void performSplitAndStoreSplitants(T& base, typename T::SelfIter_t& left, typename T::SelfIter_t& right, S... pivots)
{
    right = base.selfptrInFather;
    left = base.split(pivots...);
    assert(left->next() == right or right->next() == left);
    if (right->next() == left)
        swap(left, right);
}

auto splitMicroAndMezzoAndGetMezzoForPathSplit(Element& edgeLeft, bool relabelLeftsMicro, bool relabelRightsMicro)
{
    auto& edgeRight = E[edgeLeft.right]; 
    auto microLeft = edgeLeft.father;
    auto microRight = edgeRight.father;
    auto microForMezzoSplit = relabelRightsMicro ? microRight->next() : microRight;
    auto mezzoRight = microRight->father;
    auto oldPath = edgeRight.find();
    Mezzoset::SelfIter_t mezzoForPathSplit;

    if (edgeRight.isInConsistentMezzo())
        mezzoForPathSplit = microRight->isRightMarginal() and relabelRightsMicro ? mezzoRight->next() : mezzoRight;
    else
        mezzoForPathSplit = microRight == oldPath->leftMicro() ? oldPath->first() : oldPath->end();

    if (microRight == microLeft)
    {
        bool relabelMicro = relabelLeftsMicro and relabelRightsMicro;
        edgeRight.breakPath(true, relabelMicro);
    }
    else
    {
        edgeLeft.breakPath(false, relabelLeftsMicro);
        edgeRight.breakPath(true, relabelRightsMicro);
    }

    bool mezzoSplit = 
        mezzoRight == mezzoForPathSplit and
        microForMezzoSplit != mezzoRight->end() and
        microForMezzoSplit != mezzoRight->first() and
        microForMezzoSplit->microLabel() == oldPath;

    if (mezzoSplit)
    {
        assert(mezzoRight == mezzoForPathSplit);
        performSplitAndStoreSplitants(*mezzoRight, mezzoRight, mezzoForPathSplit, microForMezzoSplit);
    }

    return mezzoForPathSplit;
}

// both edges are in the same mezzo but in different micros
auto splitMicrosAndMezzoAndGetMezzoForPathSplit(Element& edgeLeft, Element& edgeRight)
{
    auto microLeft = edgeLeft.father;
    auto microRight = edgeRight.father;
    auto mezzo = edgeLeft.mezzo();

    bool leftMarginal = edgeLeft.isLeftMarginalInMicro();
    bool rightMarginal = edgeRight.isRightMarginalInMicro();
    bool relabelLeft = leftMarginal;
    bool relabelRight = rightMarginal;

    auto micro1 = microLeft->next();
    auto micro2 = relabelRight ? microRight->next() : microRight;

    if (micro1 == microRight and relabelRight)
        micro1 = micro2;

    bool mezzoErased = mezzo->size() == 2 and relabelLeft and relabelRight;

    edgeLeft.breakPath(true, relabelLeft);
    edgeRight.breakPath(false, relabelRight);

    bool mezzoExtractedWholy = !mezzoErased and micro1 == mezzo->first() and micro2 == mezzo->end();
    bool mezzoSplit = !mezzoErased and !mezzoExtractedWholy and micro1 != micro2;

    auto mezzoForPathSplit = mezzo->father->end(); // end() of the mezzo's path

    if (mezzoSplit)
        performSplitAndStoreSplitants(*mezzo, mezzo, mezzoForPathSplit, micro1, micro2);

    else if (mezzoExtractedWholy)
        mezzoForPathSplit = mezzo;

    return mezzoForPathSplit;
}

// i.e. when the fresh incides with marignal vertex
int checkMarginalCase(int leftId, int rightId, Element& fresh)
{
    int v = fresh.getLinkingVertex(E[leftId]);
    int w = fresh.getLinkingVertex(E[rightId]);
    assert(edgeEq({v, w}, fresh.getVertices()));

    int leftOfLeft = E[leftId].left;
    int rightOfRight = E[rightId].right;
    
    if (E[leftOfLeft].incident(v))
        leftId = leftOfLeft;
    if (E[rightOfRight].incident(w))
        rightId = rightOfRight;

    if (PathsStructure::isBorderVertex(leftId, v))
    {
        int ret = PathsStructure::split(rightId, w);
        PathsStructure::addVertex(rightId, fresh.id);
        return ret;
    }
    if (PathsStructure::isBorderVertex(rightId, w))
    {
        int ret = PathsStructure::split(leftId, v);
        PathsStructure::addVertex(leftId, fresh.id);
        return ret;
    }
    return 0;
}

// left and rigt become leftmost and rightmost edges of the internal path
void setupInitialEdges(int& leftId, int& rightId, Element& fresh)
{
    auto& edgeLeft = E[leftId];
    auto& edgeRight = E[rightId];
    auto& leftOfLeft = E[edgeLeft.left];
    auto& rightOfRight = E[edgeRight.right];

    if (!leftOfLeft.incident(fresh))
        leftId = edgeLeft.right;

    if (!rightOfRight.incident(fresh))
        rightId = edgeRight.left;
}

void setupMezzosForPathSplit(Element& edgeLeft, Element& edgeRight, Mezzoset::SelfIter_t& mezzo1, Mezzoset::SelfIter_t& mezzo2)
{
    auto oldPath = edgeLeft.find();
    mezzo1 = mezzo2 = oldPath->end();

    if (edgeLeft.father == edgeRight.father)    // both edges in one micro
    {
        bool microRelabel = edgeLeft.isLeftMarginalInMicro() and edgeRight.isRightMarginalInMicro();
        edgeLeft.breakPath(true, microRelabel);
        edgeRight.breakPath(false, false);
    }

    else if (edgeLeft.mezzo() == edgeRight.mezzo() and edgeLeft.isInConsistentMezzo())  // both edges in one consistent mezzo
    {
        mezzo1 = splitMicrosAndMezzoAndGetMezzoForPathSplit(edgeLeft, edgeRight);
        if (mezzo1 != oldPath->end())
            mezzo2 = mezzo1->next();
    }

    else
    {
        auto& leftOfLeft = E[edgeLeft.left];
        mezzo2 = splitMicroAndMezzoAndGetMezzoForPathSplit(edgeRight, true, false);
        mezzo1 = splitMicroAndMezzoAndGetMezzoForPathSplit(leftOfLeft, false, true);
    }
}
}

namespace PathsStructure {

int split(int edgeId, int v)
{
    int leftId = E[edgeId].left;
    if (E[leftId].incident(v))
        edgeId = leftId;

    auto& edgeLeft = E[edgeId];
    auto& edgeRight = E[edgeLeft.right];
    auto oldPath = edgeRight.find();
    int oldMarginalEdgeLeft = oldPath->leftEdge;
    int oldMarginalEdgeRight = oldPath->rightEdge;
    
    auto mezzoForPathSplit = splitMicroAndMezzoAndGetMezzoForPathSplit(edgeLeft, true, true);

    Path::SelfIter_t leftPath, rightPath;
    performSplitAndStoreSplitants(*oldPath, leftPath, rightPath, mezzoForPathSplit);

    setupMarginalEdges(leftPath, oldMarginalEdgeLeft, edgeLeft.id);
    setupMarginalEdges(rightPath, edgeRight.id, oldMarginalEdgeRight);

    return labelNewPath(leftPath, rightPath);
}

int insertEdge(int fresh, int oldLeft, int oldRight)
{    
    if (E[oldRight].earlierThan(E[oldLeft]))
        std::swap(oldRight, oldLeft);

    if (int newPathId = checkMarginalCase(oldLeft, oldRight, E[fresh]); newPathId != 0)
        return newPathId;

    setupInitialEdges(oldLeft, oldRight, E[fresh]);

    auto& edgeLeft = E[oldLeft];
    auto& edgeRight = E[oldRight];
    auto& edgeFresh = E[fresh];
    auto& leftOfLeft = E[edgeLeft.left];
    auto& rightOfRight = E[edgeRight.right];

    auto oldPath = edgeLeft.find();
    int oldMarginalEdgeLeft = oldPath->leftEdge;
    int oldMarginalEdgeRight = oldPath->rightEdge;

    Mezzoset::SelfIter_t mezzo1, mezzo2;
    setupMezzosForPathSplit(edgeLeft, edgeRight, mezzo1, mezzo2);

    Path::SelfIter_t externalPath, internalPath;
    performSplitAndStoreSplitants(*oldPath, externalPath, internalPath, mezzo1, mezzo2);

    setupMarginalEdges(externalPath, oldMarginalEdgeLeft, oldMarginalEdgeRight);
    setupMarginalEdges(internalPath, edgeLeft.id, edgeRight.id);

    connect(leftOfLeft.id, rightOfRight.id, true);
    insertAfter(leftOfLeft, edgeFresh);

    return labelNewPath(externalPath, internalPath);
}
}
