#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <cstdint>
#include <bitset>

using namespace std;

class ALU {
public:
    ALU() {}

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