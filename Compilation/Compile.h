//
// Created by rober on 23/12/2022.
//

#ifndef PLANG3_COMPILE_H
#define PLANG3_COMPILE_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <stdexcept>

#include "../Util/Common.h"
#include "MemoryManager.h"
#include "BlockHandler.h"
#include "../Preprocessing/LexicalAnalyser.h"
#include "../Preprocessing/LexicalResult.h"
#include "../Util/Conversions.h"

class CompileResult {
public:
    int v_size;
    BYTE* v_memory;
    int p_size;
    BYTE* p_memory;

    CompileResult()= default;

    CompileResult(int v_size, BYTE* v_memory, int p_size, BYTE* p_memory) {
        CompileResult::v_size = v_size;
        CompileResult::v_memory = v_memory;
        CompileResult::p_size = p_size;
        CompileResult::p_memory = p_memory;
    }
};

extern bool DEBUG;

CompileResult Compile(char* file_location, const char* compiled_file_location = nullptr) {
    auto* VManager = new MemoryManager("VarMemory");
    auto* PManager = new MemoryManager("ProgMemory");
    auto* currentBlockHandler = new BlockHandler(nullptr, nullptr, PManager, VManager);

    fstream file;
    file.open(file_location, ios::in);

    string line;
    int l = 0;
    if (file.is_open())
    {
        while (getline(file, line)) {
            if (DEBUG)
                cout << "Now processing: " << line << F_ENDL;
            try {
                vector<LexicalResult> result = LexicalAnalyser::ProcessLine(line);
                if (result.empty()) { l++; continue; }
                currentBlockHandler = currentBlockHandler->OnLineParsed(result);
            }
            catch (const exception& e) {
                cout << "Error - Line " << l + 1 << ": " << F_ENDL;
                cout << "    " << e.what() << F_ENDL;
                if (DEBUG)
                    throw e;
                throw runtime_error("Compilation failed");
            }
            l++;
        }

        // ! Exit all blocks
        vector<LexicalResult> empty_line;
        empty_line.emplace_back(Empty, 0, "");

        while (currentBlockHandler->Depth != 0) {
            currentBlockHandler->OnExit(empty_line, 1);
            auto* newBlockHandler = currentBlockHandler->Above;
            delete currentBlockHandler;
            currentBlockHandler = newBlockHandler;
        }

        // Create memory dump files
        if (DEBUG) {
            PManager->DumpBytes();
            VManager->DumpBytes();
        }

        file.close();
    }
    else {
        throw invalid_argument("File does not exist");
    }

    int v_size = VManager->GetSize();
    BYTE* v_memory = VManager->GetPointer();
    int p_size = PManager->GetSize();
    BYTE* p_memory = PManager->GetPointer();

    delete VManager;
    delete PManager;
    delete currentBlockHandler;

    // * Save compiled file
    if (compiled_file_location != nullptr) {
        ofstream stream;
        stream.open(compiled_file_location);
        if (!stream)
            cout << "Opening file '" << compiled_file_location << "' failed" << F_ENDL;
        else {
            vector<BYTE> size_bytes = intToBytes(v_size);
            stream.write((char*) &(size_bytes.front()), 4);
            stream.write((char*) v_memory, v_size);
            stream.write((char*) p_memory, p_size);
            if (DEBUG)
                cout << v_size << F_ENDL << p_size << F_ENDL;
            cout << "Saved compiled file" << F_ENDL;
        }
        stream.close();
    }

    return {v_size, v_memory, p_size, p_memory};
}


#endif //PLANG3_COMPILE_H
