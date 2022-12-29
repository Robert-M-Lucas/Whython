//
// Created by rober on 23/12/2022.
//

#include <iostream>
#include <fstream>
#include <cstring>

#include "Util/Common.h"
#include "MemoryManager.h"

extern bool DEBUG;

using namespace std;

ADDR MemoryManager::Alloc(int amount) {
    int new_size = m_size + amount;

    BYTE* new_memory = nullptr;
    if (memory == nullptr)
        new_memory = (BYTE*)malloc(new_size);
    else
        new_memory = (BYTE*)realloc(memory, new_size);

    if (new_memory == nullptr) throw runtime_error("Memory couldn't be allocated");

    ADDR position = m_size;
    m_size = new_size;
    memory = new_memory;

    if (DEBUG)
        cout << name << " size:" << m_size << endl;

    return position;
}

void MemoryManager::Overwrite(int address, vector<BYTE> data) {
    if (address + data.size() > m_size)
        throw invalid_argument("Data too large for address");
    for (int i = 0; i < data.size(); i++) {
        memory[address + i] = data[i];
    }
}

void MemoryManager::Overwrite(int address, const BYTE *data, int size) {
    if (address + size > m_size)
        throw invalid_argument("Data too large for address");
    for (int i = 0; i < size; i++) {
        memory[address + i] = data[i];
    }
}

void MemoryManager::Append(vector<BYTE> data) {
    int address = Alloc((int) data.size());
    Overwrite(address, data);
}

void MemoryManager::Append(const BYTE *data, int size) {
    int address = Alloc(size);
    for (int i = 0; i < size; i++) {
        memory[address + i] = data[i];
    }
}

void MemoryManager::DumpBytes() {
    ofstream stream;
    stream.open("data\\" + name + " - dump.b");
    if (!stream)
        cout << "Opening file failed" << endl;
    // use operator << for clarity
    stream.write((char*) memory, m_size);
    // test if write was successful - not *really* necessary
    if (!stream)
        cout << "Write failed" << endl;
}
