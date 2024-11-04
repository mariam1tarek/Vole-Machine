#include <iostream>
#include <string>
using namespace std;

class Memory {
public:
    string memory[256];
    const int size = 256;

    Memory() {
        for (int i = 0; i < size; ++i) {
            memory[i] = "00";
        }
    }

    string getCell(int address) const {
        return memory[address];
    }

    void setCell(int address, const string& val) {
        memory[address] = val.substr(0, 2);
    }
};