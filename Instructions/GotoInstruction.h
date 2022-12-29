//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_GOTOINSTRUCTION_H
#define PLANG3_GOTOINSTRUCTION_H

#include <vector>

#include "../Util/Common.h"
#include "../InternalTypes/AddressType.h"

using namespace std;

class GotoInstruction {
public:
    static int GetLength() { return 3; }

    static vector<BYTE> Build(ADDR address) {
        const BYTE CODE = 2;

        vector<BYTE> data;
        data.push_back(CODE);
        vector<BYTE> v_address = AddressInternalType::Build(address);
        data.insert(data.end(), v_address.begin(), v_address.end());
        return data;
    }
};

#endif //PLANG3_GOTOINSTRUCTION_H
