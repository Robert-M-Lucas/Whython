//
// Created by rober on 28/12/2022.
//

#ifndef PLANG3_BOOLTOOLS_H
#define PLANG3_BOOLTOOLS_H

#include "Common.h"
#include "../Compilation/BlockHandler.h"
#include "../Types/AbstractType.h"
#include "TypeGetter.h"

ADDR ExpressionToBoolReference(BlockHandler *blockHandler, const LexicalResult &lhs, const string &op,
                               const LexicalResult &rhs);

#endif //PLANG3_BOOLTOOLS_H
