//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_ABSTRACTTYPE_H
#define PLANG3_ABSTRACTTYPE_H

#include <string>
#include <climits>

#include "../BlockHandler.h"

using namespace std;

class AbstractType {
public:
    virtual unsigned short GetID() { return USHRT_MAX; }

    virtual string GetIdentifier() { return ""; }

    virtual ADDR Create(BlockHandler* blockHandler, const string& name) { return 0; }

    virtual void Assign(BlockHandler* blockHandler, ADDR address, const LexicalResult& literalValue) {}

    virtual void Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, ADDR overwrite_with) {}
    virtual void Overwrite(BlockHandler* blockHandler, ADDR to_overwrite, const LexicalResult& overwrite_with) {}

    virtual int Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, const LexicalResult& literalValue, ADDR out_addr) { return 0; }
    virtual int Modify(BlockHandler* blockHandler, ADDR to_modify, const string& op, ADDR modify_with, ADDR out_addr) { return 0; }
};

#endif //PLANG3_ABSTRACTTYPE_H
