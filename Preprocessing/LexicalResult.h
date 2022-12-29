//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_LEXICALRESULT_H
#define PLANG3_LEXICALRESULT_H

#include <string>

using namespace std;

enum LexicalResultType { Blocker, Assigner, Name, Keyword, StringLiteral, IntLiteral, BoolLiteral, Type, Operator, Empty };

class LexicalResult {
public:
    LexicalResultType Type;
    string Value;
    int Depth;

    LexicalResult(LexicalResultType type, int depth, string value = "") {
        Type = type;
        Value = value;
        Depth = depth;
    }
};

#endif //PLANG3_LEXICALRESULT_H
