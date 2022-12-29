//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_BLOCKCONTROLLER_H
#define PLANG3_BLOCKCONTROLLER_H

#include <vector>

#include "../Preprocessing/LexicalResult.h"

using namespace std;

class BlockHandler;

class BlockController {
public:
    virtual void
    OnEnter(BlockHandler *oldBlockHandler, BlockHandler *newBlockHandler, const vector<LexicalResult> &entryLine) {}
    virtual bool OnExit(BlockHandler* blockHandler, const vector<LexicalResult>& exitingLine, int depthDelta) { return false; }
};


#endif //PLANG3_BLOCKCONTROLLER_H
