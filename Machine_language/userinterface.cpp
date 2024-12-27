#include "UserInterface.h"
void UI::fetch(cu &cc)
{
    controlUnit = cc;
}

// Execute a basic operation based on command
bool UI::executeCommand(int &address, string instr) {
    if (instr[0] == '1') {
        controlUnit.copyMemToReg_1(instr);
    }
    else if (instr[0] == '2') {
        controlUnit.copyStrToReg_2(instr);
    }
    else if (instr[0] == '3') {
        controlUnit.copyRegToMem_3(instr);
    }
    else if (instr[0] == '4') {
        controlUnit.copyRegToReg_4(instr);
    }
    else if(instr[0] == '5'){
        controlUnit.addTwoComplement_5(reg.hexToInt(instr[1]), reg.hexToInt(instr[2]), reg.hexToInt(instr[3]));
    }
    else if(instr[0] == '6'){
        controlUnit.addIEEEFloatingPoint_6(reg.hexToInt(instr[1]), reg.hexToInt(instr[2]), reg.hexToInt(instr[3]));
    }
    else if(instr[0] == '7'){
        controlUnit.bitwiseOr_7(reg.hexToInt(instr[1]), reg.hexToInt(instr[2]), reg.hexToInt(instr[3]));
    }
    else if(instr[0] == '8'){
        controlUnit.bitwiseAnd_8(reg.hexToInt(instr[1]), reg.hexToInt(instr[2]), reg.hexToInt(instr[3]));
    }
    else if(instr[0] == '9'){
        controlUnit.bitwiseXor_9(reg.hexToInt(instr[1]), reg.hexToInt(instr[2]), reg.hexToInt(instr[3]));
    }
    else if(instr[0] == 'A' || instr[0] == 'a'){
        controlUnit.rotateToRight_A(instr);
    }
    else if(instr[0] == 'B' || instr[0] == 'b'){
        jump = controlUnit.jumpInMem_B(instr, address);
    }
    else if(instr[0] == 'C' || instr[0] == 'c'){
        c000 = true;
    }
    else if(instr[0] == 'D' || instr[0] == 'd'){
        jump = controlUnit.jumpInMem_D(instr, address);
    }
    else {
        return false;
    }
    return jump;
}

bool UI::returnC000(){
    return c000;
}

void UI::setC000(bool value){
    c000 = value;
}

Register UI::returnREG() const {
    return controlUnit.returnRegisters();
}

Memory UI::returnMEM() const {
    return controlUnit.returnMemory();
}
