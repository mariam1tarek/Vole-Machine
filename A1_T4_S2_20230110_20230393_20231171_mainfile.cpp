//Main File

#include "A1_T4_S2_20230110_20230393_2023117.h"

#include "A1_T4_S2_20230110_20230393_2023117_implementationfile.cpp"

void main_menu(){
    cout <<"**what do you want to do?"<<endl;
    cout <<"1-Load file"<<endl;
    cout <<"2-Run step by step"<<endl;
    cout <<"3-Display system status "<<endl;
    cout <<"4-Reset Machine"<<endl;
    cout <<"5-Exit"<<endl;
    cout <<"** Enter your choice (1-5):"<<endl;
}

int main() {
    MainUI ui;
    Machine machine;
    int choice;
    cout <<"**Welcome to our program"<<endl;

    
    do {
        main_menu();
        while (!(cin >> choice) || choice < 1 || choice > 5) {
            cout <<"\n";
            cout << "** Invalid choice. Please enter an integer number between 1 and 5.\n";
            cout << "** Enter your choice(1-5):";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                ui.enterFile();
                string filename = ui.getFile();
            
                if (machine.load_File(filename)) {
                    cout << "Program loaded successfully.\n";
                    machine.output();
             
                    // Loop until a halt instruction is encountered
                    while (!machine.isHalted()) {
                        machine.processor.Next_Step(machine.memory);
                        machine.output();
                    }
             
                    cout << "Execution halted.\n";
                    cout <<"\n";

                    
                } else {
                    cerr << "Failed to load program.\n";
                    cout <<"\n";
                    return 1;
                }
                break;
            }
    
            case 2: {
                cout << "** Enter instructions in hexadecimal (press 'C000' or 'c000' to finish):\n";

                string instruction;
                int address = 0;

                while (true) {
                    cout << "Instruction: ";
                    getline(cin, instruction);

                    // Check if the user wants to finish input
                    if (instruction == "C000" || instruction == "c000") {
                        break;
                    }

                    // Validate instruction format and store it in memory
                    if (instruction.length() == 4 && address < 256) {
                        char firstChar = toupper(instruction[0]);
                        if ((firstChar >= '1' && firstChar <= '6') || firstChar == 'B' || firstChar == 'C') {
                            bool validHex = true;
                            for (int i = 1; i < instruction.length(); ++i) {
                                char currentChar = toupper(instruction[i]);
                                if ((currentChar < '0' || currentChar > 'F') && !(currentChar >= 'A' && currentChar <= 'F')) {
                                    validHex = false;
                                    break;
                                }
                            }

                            if (validHex) {
                                machine.memory.setCell(address++, instruction.substr(0, 2));
                                machine.memory.setCell(address++, instruction.substr(2, 2));

                                // Run the current step and output the result
                                machine.processor.Next_Step(machine.memory);
                                machine.output();

                                // Ask if the user wants to continue or return to the main menu
                                char choice;
                                cout << "\nDo you want to continue? (y to continue, any other key to return to main menu): ";
                                cin >> choice;
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                if (tolower(choice) != 'y') {
                                    break;
                                }
                            } else {
                                cout << "Invalid instruction. The last three characters must be between '0' and 'F'.\n";
                            }
                        } else {
                            cout << "Invalid instruction. The first character must be one of the following: 1, 2, 3, 4, 5, 6, B, C.\n";
                        }
                    } else {
                        cout << "Invalid instruction format or memory limit exceeded. Please enter a valid 4-character instruction.\n";
                    }

                    if (address >= 256) {
                        cout << "Memory limit reached. Some instructions may not have been loaded.\n";
                        break;
                    }
                }

                machine.resetHalt();  // Reset the halt flag after execution
                break;
            }
            case 3: {
                display_system_status(machine);
                break;
            }
            case 4:{
                machine.reset(); 
                cout <<"Memory & register returned to zero's successfully"<<endl;
                cout<<"\n";
                cout<<"\n";
                break;
            }
            case 5:
                cout <<"\n";
                cout <<"\n";
                cout<<"** Thank you for using this program"<<"\n";
                cout <<"** Good bye:(" <<"\n";
                return 0;

            default:
                cout << "** Invalid choice. Please enter a valid option.\n\n";
                break;
        }
    } while (true);



    return 0;
}