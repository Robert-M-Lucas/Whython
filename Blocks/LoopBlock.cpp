//
// Created by rober on 26/12/2022.
//


using namespace std;

#include <iostream>

#include "LoopBlock.h"
#include "../Instructions/GotoInstruction.h"

void LoopBlock::OnEnter(BlockHandler *oldBlockHandler, BlockHandler *newBlockHandler,
                        const vector<LexicalResult> &entryLine) {
    cout << "Enter block" << endl;
    entryAddr = newBlockHandler->PManager->GetCurrentPosition();
}

bool LoopBlock::OnExit(BlockHandler* blockHandler, const vector<LexicalResult>& exitingLine, int depthDelta) {
    blockHandler->PManager->Append(GotoInstruction::Build(entryAddr));
    cout << "Exit block" << endl;
    return true;
}