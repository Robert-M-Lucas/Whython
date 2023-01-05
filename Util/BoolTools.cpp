//
// Created by rober on 28/12/2022.
//

#include "BoolTools.h"
#include "../Types/BoolType.h"

ADDR ExpressionToBoolReference(BlockHandler *blockHandler, const LexicalResult &lhs, const string &op,
                               const LexicalResult &rhs) {
    Reference* reference = blockHandler->RecursivelyGetReference(lhs.Value);
    ADDR lhs_addr = reference->Address;

    AbstractType* type = TypeGetter().GetTypeByID(reference->TypeID);
    ADDR out_bool = type->Create(blockHandler, "", 1);

    BoolType boolType = BoolType();

    if (rhs.Type == Name) {
        ADDR rhs_addr = blockHandler->RecursivelyGetReference(rhs.Value)->Address;

        int out_type = type->Modify(blockHandler, lhs_addr, op, rhs_addr, out_bool);
        if (out_type != boolType.GetID())
            throw invalid_argument("Expression doesn't evaluate to bool");
    }
    else {
        int out_type = type->Modify(blockHandler, lhs_addr, op, rhs, out_bool);
        if (out_type != boolType.GetID())
            throw invalid_argument("Expression doesn't evaluate to bool");
    }

    return out_bool;
}
