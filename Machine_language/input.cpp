#include "Input.h"

Input::Input() {
    input.resize(256, 0); // Initialize input with 256 8-bit locations (1 byte per instruction)
}
// Function to convert hex string to an integer
uint16_t Input::hexToInt(const string& hexStr) {
    uint16_t result;
    stringstream ss;
    ss << hex << hexStr;
    ss >> result;
    return result;
}
// Function to check if a string contains only valid hex digits (0-9, A-F, a-f)
bool Input::isHex(const string& str) {
    for (char c : str) {
        if (!isxdigit(c)) {  // isxdigit checks if the character is a valid hex digit
            return false;
        }
    }
    return true;
}
void Input::takeInputStr(const string& inp) {
    stringstream ss(inp);
    string instruction;
    int address = 0;
    while (ss >> instruction) {
        if (instruction.length() != 4) {
            cout << "Invalid instruction length, skipping: " << instruction << '\n';
            continue;
        }

        if (!isHex(instruction)) {
            cout << "Invalid instruction (non-hex characters), skipping: " << instruction << '\n';
            continue;
        }

        if (address > 255 || address < 0) {

            break;
            // cout << "Memory overflow: Cannot store more than 256 bytes!" << '\n';
            // continue;
        }

        // Split the 4-character instruction into two 8-bit parts
        string firstHalf = instruction.substr(0, 2);  // First two hex digits (1 byte)
        string secondHalf = instruction.substr(2, 2); // Last two hex digits (1 byte)

        data.push_back(instruction);

        // Store them in two consecutive memory locations
        input[address++] = hexToInt(firstHalf);   // Store first 8 bits
        input[address++] = hexToInt(secondHalf);  // Store second 8 bits
    }
}

const vector<uint8_t>& Input::returnInput() const {
    return input;
}

vector <string> Input::returnInputData() const {
    return data;
}
// To read data from file
void Input::takeInputFile(string filename) {
    filesystem::path currentPath = filesystem::current_path() / ".." / filename;
    string fullPathString = currentPath.string();
    ifstream file(fullPathString);
    string data, fulldata;
    if (!file.is_open()) {
        throw runtime_error("Unable to open file: " + filename);
    }
    while (getline(file, data)) {
        fulldata = fulldata + " " + data;
    }
    takeInputStr(fulldata);
}
void Input::printInput() const {
    cout << "Input content:" << endl;
    for (int i = 0; i < 256; ++i) {
        cout << setw(2) << setfill('0') << hex << i << ": "
             << setw(2) << setfill('0') << hex << static_cast<int>(input[i]) << '\n';
    }
}

void Input::clearInput(){
    // input.clear();
    data.clear();
}
