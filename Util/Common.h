#ifndef PLANG3_COMMON_H
#define PLANG3_COMMON_H

#define BYTE unsigned char
#define ADDR unsigned short
#define BOOL_TRUE 0xFF
#define BOOL_FALSE 0x00
#define F_ENDL '\n'

#include <stdexcept>

using namespace std;

int stois(const std::string& str);

class ArrData {
public:
    string Name;
    int Position;

    ArrData(string name, int position) {
        Name = name;
        Position = position;
    }
};

ArrData GetArrData(string arrMember);

#endif //PLANG3_COMMON_H
