//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_ADDRESSTYPE_H
#define PLANG3_ADDRESSTYPE_H

#include "../Util/Common.h"
#include <vector>

using namespace std;

class AddressInternalType {
public:
    static const int LENGTH;

    static vector<BYTE> Build(ADDR address) {
        vector<BYTE> data(2);

        data[0] = address >> 8;
        data[1] = address & BOOL_TRUE;
        return data;
    }
};

#endif //PLANG3_ADDRESSTYPE_H
