//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_MEMORYMANAGER_H
#define PLANG3_MEMORYMANAGER_H

#include <string>
#include <stdexcept>
#include <vector>

#include "../Util/Common.h"

using namespace std;

class MemoryManager {
private:
    BYTE* memory = nullptr;
    ADDR m_size = 0;
    string name;

public:
    explicit MemoryManager(string name = "No name") {
        MemoryManager::name = name;
    }

    [[nodiscard]] int GetSize() const { return m_size; }

    [[nodiscard]] int GetCurrentPosition() const { return m_size; }

    ADDR Alloc(int amount);

    void Overwrite(int address, vector<BYTE> data);

    void Overwrite(int address, const BYTE* data, int size);

    void Append(vector<BYTE> data);

    [[maybe_unused]] void Append(const BYTE* data, int size);

    BYTE* GetPointer() { return memory; }

    void DumpBytes();

    void FreeMemory() { free(memory); }
};


#endif //PLANG3_MEMORYMANAGER_H
