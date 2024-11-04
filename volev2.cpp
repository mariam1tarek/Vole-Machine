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
        if (address >= 0 && address < size) {
            return memory[address];
        }
        cout << "Address out of range: " << address << endl;
        return "00"; // Return default value if out of range
    }

    void setCell(int address, const string& val) {
        if (address >= 0 && address < size) {
            memory[address] = val;
        } else {
            cout << "Address out of range: " << address << endl;
        }
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
        bitset<8> bits(x);  
        unsigned long binaryValue = bits.to_ulong(); 

        stringstream str;
        str << hex << uppercase << setw(2) << setfill('0') << binaryValue;
        return str.str().substr(0, 2);
    }

    string Hexa_to_Des(const string& x){
        int result = stoi(x, nullptr, 16);
        return to_string(result);
    }


    string HexaToBinary(char hex) {
        switch (toupper(hex)) {
            case '0': return "0000";
            case '1': return "0001";
            case '2': return "0010";
            case '3': return "0011";
            case '4': return "0100";
            case '5': return "0101";
            case '6': return "0110";
            case '7': return "0111";
            case '8': return "1000";
            case '9': return "1001";
            case 'A': return "1010";
            case 'B': return "1011";
            case 'C': return "1100";
            case 'D': return "1101";
            case 'E': return "1110";
            case 'F': return "1111";
            default: return "";
        }
    }


    string hexa_String_To_Binary(const string& hex) {
        string binary = "";
        for (char hexDigit : hex) {
            binary += HexaToBinary(hexDigit);
        }
        return binary;
    }


    char BinaryToHexa(const string& binary) {
        if (binary == "0000") return '0';
        if (binary == "0001") return '1';
        if (binary == "0010") return '2';
        if (binary == "0011") return '3';
        if (binary == "0100") return '4';
        if (binary == "0101") return '5';
        if (binary == "0110") return '6';
        if (binary == "0111") return '7';
        if (binary == "1000") return '8';
        if (binary == "1001") return '9';
        if (binary == "1010") return 'A';
        if (binary == "1011") return 'B';
        if (binary == "1100") return 'C';
        if (binary == "1101") return 'D';
        if (binary == "1110") return 'E';
        if (binary == "1111") return 'F';
        return '0'; 
    }


    string binary_To_Hexa_String(const string& binary) {
        string hex = "";
        int length = binary.length();

    
        int padding = (4 - length % 4) % 4;
        string paddedBinary = string(padding, '0') + binary;


        for (int i = 0; i < paddedBinary.length(); i += 4) {
            string fourBits = paddedBinary.substr(i, 4);
            hex += BinaryToHexa(fourBits);
        }

    
        hex.erase(0, min(hex.find_first_not_of('0'), hex.size() - 1));

        return hex;
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
        string binary1= alu.hexa_String_To_Binary(reg.getCell(R2));
        string binary2= alu.hexa_String_To_Binary(reg.getCell(R3));

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

        if (result.length() > binary1.length()) {
            result = result.substr(1);  
        }


        reg.setCell(R1,alu.binary_To_Hexa_String(result));
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
        int R1 = stoi(instruction_R.substr(1, 1), nullptr, 16);
        int R2 = -1;
        int address = -1;

        switch (opcode) {
            case 1:  case 2: case 3: case 11:
                address = stoi(instruction_R.substr(2, 2), nullptr, 16);

                return {opcode, R1, address};
            case 4:
                R2 = stoi(instruction_R.substr(2, 2), nullptr, 16);
                return {opcode, R1, R2};
            case 5:
                R2 = stoi(instruction_R.substr(2, 1), nullptr, 16);
                address = stoi(instruction_R.substr(3, 1), nullptr, 16);
                return {opcode, R1, R2 ,address};
            case 6:
                R2 = stoi(instruction_R.substr(2, 1), nullptr, 16);
                address = stoi(instruction_R.substr(3, 1), nullptr, 16);
                return {opcode, R1, R2, address};


            case 12:
                return {opcode};

            default:
                cout << "Error: Invalid opcode in decode" << endl;
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