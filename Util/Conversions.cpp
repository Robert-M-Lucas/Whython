//
// Created by rober on 23/12/2022.
//

#include <vector>

#include "Conversions.h"
#include "Common.h"

using namespace std;

// ! BYTE 0 LARGEST SIGNIFICANCE

vector<unsigned char> shortToBytes(unsigned short paramShort) {
    vector<unsigned char> arrayOfByte(2);
    for (int i = 0; i < 2; i++)
        arrayOfByte[1 - i] = (paramShort >> (i * 8));
    return arrayOfByte;
}

unsigned short bytesToShort(const unsigned char *bytes) {
    unsigned short value = 0;
    for (int i = 0; i < 2; i++) {
        value += bytes[1 - i] << (i * 8);
    }
    return value;
}

vector<BYTE> intToBytes(int paramInt) {
    vector<BYTE> arrayOfByte(4);
    for (int i = 0; i < 4; i++)
        arrayOfByte[3 - i] = (paramInt >> (i * 8));
    return arrayOfByte;
}

int bytesToInt(unsigned char *bytes) {
    int value = 0;
    for (int i = 0; i < 4; i++) {
        value += bytes[3 - i] << (i * 8);
    }
    return value;
}
