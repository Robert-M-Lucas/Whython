#include <chrono>

#include "Compile.h"
#include "Execute.h"

const char* VERSION = "v0.1";

using namespace std;

bool DEBUG = false; // * Extern

int main(int argc, char *argv[])
{
    if (argc >= 2 && (argv[1][0] == 'd' || argv[1][0] == 'D'))
        DEBUG = true;

    cout << "Whython " << VERSION << endl;

    cout << "Compiling... " << endl;
    auto start = chrono::high_resolution_clock::now();

    try {
        CompileResult result = Compile();

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, std::milli> ms_double = end-start;
        cout << "Time taken: " << ms_double.count() << "ms" << endl;

        cout << "Executing... " << endl << endl;

        start = chrono::high_resolution_clock::now();

        std::ios::sync_with_stdio(true);
        Execute(result);

        end = chrono::high_resolution_clock::now();

        cout.flush();
        std::ios::sync_with_stdio(true);

        ms_double = end-start;
        cout << endl << "Time taken: " << ms_double.count() << "ms" << endl;

        return 2;
    }
    catch (const exception& e) {
        cout << "Compilation failed (" << e.what() << ")" << endl;
        if (DEBUG)
            throw e;
        return -1;
    }
}