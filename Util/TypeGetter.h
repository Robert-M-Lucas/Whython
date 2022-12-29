//
// Created by rober on 28/12/2022.
//

#ifndef PLANG3_TYPEGETTER_H
#define PLANG3_TYPEGETTER_H

#include "../Types/AbstractType.h"
#include "../Types/BoolType.h"
#include "../Types/IntType.h"
#include "../Types/CharType.h"

class TypeGetter {
public:
    AbstractType* types[3] = {
            new IntType(),
            new BoolType(),
            new CharType()
    };

    AbstractType* GetTypeByIdentifier(const string& identifier) {
        for (auto & type : types) {
            if (type->GetIdentifier() == identifier) {
                return type;
            }
        }
        throw invalid_argument("Type does not exist");
    }

    AbstractType* GetTypeByID(unsigned short id) {
        for (auto & type : types) {
            if (type->GetID() == id) {
                return type;
            }
        }
        throw invalid_argument("Type does not exist");
    }
};





#endif //PLANG3_TYPEGETTER_H
