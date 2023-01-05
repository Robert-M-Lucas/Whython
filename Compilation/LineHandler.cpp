//
// Created by rober on 23/12/2022.
//

#include "LineHandler.h"
#include "../Types/AbstractType.h"
#include "../Types/IntType.h"
#include "../Types/BoolType.h"
#include "../Instructions/OutInstruction.h"
#include "../Util/TypeGetter.h"

// template <typename T, int N> char(&dim_helper(T(&)[N]))[N];
// #define dim(x) (sizeof(dim_helper(x)))

void HandleLine(vector<LexicalResult> parsedLine, BlockHandler* blockHandler) {
    for (auto & i : parsedLine) {
        if (i.Type == Name) {
            Reference* ref = blockHandler->RecursivelyGetReferenceOrNull(i.Value);
            if (ref == nullptr) continue;
            if (ref->Size != 1)
                throw invalid_argument("Array cannot be treated as Name");
        }
        else if (i.Type == ArrMember) {
            ArrData arrData = GetArrData(i.Value);
            Reference* ref = blockHandler->RecursivelyGetReferenceOrNull(arrData.Name);
            if (ref == nullptr) continue;
            if (ref->Size == 1)
                throw invalid_argument("Name cannot be treated as Array");
            else if (ref->Size <= arrData.Position)
                throw invalid_argument("Index out of array range");
        }
    }

    // * Start with type e.g. int a = 2
    if (parsedLine.at(0).Type == Type) {
        // * int a = 1
        if (parsedLine.size() == 4) {
            if (parsedLine[1].Type == Name) { // * int a = 1
                if (parsedLine[2].Type != Assigner) {
                    throw invalid_argument("A type must be followed by a name, assigner or literal");
                }

                if (parsedLine[2].Value == "=") {
                    AbstractType *type = TypeGetter().GetTypeByIdentifier(parsedLine[0].Value);

                    ADDR addr = type->Create(blockHandler, parsedLine[1].Value, 1);
                    type->Assign(blockHandler, addr, parsedLine[3]);
                } else
                    throw invalid_argument("Initialisation must be done using an '='");
            }
            else if (parsedLine[1].Type == ArrName) { // * int a[] = 1
                if (parsedLine[2].Value != "=")
                    throw invalid_argument("Initialisation must be done using an '='");
                if (parsedLine[3].Type == IntLiteral) {

                    int arr_size = stois(parsedLine[3].Value);
                    if (arr_size <= 1)
                        throw invalid_argument("Arrays must have a size greater than 1");

                    AbstractType *type = TypeGetter().GetTypeByIdentifier(parsedLine[0].Value);
                    // ? Needed to create reference
                    ADDR arr_addr = type->Create(blockHandler, parsedLine[1].Value, arr_size);

                    for (int i = 1; i < arr_size; i++) {
                        type->Create(blockHandler, "", arr_size);
                    }
                }
                else if (parsedLine[3].Type == StringLiteral) {
                    if (parsedLine[0].Value != "char")
                        throw invalid_argument("A StringLiteral can only be used to initialise a char array");

                    AbstractType *type = TypeGetter().GetTypeByIdentifier(parsedLine[0].Value);
                    ADDR arr_addr = type->Create(blockHandler, parsedLine[1].Value, parsedLine[3].Value.size());
                    type->Assign(blockHandler, arr_addr, {StringLiteral, parsedLine[3].Depth, parsedLine[3].Value.substr(0, 1)});

                    for (int i = 1; i < parsedLine[3].Value.size(); i++) {
                        ADDR el_addr = type->Create(blockHandler, "", parsedLine[3].Value.size());
                        type->Assign(blockHandler, el_addr, {StringLiteral ,parsedLine[3].Depth, parsedLine[3].Value.substr(i, 1)});
                    }
                }
                else
                    throw invalid_argument("Arrays must be initialised with an IntLiteral value denoting its size. A StringLiteral can be used for char arrays");
            }
            else {
                throw invalid_argument("A type must be followed by a name");
            }
        }
        // * int a = b + 1
        else if (parsedLine.size() == 6) {
            if (parsedLine[1].Type != Name || parsedLine[2].Type != Assigner || parsedLine[2].Value != "="
                    || parsedLine[3].Type != Name || parsedLine[4].Type != Operator) {
                throw invalid_argument("A type must be followed by a name, assigner or literal");
            }

            AbstractType* type = TypeGetter().GetTypeByIdentifier(parsedLine[0].Value);
            ADDR addr = type->Create(blockHandler, parsedLine[1].Value, 1);
            Reference* lhs = blockHandler->RecursivelyGetReference(parsedLine[3].Value);
            AbstractType* type2 = TypeGetter().GetTypeByID(lhs->TypeID);

            if (parsedLine[5].Type == Name) {
                Reference* rhs = blockHandler->RecursivelyGetReference(parsedLine[5].Value);
                if (lhs->TypeID != rhs->TypeID)
                    throw invalid_argument("Type mismatch");
                int return_type = type2->Modify(blockHandler, lhs->Address, parsedLine[4].Value, rhs->Address, addr);
                if (return_type != type->GetID())
                    throw invalid_argument("Type mismatch");
            }
            else {
                int return_type = type->Modify(blockHandler, lhs->Address, parsedLine[4].Value, parsedLine[5], addr);
                if (return_type != type->GetID())
                    throw invalid_argument("Type mismatch");
            }
        }
        else {
            throw invalid_argument("A type must be followed by a name, assigner or literal");
        }
    }
    // * Start with name e.g. a += 2
    else if (parsedLine.at(0).Type == Name || parsedLine.at(0).Type == ArrMember) {
        if (parsedLine.size() < 3 || parsedLine[1].Type != Assigner)
            throw invalid_argument("A name must be followed by an assigner and a value");

        if (parsedLine.size() == 5) { // * a = b + 1
            if (parsedLine[1].Value != "=" || parsedLine[3].Type != Operator)
                throw invalid_argument("Invalid syntax");
            if (parsedLine[2].Type != Name && parsedLine[2].Type != ArrMember)
                throw invalid_argument("LHS must be Name or ArrMember");


            string r1_name = parsedLine[0].Value;
            int r1_offset = 0;
            if (parsedLine[0].Type == ArrMember) {
                ArrData arrData = GetArrData(parsedLine[0].Value);
                r1_name = arrData.Name;
                r1_offset = arrData.Position;
            }
            string r2_name = parsedLine[2].Value;
            int r2_offset = 0;
            if (parsedLine[2].Type == ArrMember) {
                ArrData arrData = GetArrData(parsedLine[2].Value);
                r2_name = arrData.Name;
                r2_offset = arrData.Position;
            }

            Reference* r1 = blockHandler->RecursivelyGetReference(r1_name);
            Reference* r2 = blockHandler->RecursivelyGetReference(r2_name);
            AbstractType* type1 = TypeGetter().GetTypeByID(r1->TypeID);
            AbstractType* type2 = TypeGetter().GetTypeByID(r2->TypeID);

            if (parsedLine[4].Type == Name || parsedLine[4].Type == ArrMember) {
                string r3_name = parsedLine[4].Value;
                int r3_offset = 0;
                if (parsedLine[4].Type == ArrMember) {
                    ArrData arrData = GetArrData(parsedLine[4].Value);
                    r3_name = arrData.Name;
                    r3_offset = arrData.Position;
                }
                Reference* r3 = blockHandler->RecursivelyGetReference(r3_name);
                AbstractType* type3 = TypeGetter().GetTypeByID(r3->TypeID);
                if (r2->TypeID != r3->TypeID)
                    throw invalid_argument("Type mismatch");
                int return_type = type2->Modify(blockHandler, r2->Address + (r2_offset * type2->GetSize()), parsedLine[3].Value,
                                               r3->Address + (r3_offset * type3->GetSize()), r1->Address + (r1_offset * type1->GetSize()));
                if (return_type != r1->TypeID)
                    throw invalid_argument("Type mismatch");
            }
            else {
                int return_type = type1->Modify(blockHandler, r2->Address + (r2_offset * type2->GetSize()),
                                               parsedLine[3].Value, parsedLine[4], r1->Address + (r1_offset * type1->GetSize()));
                if (r1->TypeID != return_type)
                    throw invalid_argument("Type mismatch");
            }
        }
        else if (parsedLine[1].Value == "=") { // * a = 1
            if (parsedLine[2].Type == Name || parsedLine[2].Type == ArrMember) {
                string r1_name = parsedLine[0].Value;
                int r1_offset = 0;
                if (parsedLine[0].Type == ArrMember) {
                    ArrData arrData = GetArrData(parsedLine[0].Value);
                    r1_name = arrData.Name;
                    r1_offset = arrData.Position;
                }
                string r2_name = parsedLine[2].Value;
                int r2_offset = 0;
                if (parsedLine[2].Type == ArrMember) {
                    ArrData arrData = GetArrData(parsedLine[2].Value);
                    r2_name = arrData.Name;
                    r2_offset = arrData.Position;
                }
                Reference* r1 = blockHandler->RecursivelyGetReference(r1_name);
                Reference* r2 = blockHandler->RecursivelyGetReference(r2_name);
                AbstractType* type1 = TypeGetter().GetTypeByID(r1->TypeID);
                AbstractType* type2 = TypeGetter().GetTypeByID(r2->TypeID);
                if (r1->TypeID == r2->TypeID) {
                    type1->Overwrite(blockHandler, r1->Address + (r1_offset * type1->GetSize()),
                                     r2->Address + (r2_offset * type2->GetSize()));
                }
                else
                    throw invalid_argument("Objects can only be set to objects of the same time");
            }
            else {
                string r1_name = parsedLine[0].Value;
                int r1_offset = 0;
                if (parsedLine[0].Type == ArrMember) {
                    ArrData arrData = GetArrData(parsedLine[0].Value);
                    r1_name = arrData.Name;
                    r1_offset = arrData.Position;
                }
                Reference* r1 = blockHandler->RecursivelyGetReference(r1_name);
                AbstractType* type1 = TypeGetter().GetTypeByID(r1->TypeID);
                type1->Overwrite(blockHandler, r1->Address + (r1_offset * type1->GetSize()), parsedLine[2]);
            }
        }
        else if (parsedLine[1].Value.size() == 2 && parsedLine[1].Value[1] == '=') { // * a += 1
            string r1_name = parsedLine[0].Value;
            int r1_offset = 0;
            if (parsedLine[0].Type == ArrMember) {
                ArrData arrData = GetArrData(parsedLine[0].Value);
                r1_name = arrData.Name;
                r1_offset = arrData.Position;
            }
            Reference* r1 = blockHandler->RecursivelyGetReference(r1_name);
            AbstractType* type1 = TypeGetter().GetTypeByID(r1->TypeID);
            if (parsedLine[2].Type == Name || parsedLine[2].Type == ArrMember) {
                string r2_name = parsedLine[2].Value;
                int r2_offset = 0;
                if (parsedLine[2].Type == ArrMember) {
                    ArrData arrData = GetArrData(parsedLine[2].Value);
                    r2_name = arrData.Name;
                    r2_offset = arrData.Position;
                }
                Reference* r2 = blockHandler->RecursivelyGetReference(r2_name);
                AbstractType* type2 = TypeGetter().GetTypeByID(r2->TypeID);
                type1->Modify(blockHandler, r1->Address + (r1_offset * type1->GetSize()), parsedLine[1].Value.substr(0, 1), r2->Address + (r2_offset * type2->GetSize()), r1->Address + (r1_offset * type1->GetSize()));
            }
            else {
                type1->Modify(blockHandler, r1->Address + (r1_offset * type1->GetSize()), parsedLine[1].Value.substr(0, 1), parsedLine[2], r1->Address + (r1_offset * type1->GetSize()));
            }
        }
        else {
            throw invalid_argument("Assigner '" + parsedLine[1].Value + "' not currently supported");
        }
    }
    // * Start with keyword e.g. out a
    else if (parsedLine.at(0).Type == Keyword) {
        if (parsedLine[0].Value == "out" || parsedLine[0].Value == "outnl" || parsedLine[0].Value == "nlout") {
            ADDR addr;
            unsigned short typeID;
            bool default_out = true;

            if (parsedLine.size() != 2)
                throw invalid_argument("Out must be followed something");

            if (parsedLine[1].Type == Name) {
                Reference* ref = blockHandler->RecursivelyGetReference(parsedLine[1].Value);
                addr = ref->Address;
                typeID = ref -> TypeID;
            }
            else if (parsedLine[1].Type == ArrMember) {
                int r1_offset = 0;
                ArrData arrData = GetArrData(parsedLine[1].Value);
                string r_name = arrData.Name;
                r1_offset = arrData.Position;
                Reference* ref = blockHandler->RecursivelyGetReference(r_name);
                typeID = ref->TypeID;
                addr = ref->Address + (r1_offset * TypeGetter().GetTypeByID(typeID)->GetSize());
            }
            else if (parsedLine[1].Type == IntLiteral) {
                addr = IntType().Create(blockHandler, "", 1);
                IntType::StaticAssign(blockHandler, addr, stois(parsedLine[1].Value));
                typeID = IntType().TYPE_ID;
            }
            else if (parsedLine[1].Type == StringLiteral && parsedLine[1].Value.size() == 1) {
                addr = CharType().Create(blockHandler, "", 1);
                CharType::StaticAssign(blockHandler, addr, parsedLine[1].Value[0]);
                typeID = CharType().TYPE_ID;
            }
            else if (parsedLine[1].Type == StringLiteral) {
                default_out = false;
                cout << parsedLine[1].Value;

                for (int i = 0; i < parsedLine[1].Value.size(); i++) {
                    addr = CharType().Create(blockHandler, "", 1);
                    CharType::StaticAssign(blockHandler, addr, parsedLine[1].Value[i]);
                    typeID = CharType().TYPE_ID;
                    if (i == 0)
                        blockHandler->PManager->Append(OutInstruction::Build(addr, typeID, parsedLine[0].Value == "nlout",
                                                                         parsedLine[0].Value == "nlout"));
                    else if (i != parsedLine[1].Value.size() - 2)
                        blockHandler->PManager->Append(OutInstruction::Build(addr, typeID, parsedLine[0].Value == "outnl"));
                    else
                        blockHandler->PManager->Append(OutInstruction::Build(addr, typeID));

                }
            }
            else {
                throw invalid_argument("Unsupported out type");
            }

            if (default_out)
                blockHandler->PManager->Append(OutInstruction::Build(addr, typeID, parsedLine[0].Value == "outnl" || parsedLine[0].Value == "nlout",
                                                                 parsedLine[0].Value == "nlout"));
        }
        else {
            throw invalid_argument("Keyword '" + parsedLine[0].Value + "' not supported");
        }
    }
    else if (parsedLine.at(0).Type == Empty) {} // * Ignore empty line
    else {
        throw invalid_argument("Invalid syntax");
    }
}