#include <iostream>
using namespace std;
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