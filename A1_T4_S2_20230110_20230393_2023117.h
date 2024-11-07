#ifndef Simulator_H
#define Simulator_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cstdint>
#include <bitset>
#include <limits>

using namespace std;

class Memory {
public:
    string memory[256];
    const int size = 256;
    Memory();
    string getCell(int address) const;
    void setCell(int address, const string& val);
};

class Register {
public:
    string memory[16];
    int size = 16;
    Register();
    string getCell(int index) const;
    void setCell(int index, const string& val);
};

class ALU {
public:
    ALU();
    string Dec_To_Hexa(int x);
    string Hexa_to_Des(const string& x);
    string HexaToBinary(char hex);
    string hexa_String_To_Binary(const string& hex);
    char BinaryToHexa(const string& binary);
    string binary_To_Hexa_String(const string& binary);
    string Binary_To_Hexa(string x);
    string Hexa_To_Binary(string x);
};

class CU {
public:
    CU();
    void load(int R, int address, Register& reg, Memory& mem);
    void load(int R, int value, Register& reg);
    void store(int R, int address, Register& reg, Memory& mem);
    void move(int R1, int R2, Register& reg);
    void jump(int R, int addr, Register& reg, int& program_Counter);
    void Add_Complement(int R1, int R2, int R3, Register& reg);
    void Add_Floating(Register& reg, int R1, int R2, int R3);
    void halt();
    bool isHalted() const { return halted; } 
    void resetHalt() { halted = false; } 

private:
    ALU alu;
    bool halted = false; 

};

class CPU {
public:
    int programCounter = 0;
    string instruction_R;
    Register reg;
    ALU alu;
    CU cu;
    void Next_Step(Memory& mem);
};

class Machine {
public:
    CPU processor;
    Memory memory;
    bool load_File(const string& filename);
    void output();
    bool isHalted() const { return processor.cu.isHalted(); } 
    void resetHalt() { processor.cu.resetHalt(); }
    void reset();
    
};

class MainUI {
public:
    string inputFile;
    void enterFile();
    string getFile() const;
};

#endif // Simulator_H