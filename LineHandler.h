//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_LINEHANDLER_H
#define PLANG3_LINEHANDLER_H

#include <vector>

#include "Preprocessing/LexicalResult.h"
#include "BlockHandler.h"

using namespace std;

void HandleLine(vector<LexicalResult> parsedLine, BlockHandler* blockHandler);

#endif //PLANG3_LINEHANDLER_H
