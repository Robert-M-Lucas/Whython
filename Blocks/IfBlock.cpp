//
// Created by rober on 23/12/2022.
//

#include <stdexcept>

#include "IfBlock.h"
#include "../BlockHandler.h"
#include "../Instructions/EqualsInstruction.h"
#include "../Instructions/GotoInstruction.h"
#include "../Instructions/GotoIfInstruction.h"
#include "../Types/BoolType.h"
#include "../References/ReferenceHandler.h"
#include "../Util/BoolTools.h"

void IfBlock::OnEnter(BlockHandler *oldBlockHandler, BlockHandler *newBlockHandler,
                         const vector<LexicalResult> &entryLine) {
    if ((entryLine.size() != 2 && entryLine.size() != 4) || (entryLine[1].Type != Name && entryLine[1].Type != BoolLiteral))
        throw invalid_argument("If must be followed by a boolean");

    if (entryLine.size() == 4) {
        // TODO
        constant = false;
        nameAddr = ExpressionToBoolReference(newBlockHandler, entryLine[1], entryLine[2].Value, entryLine[3]);
        entryAddr = newBlockHandler->PManager->Alloc(GotoIfInstruction::GetLength());
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

bool IfBlock::OnExit(BlockHandler* blockHandler, const vector<LexicalResult>& exitingLine, int depthDelta) {
    if (!exitingLine.empty() && depthDelta == 1 && exitingLine[0].Value == "else") {
        if (elsePosition != -1)
            throw invalid_argument("Double else");

        blockHandler->PManager->Alloc(GotoInstruction::GetLength());
        elsePosition = blockHandler->PManager->GetCurrentPosition();
        delete blockHandler->References;
        blockHandler->References = new ReferenceHandler();
        return false;
    }

    if (elsePosition == -1) { // * Just if
        if (constant && !constant_value) { // * if false
            blockHandler->PManager->Overwrite(entryAddr,
                                              GotoInstruction::Build(blockHandler->PManager->GetCurrentPosition()));
        } else {
            if (constant) { // * if true
                // ? Do nothing
            } else { // * Not constant
                blockHandler->PManager->Overwrite(entryAddr,
                                                  GotoIfInstruction::Build(blockHandler->PManager->GetCurrentPosition(),
                                                                           nameAddr, true));
            }
        }
    }
    else { // * If else
        blockHandler->PManager->Overwrite(elsePosition-(GotoInstruction::GetLength()), GotoInstruction::Build(blockHandler->PManager->GetCurrentPosition()));
        if (constant && !constant_value) { // * if false
            blockHandler->PManager->Overwrite(entryAddr,
                                              GotoInstruction::Build(elsePosition));
        } else {
            if (constant) { // * if true
                // ? Do nothing
            } else { // * Not constant
                blockHandler->PManager->Overwrite(entryAddr,
                                                  GotoIfInstruction::Build(elsePosition,
                                                                           nameAddr, true));
            }
        }
    }

    return true;
}