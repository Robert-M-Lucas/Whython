//
// Created by rober on 27/12/2022.
//

#ifndef PLANG3_EQUALSINSTRUCTION_H
#define PLANG3_EQUALSINSTRUCTION_H

#include <vector>

#include "../Util/Common.h"
#include "../Util/Conversions.h"
#include "../InternalTypes/AddressType.h"

using namespace std;

// ! From and to can be any *size*, out is 1 byte BOOL_TRUE or BOOL_FALSE
// ! Equals and NotEquals (codes 5 and 6 respectively)

class EqualsInstruction {
public:
    static vector<BYTE> Build(ADDR address_from, ADDR address_to, ADDR address_out, unsigned char size, bool not_equals = false) {
        BYTE CODE = 5;
        if (not_equals) CODE++;

        vector<BYTE> data;
        data.push_back(CODE);

        vector<BYTE> temp_bytes = AddressInternalType::Build(address_from);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        temp_bytes = AddressInternalType::Build(address_to);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        temp_bytes = shortToBytes(address_out);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        data.push_back(size);

        return data;
    }
};

#endif //PLANG3_EQUALSINSTRUCTION_H
