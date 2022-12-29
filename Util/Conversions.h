//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_CONVERSIONS_H
#define PLANG3_CONVERSIONS_H

#include <vector>

#include "Common.h"

using namespace std;

vector<unsigned char> shortToBytes(unsigned short paramShort);

unsigned short bytesToShort(const unsigned char* bytes);

vector<BYTE> intToBytes(int paramInt);

int bytesToInt(BYTE* bytes);

#endif //PLANG3_CONVERSIONS_H
