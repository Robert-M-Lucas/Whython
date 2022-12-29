//
// Created by rober on 29/12/2022.
//

#include "CharType.h"

#include "../Util/Common.h"
#include "../Util/Conversions.h"
#include "../Instructions/WriteInstruction.h"
#include "../Instructions/CopyInstruction.h"
#include "../Instructions/AddInstruction.h"
#include "../Instructions/EqualsInstruction.h"
#include "BoolType.h"

extern bool DEBUG;

using namespace std;

ADDR CharType::Create(BlockHandler* blockHandler, const string& name) {
    ADDR address = blockHandler->VManager->Alloc(1);

    if (DEBUG)
        cout << "Allocating char [" << name << ":" << address << "]" << endl;

    if (!name.empty()) {
        if (blockHandler->RecursivelyGetReferenceOrNull(name) != nullptr)
            throw invalid_argument("Name '" + name + "' already exists");
        blockHandler->References->AddReference(name, CharType::TYPE_ID, address);
    }
    return address;
}

void CharType::Assign(BlockHandler* blockHandler, ADDR address, char value) {
    if (DEBUG)
        cout << "Creating char assign instruction [" << value << ";" << address << "]" << endl;

    blockHandler->PManager->Append(WriteInstruction::Build(address, value));
}

void CharType::StaticAssign(BlockHandler* blockHandler, ADDR address, char value) {
    if (DEBUG)
        cout << "Creating static char assign instruction [" << value << ";" << address << "]" << endl;

    blockHandler->VManager->Overwrite(address, reinterpret_cast<const unsigned char *>(&value), 1);
}

void CharType::Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, const LexicalResult& overwrite_with) {
    if (DEBUG)
        cout << "Creating char overwrite instruction [" << to_overwrite << ";" << overwrite_with.Value << "]" << endl;

    ADDR addr = CharType::Create(blockHandler, "");
    if (overwrite_with.Type != StringLiteral || overwrite_with.Value.size() != 1) {
        throw invalid_argument("Only StringLiterals of size 1 supported for overwriting");
    }

    CharType::StaticAssign(blockHandler, addr, overwrite_with.Value[0]);

    CharType::Overwrite(blockHandler, to_overwrite, addr);
}

void CharType::Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, ADDR overwrite_with) {
    if (DEBUG)
        cout << "Creating char overwrite instruction [" << to_overwrite << ";" << overwrite_with << "]" << endl;
    blockHandler->PManager->Append(CopyInstruction::Build(overwrite_with, to_overwrite, 1));
}

int CharType::Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, const LexicalResult& literalValue, ADDR out_addr) {
    ADDR addr = CharType::Create(blockHandler, "");
    if (literalValue.Type != StringLiteral || literalValue.Value.size() != 1) {
        throw invalid_argument("Only StringLiterals of size 1 supported for overwriting");
    }

    CharType::StaticAssign(blockHandler, addr, literalValue.Value[0]);
    return CharType::Modify(blockHandler, to_modify, op, addr, out_addr);
}

int CharType::Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, ADDR modify_with, ADDR out_addr) {
    if (op == "==") {
        blockHandler->PManager->Append(EqualsInstruction::Build(to_modify, modify_with, out_addr, 1));
        return BoolType().GetID();
    }
    if (op == "!=") {
        blockHandler->PManager->Append(EqualsInstruction::Build(to_modify, modify_with, out_addr, 1, true));
        return BoolType().GetID();
    }
    else
        throw invalid_argument("Operator '" + op + "' not supported");
}