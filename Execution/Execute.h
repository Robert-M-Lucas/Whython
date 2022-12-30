//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_EXECUTE_H
#define PLANG3_EXECUTE_H

#include <iostream>

#include "../Util/Common.h"
#include "../Util/Conversions.h"
#include "../Compilation/Compile.h"

using namespace std;

extern bool DEBUG;

void MemOut(int size, BYTE* data) {
    for (int i = 0; i < size; i++) {
        if (i%8 == 0)
            cout << endl;
        cout << (int)data[i] << '|';
    }
    cout << endl;
}

void Execute(CompileResult compiled) {
    if (DEBUG) {
        cout << "Executing program with program size " << compiled.p_size << endl;
        MemOut(compiled.v_size, compiled.v_memory);
        cout << "VarMem" << endl;


        cout << "ProgMem" << endl;
        MemOut(compiled.p_size, compiled.p_memory);
        cout << endl;
    }

    int i = 0;
    while (i < compiled.p_size) {
        BYTE code = compiled.p_memory[i];
        i++;

        if (DEBUG) {
            cout << "Executing instruction with code " << (int)code << endl;
        }

        if (code == 0) { // * WRITE
            ADDR address = bytesToShort(compiled.p_memory + i); // Read addr
            compiled.v_memory[address] = compiled.p_memory[i + 2]; // Write value to addr
            i += 3;
        }
        else if (code == 1 || code == 10) { // * OUT
            ADDR address = bytesToShort(compiled.p_memory + i); // Read addr
            unsigned short type = bytesToShort(compiled.p_memory + (i + 2));
            switch (type) {
                case 0: // OUT INT
                    cout << bytesToInt(compiled.v_memory + address);
                    break;
                case 1: // OUT BOOL
                    if (*(compiled.v_memory + address) == BOOL_FALSE)
                        cout << "false";
                    else
                        cout << "true";
                    break;
                case 2: // OUT char
                    cout << *(compiled.v_memory + address);
                    break;
                default:
                    cout << "Type [" << type << "] doesn't support out";
            }

            if (code == 10)
                cout << F_ENDL;

            i += 4;
        }
        else if (code == 2) { // * GOTO
            ADDR address = bytesToShort(compiled.p_memory + i); // Read addr
            i = address;
        }
        else if (code == 3) { // * COPY
            ADDR addr_from = bytesToShort(compiled.p_memory + i);
            ADDR addr_to = bytesToShort(compiled.p_memory + (i + 2));
            unsigned short amount = bytesToShort(compiled.p_memory + (i + 4));
            for (unsigned short j = 0; j < amount; j++) {
                compiled.v_memory[addr_to+j] = compiled.v_memory[addr_from+j];
            }
            i += 6;
        }
        else if (code == 4) { // * ADD
            ADDR to_modify = bytesToShort(compiled.p_memory + i);
            ADDR modify_with = bytesToShort(compiled.p_memory + (i + 2));
            ADDR out_addr = bytesToShort(compiled.p_memory + (i + 4));
            unsigned char size = *(compiled.p_memory + i + 6);

            bool carry = false;
            for (unsigned char j = size-1; j <= size; j--) {
                unsigned char v = compiled.v_memory[to_modify+j];
                v += compiled.v_memory[modify_with+j];
                if (carry)
                    v += 1;
                carry = (v < compiled.v_memory[to_modify+j]) || (v < compiled.v_memory[modify_with+j]) ||
                        (v == compiled.v_memory[to_modify+j] && carry) || (v == compiled.v_memory[modify_with+j] && carry); // ! Overflow
                compiled.v_memory[out_addr+j] = v;
            }

            i += 7;
        }
        else if (code == 9) { // * SUBTRACT
            ADDR to_modify = bytesToShort(compiled.p_memory + i);
            ADDR modify_with = bytesToShort(compiled.p_memory + (i + 2));
            ADDR out_addr = bytesToShort(compiled.p_memory + (i + 4));
            unsigned char size = *(compiled.p_memory + (i + 6));

            BYTE* modify_with_copy = (BYTE*) malloc(size);
            memcpy(modify_with_copy, compiled.v_memory + modify_with, size);

            /*
            for (int j = 0; j < 4; j++) {
                for(int k = 7; k >= 0; k--){
                    int tmp = ( compiled.v_memory[modify_with + j] >> k ) & 0x1;
                    cout << tmp;
                }
            }
            cout << endl;
             */

            // * Invert
            bool inverting = false;
            for (int j = size - 1; j >= 0; j--) {
                BYTE byte = *(modify_with_copy + j);
                if (inverting) {
                    *(modify_with_copy + j) = ~byte;
                }
                else {
                    BYTE byte_out = 0x00;
                    for (int k = 0; k < 8; k++) {
                        BYTE bit = (byte >> k) & 0x01; // Get single bit on right
                        if (inverting) {
                            if (bit == 0x01) bit = 0x00;
                            else bit = 0x01;
                        }
                        else {
                            if (bit == 0x01) inverting = true;
                        }
                        bit = bit << k; // Shift bit back to right
                        byte_out = byte_out | bit; // Recombine with out
                    }
                    *(modify_with_copy + j) = byte_out;
                }
            }

            bool carry = false;
            for (unsigned char j = size-1; j <= size; j--) {
                unsigned char v = compiled.v_memory[to_modify+j];
                v += *(modify_with_copy + j);
                if (carry)
                    v += 1;
                carry = (v < compiled.v_memory[to_modify+j]) || (v < *(modify_with_copy + j)) ||
                        (v == compiled.v_memory[to_modify+j] && carry) || (v == *(modify_with_copy + j) && carry); // ! Overflow
                compiled.v_memory[out_addr+j] = v;
            }

            free(modify_with_copy);
            i += 7;
        }
        else if (code == 5 || code == 6) { // * EQUALS & NOT EQUALS
            ADDR to_modify = bytesToShort(compiled.p_memory + i);
            ADDR modify_with = bytesToShort(compiled.p_memory + (i + 2));
            ADDR out_addr = bytesToShort(compiled.p_memory + (i + 4));
            unsigned char size = *(compiled.p_memory + i + 6);

            bool equal = true;
            for (unsigned char j = 0; j < size; j++) {
                if (compiled.v_memory[to_modify + j] != compiled.v_memory[modify_with + j]) {
                    equal = false;
                    break;
                }
            }

            if (code == 6)
                equal = !equal; // ! Not equals support

            if (equal)
                compiled.v_memory[out_addr] = BOOL_TRUE;
            else
                compiled.v_memory[out_addr] = BOOL_FALSE;

            i += 7;
        }
        else if (code == 7 || code == 8) { // * GOTO IF & GOTO IF NOT
            ADDR goto_address = bytesToShort(compiled.p_memory + i); // Read addr
            ADDR byte_address = bytesToShort(compiled.p_memory + (i + 2)); // Read addr
            BYTE byte = compiled.v_memory[byte_address];

            bool go = false;
            if (byte == BOOL_TRUE) go = true;
            if (code == 8) go = !go; // ! GOTO IF NOT

            if (go)
                i = goto_address;
            else
                i += 4;
        }
        else {
            cout << "UNSUPPORTED INSTRUCTION - " << (int) code << endl;
        }
    }

    free(compiled.p_memory);
    free(compiled.v_memory);
}

#endif //PLANG3_EXECUTE_H
