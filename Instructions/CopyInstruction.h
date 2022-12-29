//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_COPYINSTRUCTION_H
#define PLANG3_COPYINSTRUCTION_H

#include <vector>

#include "../Util/Common.h"
#include "../Util/Conversions.h"
#include "../InternalTypes/AddressType.h"

using namespace std;

class CopyInstruction {
public:
    static vector<BYTE> Build(ADDR address_from, ADDR address_to, unsigned short amount) {
        const BYTE CODE = 3;

        vector<BYTE> data;
        data.push_back(CODE);

        vector<BYTE> temp_bytes = AddressInternalType::Build(address_from);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        temp_bytes = AddressInternalType::Build(address_to);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        temp_bytes = shortToBytes(amount);
        data.insert(data.end(), temp_bytes.begin(), temp_bytes.end());
        return data;
    }
};

#endif //PLANG3_COPYINSTRUCTION_H
