//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_INTTYPE_H
#define PLANG3_INTTYPE_H

#include <string>

#include "AbstractType.h"

// 32-bit signed
class IntType: public AbstractType {
public:
    const unsigned short TYPE_ID = 0;

    unsigned short GetID() override { return TYPE_ID; }
    unsigned short GetSize() override { return 4; }

    string GetIdentifier() override { return "int"; }

    ADDR Create(BlockHandler *blockHandler, const string &name, int arrSize) override;

    void Assign(BlockHandler* blockHandler, ADDR address, const LexicalResult& literalValue) override {
        if (literalValue.Type != IntLiteral) { throw invalid_argument("Only int literals supported for assignment"); }
        Assign(blockHandler, address, stois(literalValue.Value));
    }

    static void Assign(BlockHandler* blockHandler, ADDR address, int value);

    static void StaticAssign(BlockHandler* blockHandler, ADDR address, int value);

    void Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, ADDR overwrite_with) override;

    void Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, const LexicalResult& overwrite_with) override;

    int Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, const LexicalResult& literalValue, ADDR out_addr) override;

    int Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, ADDR modify_with, ADDR out_addr) override;
};


#endif //PLANG3_INTTYPE_H
