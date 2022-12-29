//
// Created by rober on 23/12/2022.
//

#include <string>

#include "IntType.h"

#include "BoolType.h"
#include "../Util/Common.h"
#include "../Util/Conversions.h"
#include "../Instructions/WriteInstruction.h"
#include "../Instructions/CopyInstruction.h"
#include "../Instructions/AddInstruction.h"
#include "../Instructions/EqualsInstruction.h"

extern bool DEBUG;

using namespace std;

ADDR IntType::Create(BlockHandler* blockHandler, const string& name) {
    ADDR address = blockHandler->VManager->Alloc(4);

    if (DEBUG)
        cout << "Allocating int [" << name << ":" << address << "]" << endl;

    if (!name.empty()) {
        if (blockHandler->RecursivelyGetReferenceOrNull(name) != nullptr)
            throw invalid_argument("Name '" + name + "' already exists");
        blockHandler->References->AddReference(name, IntType::TYPE_ID, address);
    }
    return address;
}

void IntType::Assign(BlockHandler* blockHandler, ADDR address, int value) {
    if (DEBUG)
        cout << "Creating int assign instruction [" << value << ";" << address << "]" << endl;

    vector<BYTE> value_bytes = intToBytes(value);
    for (int i = 0; i < 4; i++) {
        blockHandler->PManager->Append(WriteInstruction::Build(address + i, value_bytes[i]));
    }
}

void IntType::StaticAssign(BlockHandler* blockHandler, ADDR address, int value) {
    if (DEBUG)
        cout << "Creating static int assign instruction [" << value << ";" << address << "]" << endl;

    vector<BYTE> value_bytes = intToBytes(value);
    blockHandler->VManager->Overwrite(address, value_bytes);
}

void IntType::Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, const LexicalResult& overwrite_with) {
    if (DEBUG)
        cout << "Creating int overwrite instruction [" << to_overwrite << ";" << overwrite_with.Value << "]" << endl;

    ADDR addr = IntType::Create(blockHandler, "");
    if (overwrite_with.Type != IntLiteral) {
        throw invalid_argument("Only IntLiterals are supported for overwriting");
    }
    IntType::StaticAssign(blockHandler, addr, stois(overwrite_with.Value));
    IntType::Overwrite(blockHandler, to_overwrite, addr);
}

void IntType::Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, ADDR overwrite_with) {
    if (DEBUG)
        cout << "Creating int overwrite instruction [" << to_overwrite << ";" << overwrite_with << "]" << endl;
    blockHandler->PManager->Append(CopyInstruction::Build(overwrite_with, to_overwrite, 4));
}

int IntType::Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, const LexicalResult& literalValue, ADDR out_addr) {
    ADDR addr = IntType::Create(blockHandler, "");
    if (literalValue.Type != IntLiteral) { throw invalid_argument("Only IntLiterals supported"); }
    IntType::StaticAssign(blockHandler, addr, stois(literalValue.Value));
    return IntType::Modify(blockHandler, to_modify, op, addr, out_addr);
}

int IntType::Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, ADDR modify_with, ADDR out_addr) {
    if (op == "+") {
        blockHandler->PManager->Append(AddInstruction::Build(to_modify, modify_with, out_addr, 4));
        return GetID();
    }
    else if (op == "==") {
        blockHandler->PManager->Append(EqualsInstruction::Build(to_modify, modify_with, out_addr, 4));
        return BoolType().TYPE_ID;
    }
    else if (op == "!=") {
        blockHandler->PManager->Append(EqualsInstruction::Build(to_modify, modify_with, out_addr, 4, true));
        return BoolType().TYPE_ID;
    }
    else {
        throw invalid_argument("Operator '" + op + "' not supported");
    }
}