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

class Register {
public:
    string memory[16]; 
    int size = 16;

    Register() {
        for (int i = 0; i < size; ++i) {
            memory[i] = "00"; 
        }
    }

    string getCell(int index) const {
        return memory[index];
    }

    void setCell(int index, const string& val) {
        memory[index] = val.substr(0, 2); 
    }
};

class ALU {
public:

    bool isValid(const string& str) const {
        if (str.empty()) return false;
        for (char c : str) {
            if (!isxdigit(c)) return false;
        }
        return true;
    }

    string Dec_To_Hexa(int x) {
        stringstream str;
        str << hex << uppercase << setw(2) << setfill('0') << x;
        return str.str().substr(0, 2);
        
    }

    string Hexa_to_Des(const string& x){
        int result = stoi(x, nullptr, 16);
        return to_string(result);
    }


    string Binary_To_Hexa(string x) {
	    bitset<8> set(x);
        stringstream ss;
        ss << hex << uppercase << setw(2) << setfill('0') << set.to_ulong();
        return ss.str();
    }

        
    string Hexa_To_Binary(string  x){
        bitset<8> bits(stoi(x, nullptr, 16));
        return bits.to_string();
    }
    
};

class CU {
public:
    ALU alu;

    void load(int R, int address, Register& reg, Memory& mem) {
        int value = stoi(mem.getCell(address), nullptr, 16);
        reg.setCell(R, ALU().Dec_To_Hexa(value));
    }

    void load(int R, int value, Register& reg) {
        reg.setCell(R, ALU().Dec_To_Hexa(value));
    }

    void store(int R, int address, Register& reg, Memory& mem) {
        mem.setCell(address, reg.getCell(R));
    }

    void move(int R1, int R2, Register& reg) {
        reg.setCell(R2, reg.getCell(R1));
    }


    void jump(int R, int addr, Register& reg, int& program_Counter) {
        if (reg.getCell(R) == reg.getCell(0)) {
            if (addr %2 ==0){
                program_Counter = addr;
            }else{
                halt();
            }
        }
    }


    void Add_Complement(int R1 , int R2 ,int R3 , Register& reg ){
        string binary1= alu.Hexa_To_Binary(reg.getCell(R2));
        string binary2= alu.Hexa_To_Binary(reg.getCell(R3));

        while (binary1.length() < binary2.length()) binary1 = "0" + binary1;
        while (binary2.length() < binary1.length()) binary2 = "0" + binary2;

        string result = "";  
        int carry = 0;

        for (int i = binary1.length() - 1; i >= 0; i--) {
            int bitA = binary1[i] - '0';
            int bitB = binary2[i] - '0';
            int sum = bitA + bitB + carry;

            carry = sum >> 1;             
            result = to_string(sum % 2) + result;  
        }

        if (carry) result = "1" + result ;
            

        reg.setCell(R1,alu.Binary_To_Hexa(result));
    }


    void Add_Floating(Register& reg, int R1, int R2, int R3) {
    	struct Float8 {
    		unsigned int sign : 1;
    		unsigned int exponent : 3;
    		unsigned int mantissa : 4;
    		static Float8 F_Bin(string X) {
    			Float8 F8{ };
    			F8.sign = (X[0] == '1') ? 1 : 0;
    			F8.exponent = stoi(X.substr(1, 3));
    			F8.mantissa = stoi(X.substr(4));
    			cout << int((X[0] == '1') ? 1 : 0) << stoi(X.substr(1, 3)) << stoi(X.substr(4)) << endl;
    			return (F8);
    		}
    		static Float8 F_Float(float X) {
    			Float8 F8{};
    			F8.sign = (X < 0) ? 1 : 0;
    			X = fabs(X);
    			int exp = 0;
    			while (X >= 2.0) {
    				X /= 2.0;
    				exp++;
    			}
    			while (X < 1.0 && exp > -4) {
    				X *= 2.0;
    				exp--;
    			}

    			F8.exponent = exp + 4;
    			F8.mantissa = static_cast<int>(X * 16) & 0xF;
    			return (F8);
    		}

    		float To_Float() const {
    			float mantissa_Val = static_cast<float>(mantissa) / 16.0f;
    			float result = (1.0 + mantissa_Val) * pow(2, static_cast<int>(exponent) - 4);
    			return (sign == 1) ? -result : result;
    		}

    		static Float8 ADD(const Float8& one, const Float8& two) {
    			float res = one.To_Float() + two.To_Float();
    			return (F_Float(res));
    		}

    		string print_Bin() const {
    			stringstream X;
    			X <<sign <<bitset<3>(exponent)<< bitset<4> (mantissa);
    			return ( X.str());
    		}
    	};
    	Float8 A = Float8::F_Bin(alu.Hexa_To_Binary(reg.getCell(R2)));
    	Float8 B = Float8::F_Bin(alu.Hexa_To_Binary(reg.getCell(R3)));
    	Float8 res = Float8::ADD(A, B);
    	reg.setCell(R1, alu.Binary_To_Hexa(res.print_Bin()));
}




    void halt() {
        cout << "Program halted." << endl;
        exit(0);
    }
};

class CPU {
public:
    int programCounter = 0;
    string instruction_R;
    Register reg;
    Memory mem;
    ALU alu;
    CU cu;

    vector<int> check(const string& instruction_R) {

        int opcode = stoi(instruction_R.substr(0, 1), nullptr, 16);
        int address =-1;
        int R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
        int R2 = -1;  
        

        switch (opcode) {
            case 1: case 2: case 3: case 4: case 11:
                R2 = stoi(instruction_R.substr(2, 2), nullptr, 16);
                address =stoi(instruction_R.substr(2, 1), nullptr, 16);
                
                return {opcode, R1, R2, address};

            case 5: case 6:
                R2 = stoi(instruction_R.substr(2, 1), nullptr, 16);
                address =stoi(instruction_R.substr(2, 1), nullptr, 16);
                return {opcode, R1, R2 };

            case 12:
                return {opcode};  // Halt does not require R1/R2/address

            default:
                cout << "Invalid opcode in check" << endl;
                return {};
        }
    }

    void execute(int opcode, int R1=-1, int R2=-1, int address=-1) {
        switch (opcode) {
            case 1:
                cu.load(R1, address, reg, mem);
                break;
            case 2:
                cu.load(R1, R2, reg);
                break;
            case 3:
                cu.store(R1, address, reg, mem);
                break;
            case 4:
                cu.move(R1, R2, reg);
                break;
            case 5:
                cu.Add_Complement(R1, R2, address, reg);
                break;
            case 6:
                cu.Add_Floating(reg,  R1,  R2, address);
                break;
            case 11:
                cu.jump(R1, address, reg, programCounter);
                break;
            case 12:
                cu.halt();
                break;
            default:
                cout << "Invalid opcode!" << endl;
        }
    }

    void Next_Step(Memory& mem) {
        // Fetch
        instruction_R = mem.getCell(programCounter) + mem.getCell(programCounter + 1);
        programCounter += 2;

        // Decode
        vector<int> decoded = check(instruction_R);
        if (decoded.empty()) return;

        // Execute
        int opcode = decoded[0];
        int R1 = decoded.size() > 1 ? decoded[1] : -1;
        int R2 = decoded.size() > 2 ? decoded[2] : -1;
        int address = decoded.size() > 3 ? decoded[3] : -1;
        execute(opcode, R1, R2, address);
    }
};




class Machine {
public:
    Memory memory;
    CPU processor;
    

    bool load_File(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return false;
        }

        ALU val;
        string instruction;
        int address = 0;

        while (inputFile >> instruction && address < memory.size) {
            if (val.isValid(instruction)){
                memory.setCell(address++, instruction.substr(0, 2));
                 if (instruction.size() > 2) {
                    memory.setCell(address++, instruction.substr(2, 2));
                }
            }else{
                cout <<"Invalid Instruction"<<"\n";
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

int main() {
    MainUI ui;
    Machine machine;

    ui.enterFile();
    string filename = ui.getFile();

    if (machine.load_File(filename)) {
        cout << "Program loaded successfully.\n";
        machine.output();
    } else {
        cerr << "Failed to load program.\n";
        return 1;
    }

    while (true) {
        machine.processor.Next_Step(machine.memory);
        machine.output();
    }

    return 0;
}