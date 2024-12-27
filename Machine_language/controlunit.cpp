#include "ControlUnit.h"
void cu::fetch(Memory &mem, Register &regi){
    memory = mem;
    registers = regi;
}
void cu::fetch(Memory &mem){
    memory = mem;
}

// LOAD the register R with the value found in the memory cell whose address is XY.
void cu::copyMemToReg_1(string instructions)
{
    // The Place Number Of The Register
    int reg_num = registers.hexToInt(instructions[1]);

    // Memory Address
    string address_hex = instructions.substr(2, 2);
    int address = memory.hexToInt(address_hex);
    if(address > 255 || address < 0){
        return;
    }

    registers.setRegisterCell(reg_num, memory.intToHexStr(memory.getMemoryCell(address)));
}

//  LOAD the register R with the bit pattern XY.
void cu::copyStrToReg_2(string instructions)
{
    // The Place Number Of The Register
    int reg_num = registers.hexToInt(instructions[1]);

    // Memory Address
    string address_hex = instructions.substr(2, 2);

    registers.setRegisterCell(reg_num, address_hex);
}


void cu::copyRegToMem_3(string instructions)
{
    // Register address to copy it into memory.
    int reg_num = registers.hexToInt(instructions[1]);

    // Memory Address
    int address = memory.hexToInt(instructions.substr(2, 2));

    if(address > 255 || address < 0){
        return;
    }

    memory.setMemoryCell(address, memory.intToHexStr(registers.getRegisterCell(reg_num)));
}

// 40RS Copy the content found in register R to register S.
void cu::copyRegToReg_4(string instructions)
{
    // Retrieve the content in Register Cell number R
    int content_cell = registers.getRegisterCell(registers.hexToInt(instructions[2]));

    registers.setRegisterCell(registers.hexToInt(instructions[3]), registers.intToHexStr(content_cell));
}

// Add in two's complement notation.
void cu::addTwoComplement_5(int index1, int index2, int index3) {
    int8_t reg2Value = static_cast<int8_t>(registers.getRegisterCell(index2));
    int8_t reg3Value = static_cast<int8_t>(registers.getRegisterCell(index3));

    int8_t result = reg2Value + reg3Value;

    registers.setRegisterCell(index1, registers.intToHexStr(static_cast<uint8_t>(result)));
}

double cu::registerToFloat(uint8_t hexnum) {
    // Extract components
    uint8_t S = (hexnum >> 7) & 0x01;          // Sign bit

    // Convert Sign
    double sign = (S == 0) ? 1.0 : -1.0;

    uint8_t E = (hexnum >> 4) & 0x07;        // Exponent bits (3 bits)

    // Convert Exponent
    int bias = 4;
    int exp = E; // Already in decimal
    int actual_exp = exp - bias;

    uint8_t M = hexnum & 0x0F;               // Mantissa bits (4 bits)
    // Convert Mantissa
    double mantissa_value = 0.0; // Initialize mantissa value
    for (int i = 0; i < 4; ++i) {
        if (M & (1 << (3 - i))) { // Check each bit of mantissa
            mantissa_value += 1.0 / pow(2, i + 1); // 1/(2^(i+1))
        }
    }

    // Calculate final value
    double result = sign * mantissa_value * pow(2, actual_exp);

    return result;
}

uint8_t cu::floatToRegister(double value) {
    if (value == 0.0) {
        return 0; // Edge case for zero
    }

    // Determine the sign bit
    uint8_t S = (value < 0) ? 1 : 0;
    if (S) {
        value = -value; // Work with positive value for further calculations
    }

    // Normalize the value to find exponent and mantissa
    int exp = 0;
    while (value >= 2.0) {
        value /= 2.0;
        exp++;
    }
    while (value < 1.0) {
        value *= 2.0;
        exp--;
    }

    // Calculate biased exponent (bias is 4)
    uint8_t EEE = exp + 4; // Add bias

    // Calculate mantissa (4 bits)
    uint8_t MMMM = 0;
    for (int i = 0; i < 4; ++i) {
        value -= 1.0 / pow(2, i + 1); // Remove the leading 1 (implicit)
        if (value > 0) {
            MMMM |= (1 << (3 - i)); // Set the respective bit
            value -= 1.0 / pow(2, i + 1); // Subtract the value added
        }
    }

    // Combine into one uint8_t value
    uint8_t hexnum = (S << 7) | (EEE << 4) | (MMMM & 0x0F);

    // Validate exponent range (it should fit in 3 bits)
    if (EEE > 7) {
        throw overflow_error("Resulting exponent out of range.");
    }
    hexnum += 16;
    return hexnum;
}

// Add in floating point notation.
void cu::addIEEEFloatingPoint_6(int index1, int index2, int index3) {
    float reg2Value = registerToFloat(registers.getRegisterCell(index2));
    float reg3Value = registerToFloat(registers.getRegisterCell(index3));

    float result = reg2Value + reg3Value;

    registers.setRegisterCell(index1, registers.intToHexStr(floatToRegister(result)));
}

// Perform bitwise OR on two registers (s and t) and store in register r
void cu::bitwiseOr_7(int index1, int index2, int index3) {
    if (index1 < 0 || index1 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    if (index2 < 0 || index2 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    if (index3 < 0 || index3 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    int result = registers.getRegisterCell(index2) | registers.getRegisterCell(index3);
    registers.setRegisterCell(index1, registers.intToHexStr(result));
}

// Perform bitwise AND on two registers (s and t) and store in register r
void cu::bitwiseAnd_8(int index1, int index2, int index3) {
    if (index1 < 0 || index1 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    if (index2 < 0 || index2 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    if (index3 < 0 || index3 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    int result = registers.getRegisterCell(index2) & registers.getRegisterCell(index3);
    registers.setRegisterCell(index1, registers.intToHexStr(result));
}

// Perform bitwise XOR on two registers (s and t) and store in register r
void cu::bitwiseXor_9(int index1, int index2, int index3) {
    if (index1 < 0 || index1 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    if (index2 < 0 || index2 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    if (index3 < 0 || index3 >= 16) {
        cout << "Invalid register address\n";
        return;
    }
    int result = registers.getRegisterCell(index2) ^ registers.getRegisterCell(index3);
    registers.setRegisterCell(index1, registers.intToHexStr(result));
}

uint8_t cu::rotateRight(uint8_t value, int positions) {
    positions %= 8; // Ensure positions are within the range of 0-7 for an 8-bit register
    return (value >> positions) | (value << (8 - positions));
}

// Rotate register with x right steps.
void cu::rotateToRight_A(string instructions) {
    uint8_t value = registers.getRegisterCell(registers.hexToInt(instructions[1]));
    value = rotateRight(value, registers.hexToInt(instructions[3]));
    registers.setRegisterCell(registers.hexToInt(instructions[1]), registers.intToHexStr(value));
}

// Jump to location in memory if r value equal to register 0 value.
bool cu::jumpInMem_B(string instructions, int &counter){
    int reg_num = registers.hexToInt(instructions[1]);
    if((reg_num > 255) || (reg_num < 0)){ // Making sure of indexes values.
        cout << "Invalid indexes (indexes needs to be from 0 to 255)\n";
         return false;
    }
    if(registers.getRegisterCell(reg_num) == registers.getRegisterCell(0)){ // Doing jumpInMem_B operation.
        string address_hex = instructions.substr(2, 2);
        qInfo() << address_hex << "\n";
        qInfo() << memory.hexToInt(instructions.substr(2, 2)) << "\n";
        counter = memory.hexToInt(instructions.substr(2, 2));
         return true;
    }
    else{
         return false;
    }
}

// Jump to location in memory if r value bigger than register 0 value in two's complement.
bool cu::jumpInMem_D(string instructions, int &counter){
    int reg_num = registers.hexToInt(instructions[1]);
    if((reg_num > 255) || (reg_num < 0)){ // Making sure of indexes values.
        cout << "Invalid indexes (indexes needs to be from 0 to 255)\n";
        return false;
    }
    int8_t reg0Value = static_cast<int8_t>(registers.getRegisterCell(0));
    int8_t reg1Value = static_cast<int8_t>(registers.getRegisterCell(reg_num));
    if(reg1Value > reg0Value){ // Doing jumpInMem_D operation.
        counter = memory.hexToInt(instructions.substr(2, 2));
         return true;
    }
    else{
         return false;
    }
}

Register cu::returnRegisters() const {
    return registers;
}
Memory cu::returnMemory() const {
    return memory;
}
