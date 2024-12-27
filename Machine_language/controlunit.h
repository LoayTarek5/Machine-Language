#ifndef CONTROLUNIT_H
#define CONTROLUNIT_H
#include <QApplication>
#include <QMainWindow>
#include <QTableWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QGroupBox>
#include <QHeaderView>
#include <QTextEdit>
#include <QFrame>
#include <QCheckBox>
#include <QSpacerItem>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QRegExp>
#include <QThread>
#include <QStyleFactory>
#include <QTimer>
#include <QScrollArea>
#include <QSpinBox>

#include <bits/stdc++.h>

#include "Memory.h"
#include "Registers.h"
using namespace std;
class cu {
private:
    Register registers;
    Memory memory;
public:
    void fetch(Memory &mem, Register &regi);

    void fetch(Memory &mem);

    // LOAD the register R with the value found in the memory cell whose address is XY.
    void copyMemToReg_1(string instructions);

    //  LOAD the register R with the bit pattern XY.
    void copyStrToReg_2(string instructions);

    void copyRegToMem_3(string instructions);

    // 40RS Copy the content found in register R to register S.
    void copyRegToReg_4(string instructions);

    // Add in two's complement notation.
    void addTwoComplement_5(int index1, int index2, int index3);

    double registerToFloat(uint8_t hexnum);

    uint8_t floatToRegister(double value);

    // Add in floating point notation.
    void addIEEEFloatingPoint_6(int index1, int index2, int index3);

    // Perform bitwise OR on two registers (s and t) and store in register r
    void bitwiseOr_7(int index1, int index2, int index3);

    // Perform bitwise AND on two registers (s and t) and store in register r
    void bitwiseAnd_8(int index1, int index2, int index3);

    // Perform bitwise XOR on two registers (s and t) and store in register r
    void bitwiseXor_9(int index1, int index2, int index3);

    uint8_t rotateRight(uint8_t value, int positions);

    // Rotate register with x right steps.
    void rotateToRight_A(string instructions);

    // Jump to location in memory if r value equal to register 0 value.
    bool jumpInMem_B(string instructions, int &counter);

    // Jump to location in memory if r value bigger than register 0 value in two's complement.
    bool jumpInMem_D(string instructions, int &counter);

    Register returnRegisters() const;

    Memory returnMemory() const;
};
#endif //CONTROLUNIT_H
