//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_REFERENCEHANDLER_H
#define PLANG3_REFERENCEHANDLER_H


#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>

#include "Reference.h"

using namespace std;

class ReferenceHandler {
private:
    vector<Reference*> references;
public:
    void AddReference(const string& name, unsigned short typeID, int address);
    // int AddUnnamedReference(unsigned short typeID, int address);

    Reference* GetReference(const string& referenceName);
    // Reference* GetReference(int index);

    Reference* GetReferenceOrNull(const string& referenceName);

    ~ReferenceHandler();
};


#endif //PLANG3_REFERENCEHANDLER_H
