//
// Created by rober on 26/12/2022.
//

#ifndef PLANG3_LOOPBLOCK_H
#define PLANG3_LOOPBLOCK_H

#include "../BlockHandler.h"
#include "BlockController.h"


class LoopBlock : public BlockController {
private:
    ADDR entryAddr = 0;
public:
    void OnEnter(BlockHandler *oldBlockHandler, BlockHandler *newBlockHandler,
                 const vector<LexicalResult> &entryLine) override;

    bool OnExit(BlockHandler* blockHandler, const vector<LexicalResult>& exitingLine, int depthDelta) override;
};


#endif //PLANG3_LOOPBLOCK_H
