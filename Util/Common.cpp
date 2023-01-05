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

ArrData GetArrData(string arrMember) {
    int pos1 = arrMember.find('[');
    if (pos1 != string::npos) {
        int pos2 = arrMember.find(']');
        if (pos2 != string::npos) {
            if (pos2 == pos1 + 1 && pos2 == arrMember.size() - 1) // * arrName[]
                throw invalid_argument("ArrName, not Member");
            else if (pos2 > pos1)
                return ArrData(arrMember.substr(0, arrMember.find('[')), stois(arrMember.substr(pos1+1, (pos2 - pos1) - 1)));
            else
                throw invalid_argument("']' found before '['");
        }
        else
            throw invalid_argument("']' not found");
    }
    else
        throw invalid_argument("'[' not found");
}
