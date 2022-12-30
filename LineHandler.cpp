//
// Created by rober on 23/12/2022.
//

#include "LineHandler.h"
#include "Types/AbstractType.h"
#include "Types/IntType.h"
#include "Types/BoolType.h"
#include "Instructions/OutInstruction.h"
#include "Util/TypeGetter.h"

// template <typename T, int N> char(&dim_helper(T(&)[N]))[N];
// #define dim(x) (sizeof(dim_helper(x)))

void HandleLine(vector<LexicalResult> parsedLine, BlockHandler* blockHandler) {
    // * Start with type e.g. int a = 2
    if (parsedLine.at(0).Type == Type) {
        if (parsedLine.size() == 4) {
            if (parsedLine[1].Type != Name || parsedLine[2].Type != Assigner) {
                throw invalid_argument("A type must be followed by a name, assigner or literal");
            }

            if (parsedLine[2].Value == "=") {
                AbstractType *type = TypeGetter().GetTypeByIdentifier(parsedLine[0].Value);

                ADDR addr = type->Create(blockHandler, parsedLine[1].Value);
                type->Assign(blockHandler, addr, parsedLine[3]);
            } else
                throw invalid_argument("Initialisation must be done using an '='");
        }
        else if (parsedLine.size() == 6) {
            if (parsedLine[1].Type != Name || parsedLine[2].Type != Assigner || parsedLine[2].Value != "="
                    || parsedLine[3].Type != Name || parsedLine[4].Type != Operator) {
                throw invalid_argument("A type must be followed by a name, assigner or literal");
            }

            AbstractType* type = TypeGetter().GetTypeByIdentifier(parsedLine[0].Value);
            ADDR addr = type->Create(blockHandler, parsedLine[1].Value);
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
    else if (parsedLine.at(0).Type == Name) {
        if (parsedLine.size() < 3 || parsedLine[1].Type != Assigner)
            throw invalid_argument("A name must be followed by an assigner and a value");

        if (parsedLine.size() == 5) { // * a = b + 1
            if (parsedLine[1].Value != "=" || parsedLine[3].Type != Operator)
                throw invalid_argument("Invalid syntax");
            if (parsedLine[2].Type != Name)
                throw invalid_argument("LHS must be name");

            Reference* r1 = blockHandler->RecursivelyGetReference(parsedLine[0].Value);
            Reference* r2 = blockHandler->RecursivelyGetReference(parsedLine[2].Value);
            AbstractType* type = TypeGetter().GetTypeByID(r2->TypeID);

            if (parsedLine[4].Type == Name) {
                Reference* r3 = blockHandler->RecursivelyGetReference(parsedLine[4].Value);
                if (r2->TypeID != r3->TypeID)
                    throw invalid_argument("Type mismatch");
                int return_type = type->Modify(blockHandler, r2->Address, parsedLine[3].Value, r3->Address, r1->Address);
                if (return_type != r1->TypeID)
                    throw invalid_argument("Type mismatch");
            }
            else {
                int return_type = type->Modify(blockHandler, r2->Address, parsedLine[3].Value, parsedLine[4], r1->Address);
                if (r1->TypeID != return_type)
                    throw invalid_argument("Type mismatch");
            }
        }
        else if (parsedLine[1].Value == "=") { // * a = 1
            if (parsedLine[2].Type == Name) {
                Reference* r1 = blockHandler->RecursivelyGetReference(parsedLine[0].Value);
                Reference* r2 = blockHandler->RecursivelyGetReference(parsedLine[2].Value);
                if (r1->TypeID == r2->TypeID) {
                    AbstractType* type = TypeGetter().GetTypeByID(r1->TypeID);
                    type->Overwrite(blockHandler, r1->Address, r2->Address);
                }
                else
                    throw invalid_argument("Objects can only be set to objects of the same time");
            }
            else {
                Reference* r1 = blockHandler->RecursivelyGetReference(parsedLine[0].Value);
                AbstractType* type = TypeGetter().GetTypeByID(r1->TypeID);
                type->Overwrite(blockHandler, r1->Address, parsedLine[2]);
            }
        }
        else if (parsedLine[1].Value.size() == 2 && parsedLine[1].Value[1] == '=') { // * a += 1
            Reference* r = blockHandler->RecursivelyGetReference(parsedLine[0].Value);
            AbstractType* type = TypeGetter().GetTypeByID(r->TypeID);
            if (parsedLine[2].Type == Name) {
                Reference* r2 = blockHandler->RecursivelyGetReference(parsedLine[2].Value);
                type->Modify(blockHandler, r->Address, parsedLine[1].Value.substr(0, 1), r2->Address, r->Address);
            }
            else {
                type->Modify(blockHandler, r->Address, parsedLine[1].Value.substr(0, 1), parsedLine[2], r->Address);
            }
        }
        else {
            throw invalid_argument("Assigner '" + parsedLine[1].Value + "' not currently supported");
        }
    }
    // * Start with keyword e.g. out a
    else if (parsedLine.at(0).Type == Keyword) {
        if (parsedLine[0].Value == "out" || parsedLine[0].Value == "outnl") {
            ADDR addr;
            unsigned short typeID;

            if (parsedLine.size() != 2)
                throw invalid_argument("Out must be followed something");

            if (parsedLine[1].Type == Name) {
                Reference* ref = blockHandler->RecursivelyGetReference(parsedLine[1].Value);
                addr = ref->Address;
                typeID = ref -> TypeID;
            }
            else if (parsedLine[1].Type == IntLiteral) {
                addr = IntType().Create(blockHandler, "");
                IntType::StaticAssign(blockHandler, addr, stois(parsedLine[1].Value));
                typeID = IntType().TYPE_ID;
            }
            else if (parsedLine[1].Type == StringLiteral && parsedLine[1].Value.size() == 1) {
                addr = CharType().Create(blockHandler, "");
                CharType::StaticAssign(blockHandler, addr, parsedLine[1].Value[0]);
                typeID = CharType().TYPE_ID;
            }
            else {
                throw invalid_argument("Unsupported out type");
            }

            blockHandler->PManager->Append(OutInstruction::Build(addr, typeID, parsedLine[0].Value == "outnl"));
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