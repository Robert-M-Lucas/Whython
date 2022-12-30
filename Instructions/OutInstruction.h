//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_OUTINSTRUCTION_H
#define PLANG3_OUTINSTRUCTION_H

#include <vector>

#include "../Util/Common.h"
#include "../Util/Conversions.h"
#include "../InternalTypes/AddressType.h"

using namespace std;

class OutInstruction {
public:
    static vector<unsigned char> Build(ADDR address, unsigned short type, bool new_line = false) {
        BYTE CODE = 1;
        if (new_line)
            CODE += 9;

        vector<unsigned char> data;
        data.push_back(CODE);
        vector<unsigned char> v_address = AddressInternalType::Build(address);
        data.insert(data.end(), v_address.begin(), v_address.end());
        v_address = shortToBytes(type);
        data.insert(data.end(), v_address.begin(), v_address.end());
        return data;
    }
};

#endif //PLANG3_OUTINSTRUCTION_H
