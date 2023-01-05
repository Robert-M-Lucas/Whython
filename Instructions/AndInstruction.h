//
// Created by rober on 31/12/2022.
//

#ifndef PLANG3_ANDINSTRUCTION_H
#define PLANG3_ANDINSTRUCTION_H

#include <vector>

#include "../Util/Common.h"
#include "../Util/Conversions.h"
#include "../InternalTypes/AddressType.h"

using namespace std;

class AndInstruction {
public:
    static vector<BYTE> Build(ADDR address_from, ADDR address_to, ADDR address_out) {
        const BYTE CODE = 11;

        vector<BYTE> data;
        data.push_back(CODE);

        vector<BYTE> temp_bytes = AddressInternalType::Build(address_from);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        temp_bytes = AddressInternalType::Build(address_to);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        temp_bytes = shortToBytes(address_out);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        return data;
    }
};

#endif //PLANG3_ANDINSTRUCTION_H
