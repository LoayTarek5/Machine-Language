#ifndef REGISTERS_H
#define REGISTERS_H
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
using namespace std;

class Register{
private:
    vector<uint8_t> registers; // Register storage as 8-bit values (1 byte per location)
public:
    Register();

    // Function to get the value of a specific register cell
    int getRegisterCell(int address) const;

    string intToHexStr(int value);

    // Function to convert hex string to an integer
    uint16_t hexToInt(const string& hexStr);

    uint16_t hexToInt(const char& hexStr);

    // Function to check if a string contains only valid hex digits (0-9, A-F, a-f)
    bool isHex(const string& str);

    // Function to set the value of a specific register cell
    void setRegisterCell(int address, string value);

    void printRegisters() const;

    const vector<uint8_t>& returnRegisters() const;

    void clearRegisters();
};
#endif //REGISTERS_H
