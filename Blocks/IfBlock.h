//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_IFBLOCK_H
#define PLANG3_IFBLOCK_H

#include "../BlockHandler.h"
#include "BlockController.h"

class IfBlock : public BlockController {
private:
    ADDR entryAddr = 0;
    bool constant = false;
    bool constant_value = false;
    ADDR nameAddr = 0;
    int elsePosition = -1;
public:
    void OnEnter(BlockHandler *oldBlockHandler, BlockHandler *newBlockHandler,
                 const vector<LexicalResult> &entryLine) override;

    bool OnExit(BlockHandler* blockHandler, const vector<LexicalResult>& exitingLine, int depthDelta) override;
};


#endif //PLANG3_IFBLOCK_H
