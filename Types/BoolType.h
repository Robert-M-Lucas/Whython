//
// Created by rober on 27/12/2022.
//

#ifndef PLANG3_BOOLTYPE_H
#define PLANG3_BOOLTYPE_H

#include "AbstractType.h"
#include "../Compilation/BlockHandler.h"

class BoolType: public AbstractType {
public:
    const unsigned short TYPE_ID = 1;

    unsigned short GetID() override { return TYPE_ID; }

    string GetIdentifier() override { return "bool"; }

    ADDR Create(BlockHandler* blockHandler, const string& name) override;

    void Assign(BlockHandler* blockHandler, ADDR address, const LexicalResult& literalValue) override {
        if (literalValue.Type != IntLiteral && literalValue.Type != BoolLiteral) { throw invalid_argument("Only bool and int literals supported for assignment"); }
        if (literalValue.Type == IntLiteral)
            Assign(blockHandler, address, stois(literalValue.Value) != 0);
        else
            Assign(blockHandler, address, literalValue.Value == "true");
    }

    static void Assign(BlockHandler* blockHandler, ADDR address, bool value);

    static void StaticAssign(BlockHandler* blockHandler, ADDR address, bool value);

    void Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, ADDR overwrite_with) override;

    void Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, const LexicalResult& overwrite_with) override;

    static ADDR ExpressionToBoolReference(BlockHandler* blockHandler, const LexicalResult& lhs, const string& op, const LexicalResult& rhs);

    int Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, const LexicalResult& literalValue, ADDR out_addr) override;

    int Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, ADDR modify_with, ADDR out_addr) override;
};



#endif //PLANG3_BOOLTYPE_H
