#ifndef MEMORY_H
#define MEMORY_H
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

class Memory {
private:
    vector<uint8_t> memory; // Memory storage as 8-bit values (1 byte per location)

public:
    Memory(); // Constructor

    void takeMemory(vector<uint8_t> inp);

    // Function to get the value of a specific memory cell
    int getMemoryCell(int address) const;

    // Function to convert an integer to a hex string
    string intToHexStr(int value);

    // Function to convert a hex string to an integer
    uint16_t hexToInt(const string& hexStr);

    // Function to check if a string contains only valid hex digits (0-9, A-F, a-f)
    bool isHex(const string& str);

    // Function to set the value of a specific memory cell
    void setMemoryCell(int address, string value);

    // Function to print the memory content (for debugging)
    void printMemory() const;

    const vector<uint8_t>& returnMemory() const;

    void clearMemory();
};

#endif // MEMORY_H
