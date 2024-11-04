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

class CU {
public:
    CU() {}
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

