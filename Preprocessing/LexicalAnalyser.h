//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_LEXICALANALYSER_H
#define PLANG3_LEXICALANALYSER_H

#include <vector>
#include <string>

#include "LexicalResult.h"

using namespace std;

class LexicalAnalyser {
    const static string assigners[5];
    const static string blockers[4];
    const static string operators[12];
    const static string keywords[3];
    const static string types[3];
public:
    static LexicalResult ProcessSymbol(string symbolString, int depth);

    static vector<LexicalResult> ProcessLine(string line);
};


#endif //PLANG3_LEXICALANALYSER_H
