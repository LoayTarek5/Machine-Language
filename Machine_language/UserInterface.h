#ifndef USERINTERFACE_H
#define USERINTERFACE_H
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
#include "Registers.h"
#include "Memory.h"
#include "ControlUnit.h"
#include <bits/stdc++.h>
using namespace std;

class UI {
private:
    Register reg;
    cu controlUnit;
    bool c000 = false;
    bool jump = false;
public:


    void fetch(cu &cc);

    // Execute a basic operation based on command
    bool executeCommand(int &address, string instr);

    bool returnC000();

    void setC000(bool value);

    Register returnREG() const;

    Memory returnMEM() const;

};
#endif //USERINTERFACE_H
