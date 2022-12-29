//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_WRITEINSTRUCTION_H
#define PLANG3_WRITEINSTRUCTION_H

#include <vector>

#include "../Util/Common.h"
#include "../InternalTypes/AddressType.h"

using namespace std;

class WriteInstruction {
public:
    static vector<BYTE> Build(ADDR address, BYTE value) {
        const BYTE CODE = 0;

        vector<BYTE> data;
        data.push_back(CODE);
        vector<BYTE> v_address = AddressInternalType::Build(address);
        data.insert(data.end(), v_address.begin(), v_address.end());
        data.push_back(value);
        return data;
    }
};

#endif //PLANG3_WRITEINSTRUCTION_H
