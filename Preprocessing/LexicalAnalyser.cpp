//
// Created by rober on 23/12/2022.
//

#include <iostream>

#include "LexicalAnalyser.h"
#include "LexicalResult.h"

using namespace std;

const char digits[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

const string LexicalAnalyser::assigners[] = { "=", "+=", "-=", "*=", "/=" };
const string LexicalAnalyser::blockers[] = { "while", "loop", "if", "else" };
const string LexicalAnalyser::operators[] = { "+", "-", "*", "/", ">", "<", ">=", "<=", "==", "!=", "|", "&" };
const string LexicalAnalyser::keywords[] = { "out" };
const string LexicalAnalyser::types[] = { "int", "bool", "char" };

LexicalResult LexicalAnalyser::ProcessSymbol(string symbolString, int depth) {
    if (symbolString.empty()) return {Empty, depth};

    if (symbolString == "true" || symbolString == "false") return {BoolLiteral, depth, symbolString};

    for (const auto & assigner : assigners) {
        if (assigner == symbolString) {
            return {Assigner, depth, symbolString};
        }
    }

    for (const auto & blocker : blockers) {
        if (blocker == symbolString) {
            return {Blocker, depth, symbolString};
        }
    }

    for (const auto & type : types) {
        if (type == symbolString) {
            return {Type, depth, symbolString};
        }
    }

    for (const auto & keyword : keywords) {
        if (keyword == symbolString) {
            return {Keyword, depth, symbolString};
        }
    }

    if (symbolString.size() >= 2 && symbolString[0] == '\'' && symbolString[symbolString.size() - 1] == '\'')
        return {StringLiteral, depth, symbolString};

    bool number = true;
    for (int i = 0; i < symbolString.size(); i++) {
        if (i == 0 && symbolString.size() > 1 && symbolString[i] == '-')
            continue;

        bool found = false;
        for (char digit: digits) {
            if (symbolString[i] == digit) {
                found = true;
                break;
            }
        }
        if (!found) {
            number = false;
            break;
        }
    }

    if (number) {
        return {IntLiteral, depth, symbolString};
    }

    for (const auto & i : operators) {
        if (symbolString == i) {
            return {Operator, depth, symbolString};
        }
    }

    return {Name, depth, symbolString};
}

vector<LexicalResult> LexicalAnalyser::ProcessLine(string line) {
    vector<LexicalResult> result;

    int depth = 0;
    if (line.size() > 0 && line[0] == '\t') {
        while (depth < line.size() && line[depth] == '\t') { depth++; }
    }
    else if (line.size() >= 4) {
        while ((depth * 4) <= line.size() - 4 && line.substr(depth*4, 4) == "    ") { depth++; }
    }

    string buffer;
    for (int i = depth; i < line.size(); i++) {
        // * String handling
        if (line[i] == '"') {
            if (!buffer.empty())
                throw invalid_argument("String formatting exception");

            i++;
            bool closed = false;
            while (i < line.size()) {
                if (line[i] == '"') {
                    if (buffer.empty())
                        throw invalid_argument("Strings can't be empty");
                    closed = true;
                    break;
                }
                buffer += line[i];
                i++;
            }

            if (!closed)
                throw invalid_argument("String not closed");

            result.emplace_back(StringLiteral, depth, buffer);
            buffer = "";
            continue;
        }

        bool comment = (line[i] == '/' && i+1 < line.size() && line[i] == '/');

        if (line[i] == ' ' || comment) {
            LexicalResult symbol = ProcessSymbol(buffer, depth);
            if (symbol.Type != Empty) {
                result.push_back(symbol);
            }
            buffer = "";
            if (comment) break;
            continue;
        }

        buffer += line[i];
    }

    LexicalResult symbol = ProcessSymbol(buffer, depth);
    if (symbol.Type != Empty) {
        result.push_back(symbol);
    }

    return result;
}
