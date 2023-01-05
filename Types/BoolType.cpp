//
// Created by rober on 27/12/2022.
//

#include <string>

#include "BoolType.h"

#include "../Util/Common.h"
#include "../Util/Conversions.h"
#include "../Instructions/WriteInstruction.h"
#include "../Instructions/CopyInstruction.h"
#include "../Instructions/AddInstruction.h"
#include "../Instructions/EqualsInstruction.h"
#include "../Instructions/AndInstruction.h"

extern bool DEBUG;

using namespace std;

ADDR BoolType::Create(BlockHandler *blockHandler, const string &name, int arrSize) {
    ADDR address = blockHandler->VManager->Alloc(1);

    if (DEBUG)
        cout << "Allocating bool [" << name << ":" << address << "]" << endl;

    if (!name.empty()) {
        if (blockHandler->RecursivelyGetReferenceOrNull(name) != nullptr)
            throw invalid_argument("Name '" + name + "' already exists");
        blockHandler->References->AddReference(name, BoolType::TYPE_ID, address, arrSize);
    }
    return address;
}

void BoolType::Assign(BlockHandler* blockHandler, ADDR address, bool value) {
    if (DEBUG)
        cout << "Creating bool assign instruction [" << value << ";" << address << "]" << endl;

    BYTE b_value;
    if (value)
        b_value = BOOL_TRUE;
    else
        b_value = BOOL_FALSE;

    blockHandler->PManager->Append(WriteInstruction::Build(address, b_value));
}

void BoolType::StaticAssign(BlockHandler* blockHandler, ADDR address, bool value) {
    if (DEBUG)
        cout << "Creating static bool assign instruction [" << value << ";" << address << "]" << endl;

    BYTE b_value;
    if (value)
        b_value = BOOL_TRUE;
    else
        b_value = BOOL_FALSE;

    blockHandler->VManager->Overwrite(address, &b_value, 1);
}

void BoolType::Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, const LexicalResult& overwrite_with) {
    if (DEBUG)
        cout << "Creating bool overwrite instruction [" << to_overwrite << ";" << overwrite_with.Value << "]" << endl;

    ADDR addr = BoolType::Create(blockHandler, "", 1);
    if (overwrite_with.Type != IntLiteral && overwrite_with.Type != BoolLiteral) {
        throw invalid_argument("Only IntLiterals and BoolLiterals are supported for overwriting");
    }
    if (overwrite_with.Type == IntLiteral)
        BoolType::StaticAssign(blockHandler, addr, stois(overwrite_with.Value) != 0);
    else
        BoolType::StaticAssign(blockHandler, addr, overwrite_with.Value == "true");
    BoolType::Overwrite(blockHandler, to_overwrite, addr);
}

void BoolType::Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, ADDR overwrite_with) {
    if (DEBUG)
        cout << "Creating bool overwrite instruction [" << to_overwrite << ";" << overwrite_with << "]" << endl;
    blockHandler->PManager->Append(CopyInstruction::Build(overwrite_with, to_overwrite, 1));
}

int BoolType::Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, const LexicalResult& literalValue, ADDR out_addr) {
    ADDR addr = BoolType::Create(blockHandler, "", 1);
    if (literalValue.Type != BoolLiteral && literalValue.Type != IntLiteral) { throw invalid_argument("Only Int and Bool Literals supported"); }
    if (literalValue.Type == IntLiteral)
        BoolType::StaticAssign(blockHandler, addr, stois(literalValue.Value) != 0);
    else
        BoolType::StaticAssign(blockHandler, addr, literalValue.Value == "true");
    return BoolType::Modify(blockHandler, to_modify, op, addr, out_addr);
}

int BoolType::Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, ADDR modify_with, ADDR out_addr) {
    if (op == "==") {
        blockHandler->PManager->Append(EqualsInstruction::Build(to_modify, modify_with, out_addr, 1));
        return GetID();
    }
    else if (op == "!=") {
        blockHandler->PManager->Append(EqualsInstruction::Build(to_modify, modify_with, out_addr, 1, true));
        return GetID();
    }
    else if (op == "&") {
        blockHandler->PManager->Append(AndInstruction::Build(to_modify, modify_with, out_addr));
        return GetID();
    }
    else
        throw invalid_argument("Operator '" + op + "' not supported");
}