//
// Created by rober on 29/12/2022.
//

#ifndef PLANG3_CHARTYPE_H
#define PLANG3_CHARTYPE_H


#include "AbstractType.h"
#include "../Compilation/BlockHandler.h"

class CharType: public AbstractType {
public:
    const unsigned short TYPE_ID = 2;

    unsigned short GetID() override { return TYPE_ID; }

    string GetIdentifier() override { return "char"; }

    ADDR Create(BlockHandler* blockHandler, const string& name) override;

    void Assign(BlockHandler* blockHandler, ADDR address, const LexicalResult& literalValue) override;

    static void Assign(BlockHandler* blockHandler, ADDR address, char value);

    static void StaticAssign(BlockHandler* blockHandler, ADDR address, char value);

    void Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, ADDR overwrite_with) override;

    void Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, const LexicalResult& overwrite_with) override;

    int Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, const LexicalResult& literalValue, ADDR out_addr) override;

    int Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, ADDR modify_with, ADDR out_addr) override;
};



#endif //PLANG3_CHARTYPE_H
