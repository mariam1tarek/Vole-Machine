#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cstdint>
#include <bitset>

using namespace std;

class Machine {
public:
    CPU processor;
    Memory memory;

    bool load_File(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }

        string instruction;
        int address = 0;

        while (inputFile >> instruction && address < memory.size) {
            memory.setCell(address++, instruction.substr(0, 2));
            if (instruction.size() > 2) {
                memory.setCell(address++, instruction.substr(2, 2));
            }
        }

        inputFile.close();
        return true;
    }

    void output() {
        cout << "Memory:" << endl;
        for (int i = 0; i < 256; ++i) {
            if (i % 16 == 0) cout << endl;
            cout << setw(2) << setfill('0') << memory.getCell(i) << "  ";
        }
        cout<<endl;
        cout << "\nRegisters:" << endl;
        for (int i = 0; i < 16; ++i) {
            cout << "R"<< i <<":";
            cout << setw(2) << setfill('0') << hex << uppercase << processor.reg.getCell(i) << endl;
        }
        cout << dec;
    }
};

class MainUI {
public:
    string inputFile;

    void enterFile() {
        cout << "Enter the name of the input file (with extension): ";
        cin >> inputFile;
    }

    string getFile() const {
        return inputFile;
    }
};