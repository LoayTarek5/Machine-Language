#include "Memory.h"
Memory::Memory() {
    memory.resize(256, 0); // Initialize memory with 256 8-bit locations (1 byte per instruction)
}

void Memory::takeMemory(vector<uint8_t> inp) {
    memory = inp;
}

// Function to get the value of a specific memory cell
int Memory::getMemoryCell(int address) const {
    if (address < 0 || address >= 256) {
        cout << "Invalid memory address\n";
        return 0;
    }
    return memory[address];
}

string Memory::intToHexStr(int value) {
    stringstream ss;
    ss << hex << value;
    return ss.str();
}
// Function to convert hex string to an integer
uint16_t Memory::hexToInt(const string& hexStr) {
    uint16_t result;
    stringstream ss;
    ss << hex << hexStr;
    ss >> result;
    return result;
}
// Function to check if a string contains only valid hex digits (0-9, A-F, a-f)
bool Memory::isHex(const string& str) {
    for (char c : str) {
        if (!isxdigit(c)) {  // isxdigit checks if the character is a valid hex digit
            return false;
        }
    }
    return true;
}
// Function to set the value of a specific memory cell
void Memory::setMemoryCell(int address, string value) {
    if(value.size() > 2 || value.size() == 0){
        cout << "Invalid value (value needs to be 1 or 2 hex characters)\n";
        return;
    }

    if (!isHex(value)) {
        cout << "Invalid value (non-hex characters)\n";
        return;
    }

    if (address < 0 || address >= 256) {
        cout << "Invalid memory address\n";
        return;
    }
    memory[address] = hexToInt(value);
}

// Function to print the memory content (for debugging)
void Memory::printMemory() const {
    qInfo() << "Memory content: " << "\n";
    for (int i = 0; i < memory.size(); ++i) {
        QString hexValue = QString("%1").arg(memory[i], 2, 16, QChar('0')).toUpper();
        qInfo() << "HEX: " << hexValue << "\n";
    }
}

const vector<uint8_t>& Memory::returnMemory() const {
    return memory;
}

void Memory::clearMemory(){
    memory.clear();
    memory.resize(256, 0); // Initialize memory with 256 8-bit locations (1 byte per instruction)
}
