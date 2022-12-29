//
// Created by rober on 23/12/2022.
//

#include <stdexcept>

#include "ReferenceHandler.h"

using namespace std;

Reference *ReferenceHandler::GetReference(const string &referenceName) {
    Reference* reference = GetReferenceOrNull(referenceName);
    if (reference == nullptr)
        throw invalid_argument("Name '" + referenceName + "' not found");
    return reference;
}

Reference *ReferenceHandler::GetReferenceOrNull(const string &referenceName) {
    for (auto & reference : references) {
        if (reference->Name == referenceName) {
            return reference;
        }
    }
    return nullptr;
}

void ReferenceHandler::AddReference(const string& name, unsigned short typeID, int address) {
    references.push_back(new Reference(name, typeID, address));
}

/*
int ReferenceHandler::AddUnnamedReference(unsigned short typeID, int address) {
    references.push_back(new Reference("", typeID, address));
    return references.size()-1;
}

Reference *ReferenceHandler::GetReference(int index) {
    return references[index];
}
*/

ReferenceHandler::~ReferenceHandler() {
    for (int i = 0; i < references.size(); i++) {
        delete references[i];
    }
}