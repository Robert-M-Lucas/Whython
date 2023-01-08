#include <chrono>

#include "Compilation/Compile.h"
#include "Execution/Execute.h"

const char* VERSION = "v0.1";

using namespace std;

bool DEBUG = false; // * Extern

int main(int argc, char *argv[])
{
    cout << "Whython " << VERSION << F_ENDL;

    if (argc < 2) {
        cout << "No file provided" << F_ENDL;
        return -1;
    }

    if (argc >= 3 && (argv[2][0] == 'd' || argv[2][0] == 'D'))
        DEBUG = true;

    bool from_compiled = false;
    string file_string(argv[1]);
    if (file_string.size() >= 9 && file_string.substr(file_string.size()-9, 9) == ".compiled")
        from_compiled = true;

    try {
        CompileResult result;

        if (!from_compiled) {
            cout << "Compiling... " << F_ENDL;
            auto start = chrono::high_resolution_clock::now();

            result = Compile(argv[1], (string(argv[1]) + ".compiled").c_str());

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double, std::milli> ms_double = end - start;
            cout << "Time taken: " << ms_double.count() << "ms" << F_ENDL;
        }
        else {
            cout << "Loading compiled file..." << F_ENDL;
            ifstream stream;
            stream.open(argv[1]);
            if (!stream) {
                cout << "Reading file failed" << F_ENDL;
                return -1;
            }
            stream.seekg(0, std::ios::end);
            int length = stream.tellg();
            stream.seekg(0, std::ios::beg);
            char* file = (char*) malloc(length);
            stream.read(file, length);

            int v_memory_size = bytesToInt(reinterpret_cast<BYTE*>(file));
            BYTE* v_memory = (BYTE*) malloc(v_memory_size);
            mempcpy(v_memory, file + 4, v_memory_size);
            int p_memory_size = (length - v_memory_size) - 4;
            BYTE* p_memory = (BYTE*) malloc(p_memory_size);
            mempcpy(p_memory, file + 4 + v_memory_size, p_memory_size);

            if (DEBUG)
                cout << length << F_ENDL << v_memory_size << F_ENDL << p_memory_size << F_ENDL;

            result = CompileResult(v_memory_size, v_memory, p_memory_size, p_memory);

            free(file);
        }

        cout << "Executing... " << F_ENDL << "---------" << F_ENDL;

        auto start2 = chrono::high_resolution_clock::now();

        std::ios::sync_with_stdio(true);
        Execute(result);

        auto end2 = chrono::high_resolution_clock::now();

        cout.flush();
        std::ios::sync_with_stdio(true);

        chrono::duration<double, std::milli> ms_double2 = end2-start2;
        cout << F_ENDL << "---------" << F_ENDL << "Time taken: " << ms_double2.count() << "ms" << F_ENDL;

        return 2;
    }
    catch (const exception& e) {
        cout << "Compilation failed (" << e.what() << ")" << F_ENDL;
        if (DEBUG)
            throw e;
        return -1;
    }
}