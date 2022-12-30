//
// Created by rober on 23/12/2022.
//

#include <stdexcept>

#include "WhileBlock.h"
#include "../Compilation/BlockHandler.h"
#include "../Instructions/EqualsInstruction.h"
#include "../Instructions/GotoInstruction.h"
#include "../Instructions/GotoIfInstruction.h"
#include "../Types/BoolType.h"
#include "../Util/BoolTools.h"

void WhileBlock::OnEnter(BlockHandler *oldBlockHandler, BlockHandler *newBlockHandler,
                         const vector<LexicalResult> &entryLine) {
    if ((entryLine.size() != 2 && entryLine.size() != 4) || (entryLine[1].Type != Name && entryLine[1].Type != BoolLiteral))
        throw invalid_argument("While must be followed by a boolean");

    if (entryLine.size() == 4) {
        constant = false;
        entryAddr = newBlockHandler->PManager->GetCurrentPosition();
        nameAddr = ExpressionToBoolReference(newBlockHandler, entryLine[1], entryLine[2].Value, entryLine[3]);
        offset = newBlockHandler->PManager->GetCurrentPosition() - entryAddr;
        newBlockHandler->PManager->Alloc(GotoIfInstruction::GetLength());
        return;
    }

    if (entryLine[1].Type == BoolLiteral) {
        constant = true;
        if (entryLine[1].Value == "true") {
            constant_value = true;
            entryAddr = newBlockHandler->PManager->GetCurrentPosition();
        }
        else {
            constant_value = false;
            entryAddr = newBlockHandler->PManager->Alloc(GotoInstruction::GetLength());
        }
    }
    else {
        Reference* reference = newBlockHandler->RecursivelyGetReference(entryLine[1].Value);
        if (reference->TypeID != BoolType().GetID())
            throw invalid_argument("Name following while must be a bool");
        nameAddr = reference->Address;
        entryAddr = newBlockHandler->PManager->Alloc(GotoIfInstruction::GetLength());
    }
}

bool WhileBlock::OnExit(BlockHandler* blockHandler, const vector<LexicalResult>& exitingLine, int depthDelta) {
    if (constant && !constant_value) { // * while false
        blockHandler->PManager->Overwrite(entryAddr + offset, GotoInstruction::Build(blockHandler->PManager->GetCurrentPosition()));
    }
    else {
        if (constant) { // * while true
            blockHandler->PManager->Append(GotoInstruction::Build(entryAddr));
        }
        else {
            blockHandler->PManager->Append(GotoInstruction::Build(entryAddr));
            blockHandler->PManager->Overwrite(entryAddr + offset, GotoIfInstruction::Build(blockHandler->PManager->GetCurrentPosition(), nameAddr,
                                                                                  true));
        }
    }

    return true;
}