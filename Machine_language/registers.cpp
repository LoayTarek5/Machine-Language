#include "Registers.h"

Register::Register() {
    registers.resize(16, 0); // Initialize register with 256 8-bit locations (1 byte per instruction)
}

// Function to get the value of a specific register cell
int Register::getRegisterCell(int address) const {
    if (address < 0 || address >= 16) {
        cout << "Invalid register address\n";
        return 0;
    }
    return registers[address];
}

string Register::intToHexStr(int value) {
    stringstream ss;
    ss << hex << value;
    return ss.str();
}

// Function to convert hex string to an integer
uint16_t Register::hexToInt(const string& hexStr) {
    uint16_t result;
    stringstream ss;
    ss << hex << hexStr;
    ss >> result;
    return result;
}

uint16_t Register::hexToInt(const char& hexStr) {
    uint16_t result;
    stringstream ss;
    ss << hex << hexStr;
    ss >> result;
    return result;
}

// Function to check if a string contains only valid hex digits (0-9, A-F, a-f)
bool Register::isHex(const string& str) {
    for (char c : str) {
        if (!isxdigit(c)) {  // isxdigit checks if the character is a valid hex digit
            return false;
        }
    }
    return true;
}

// Function to set the value of a specific register cell
void Register::setRegisterCell(int address, string value) {
    if(value.size() > 2 || value.empty()){
        cout << "Invalid value (value needs to be 1 or 2 hex characters)\n";
        return;
    }

    if (!isHex(value)) {
        cout << "Invalid value (non-hex characters)\n";
        return;
    }

    if (address < 0 || address >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    if(hexToInt(value) > 255){ // If it exceeds 255 we take last 2 digits only.
        value = value.substr(1, 2);
    }
    registers[address] = hexToInt(value);

}

void Register::printRegisters() const {
    qInfo() << "Registers content: " << "\n";
    for (int i = 0; i < 16; ++i) {
        QString hexValue = QString("%1").arg(registers[i], 2, 16, QChar('0')).toUpper();
        qInfo() << "HEX: " << hexValue << "\n";
    }
}

const vector<uint8_t>& Register::returnRegisters() const {
    return registers;
}

void Register::clearRegisters(){
    registers.clear();
    registers.resize(16, 0); // Initialize register with 256 8-bit locations (1 byte per instruction)
}
