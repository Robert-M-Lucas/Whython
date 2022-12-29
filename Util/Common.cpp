//
// Created by rober on 25/12/2022.
//

#include "Common.h"

int stois(const string &str) {
    try {
        return stoi(str);
    }
    catch (const exception& e) {
        throw invalid_argument("Integer conversion (" + str + ") failed");
    }
}
