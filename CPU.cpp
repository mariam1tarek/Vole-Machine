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

class CPU {
public:
    int programCounter = 0;
    string instruction_R;
    Register reg;
    ALU alu;
    CU cu;

    void Next_Step(Memory& mem) {
        instruction_R = mem.getCell(programCounter) + mem.getCell(programCounter + 1);
        programCounter += 2;

        int opcode = stoi(instruction_R.substr(0, 1), nullptr, 16);
        int R1;
        int R2;
        int address;
        // int address = stoi(instruction_R.substr(2, 1), nullptr, 16);
        // int R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
        // int R2 = stoi(instruction_R.substr(2, 2), nullptr, 16);
        
        switch (opcode) {
            case 1: // Load from memory
                address = stoi(instruction_R.substr(2, 2), nullptr, 16);
                R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
                R2 = stoi(instruction_R.substr(2, 2), nullptr, 16);
                cu.load(R1, address, reg, mem);
                break;
            case 2: // Load immediate value
                address = stoi(instruction_R.substr(2, 2), nullptr, 16);
                R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
                R2 = stoi(instruction_R.substr(2, 2), nullptr, 16);
                cu.load(R1, R2, reg);
                break;
            case 3: // Store
                address = stoi(instruction_R.substr(2, 2), nullptr, 16);
                R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
                R2 = stoi(instruction_R.substr(2, 2), nullptr, 16);
                cu.store(R1, address, reg, mem);
                break;
            case 4: // Move
                // int address = stoi(instruction_R.substr(2, 2), nullptr, 16);
                R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
                R2 = stoi(instruction_R.substr(2, 2), nullptr, 16);
                cu.move(00, R2, reg);
                break;
            case 5: // Add (two's complement)
                address = stoi(instruction_R.substr(3, 1), nullptr, 16);
                R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
                R2 = stoi(instruction_R.substr(2, 1), nullptr, 16);
                cu.Add_Complement( R1 ,  R2 , address ,  reg );
                break;
            case 6: // Add (floating-point)     
                address = stoi(instruction_R.substr(3, 1), nullptr, 16);
                R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
                R2 = stoi(instruction_R.substr(2, 1), nullptr, 16);
                cu.Add_Floating(reg,  R1,  R2,  address);
                break;
            case 11: // Jump
                address = stoi(instruction_R.substr(2, 2), nullptr, 16);
                R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
                R2 = stoi(instruction_R.substr(2, 2), nullptr, 16);
                cu.jump(R1, address, reg, programCounter);
                break;
            case 12: // Halt
                cu.halt();
                break;
            default:
                cout << "Invalid opcode!" << endl;
                break;
        }
    }
};


