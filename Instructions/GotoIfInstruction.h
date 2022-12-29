//
// Created by rober on 27/12/2022.
//

#ifndef PLANG3_GOTOIFINSTRUCTION_H
#define PLANG3_GOTOIFINSTRUCTION_H

#include <vector>

#include "../Util/Common.h"
#include "../InternalTypes/AddressType.h"

using namespace std;

// ! If and NotIf (codes 7 and 8 respectively)

class GotoIfInstruction {
public:
    static int GetLength() { return 5; }

    static vector<BYTE> Build(ADDR goto_address, ADDR bool_address, bool if_not = false) {
        BYTE CODE = 7;
        if (if_not) CODE++;

        vector<BYTE> data;
        data.push_back(CODE);
        vector<BYTE> v_address = AddressInternalType::Build(goto_address);
        data.insert(data.end(), v_address.begin(), v_address.end());
        v_address = AddressInternalType::Build(bool_address);
        data.insert(data.end(), v_address.begin(), v_address.end());
        return data;
    }
};

#endif //PLANG3_GOTOIFINSTRUCTION_H
