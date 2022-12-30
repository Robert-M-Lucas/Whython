//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_BLOCKHANDLER_H
#define PLANG3_BLOCKHANDLER_H

#include <iostream>
#include <vector>

#include "../References/ReferenceHandler.h"
#include "MemoryManager.h"
#include "../Blocks/BlockController.h"

using namespace std;

class BlockHandler {
public:
    BlockController* Controller;
    int Depth;
    BlockHandler* Above;
    ReferenceHandler* References;
    MemoryManager* PManager;
    MemoryManager* VManager;

    BlockHandler(BlockHandler* above, BlockController* controller, MemoryManager* pManager, MemoryManager* mManager);

    BlockHandler* OnLineParsed(vector<LexicalResult> parsedLine);

    bool OnExit(const vector<LexicalResult>& exitingLine, int depthDelta);

    Reference* RecursivelyGetReference(const string& referenceName);
    Reference* RecursivelyGetReferenceOrNull(const string &referenceName);

    ~BlockHandler();
};


#endif //PLANG3_BLOCKHANDLER_H
