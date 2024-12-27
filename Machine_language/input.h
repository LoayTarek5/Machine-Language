#ifndef INPUT_H
#define INPUT_H
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
using namespace std;
class Input{
private:
    vector<uint8_t> input; // Input as 8-bit values (1 byte per location)
    vector<string> data;

public:
    Input();
    // Function to convert hex string to an integer
    uint16_t hexToInt(const string& hexStr);
    // Function to check if a string contains only valid hex digits (0-9, A-F, a-f)
    bool isHex(const string& str);

    void takeInputStr(const string& inp);

    const vector<uint8_t>& returnInput() const;

    vector <string> returnInputData() const;
    // To read data from file
    void takeInputFile(string filename);

    void printInput() const;

    void clearInput();
};

#endif //INPUT_H
