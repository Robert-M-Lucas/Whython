//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_REFERENCE_H
#define PLANG3_REFERENCE_H

#include <string>

using namespace std;

class Reference {
public:
    string Name;
    unsigned short TypeID;
    int Address;
    int Size;

    Reference(string name, unsigned short typeID, int address, int size = 1) {
        Name = name;
        TypeID = typeID;
        Address = address;
        Size = size;
    }
};

#endif //PLANG3_REFERENCE_H
