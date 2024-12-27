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
#include "memory.h"
#include "registers.h"
#include "controlunit.h"
#include "userinterface.h"
#include "input.h"

#include <bits/stdc++.h>
using namespace std;

// Memory Table Widget
class MemoryTable : public QTableWidget {
public:
    MemoryTable() {
        setColumnCount(3);
        setRowCount(256);
        setHorizontalHeaderLabels({"Address", "Binary", "Hex"});
        verticalHeader()->setVisible(false);

        // Make Columns and Rows stretch to fit the table's width
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Populate table with non-null items
        for(int i = 0; i < 256; i++) {
            // Create items with parent widget to ensure proper memory management
            QTableWidgetItem* addressItem = new QTableWidgetItem(QString("%1").arg(i, 2, 16, QChar('0')).toUpper());
            QTableWidgetItem* binaryItem = new QTableWidgetItem("00000000");
            QTableWidgetItem* hexItem = new QTableWidgetItem("00");

            // Make address column read-only
            addressItem->setFlags(addressItem->flags() & ~Qt::ItemIsEditable);

            // Set items
            setItem(i, 0, addressItem);
            setItem(i, 1, binaryItem);
            setItem(i, 2, hexItem);
        }

        // Dark theme styling
        setStyleSheet(
            "QTableWidget { "
            "   background-color: #1e1e1e; "
            "   gridline-color: #404040; "
            "   color: #d4d4d4; "
            "   border: 1px solid #404040; "
            "} "
            );
        /*
        setStyleSheet(
            "QTableWidget { "
            "   background-color: #1e1e1e; "
            "   gridline-color: #404040; "
            "   color: #d4d4d4; "
            "   border: 1px solid #404040; "
            "} "
            "QTableWidget::item { "
            "   background-color: #1e1e1e; "
            "} "
            "QTableWidget::item:selected { "
            "   background-color: #264f78; "
            "} "
            "QHeaderView::section { "
            "   background-color: #252526; "
            "   color: #d4d4d4; "
            "   padding: 4px; "
            "   border: 1px solid #404040; "
            "}"
            );*/
    }
    // Add method to safely update cell contents
    void updateCell(int row, int column, const QString& value) {
        if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount()) {
            QTableWidgetItem* item = this->item(row, column);
            if (!item) {
                item = new QTableWidgetItem();
                setItem(row, column, item);
            }
            item->setText(value);
        }
    }
    void printHexContent() {
        for(int row = 0; row < rowCount(); row++) {
            QTableWidgetItem* hexItem = item(row, 2);  // Column 2 is Hex
            if (hexItem) {
                qDebug() << QString("Address: %1, Hex: %2")
                .arg(item(row, 0)->text())
                    .arg(hexItem->text());
            }
        }
    }

};

// Register Table Widget
class RegisterTable : public QTableWidget {
public:
    RegisterTable() {
        setColumnCount(3);
        setRowCount(16);
        setHorizontalHeaderLabels({"Register", "Binary", "Hex"});
        verticalHeader()->setVisible(false);

        // Make Columns and Rows stretch to fit the table's width
        horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        // Populate table
        for(int i = 0; i < 16; i++) {

            // Create items with parent widget to ensure proper Registers
            QTableWidgetItem* reg = new QTableWidgetItem(QString("%1").arg(i, 1, 16).toUpper());
            QTableWidgetItem* binaryItem = new QTableWidgetItem("00000000");
            QTableWidgetItem* hexItem = new QTableWidgetItem("00");

            // Make Register column read-only
            reg->setFlags(reg->flags() & ~Qt::ItemIsEditable);

            // Set items
            setItem(i, 0, reg);
            setItem(i, 1, binaryItem);
            setItem(i, 2, hexItem);
        }

        // Set register 3 content (highlighted in pink)
        // QTableWidgetItem* highlightedItem = new QTableWidgetItem("01010000");
        // highlightedItem->setBackground(QColor(255, 192, 203));
        // setItem(3, 1, highlightedItem);
        // setItem(3, 2, new QTableWidgetItem("50"));

        // Dark theme styling
        setStyleSheet(
            "QTableWidget { "
            "   background-color: #1e1e1e; "
            "   gridline-color: #404040; "
            "   color: #d4d4d4; "
            "   border: 1px solid #404040; "
            "} "
            );
    }
    // Add method to safely update cell contents
    void updateCell(int row, int column, const QString& value) {
        if (row >= 0 && row < rowCount() && column >= 0 && column < columnCount()) {
            QTableWidgetItem* item = this->item(row, column);
            if (!item) {
                item = new QTableWidgetItem();
                setItem(row, column, item);
            }
            item->setText(value);
        }
    }
};

// Control Panel Widget
class ControlPanel : public QWidget {
private:
    Input input;
public:
    QPushButton* fetchBtn;
    QPushButton* decodeBtn;
    QLineEdit* instructionInput;
    QPushButton* runHaltBtn;
    QPushButton* terminateBtn;
    QPushButton* executeBtn;
    QLineEdit* yValue;
    QLabel* description;
    QLabel* instrFormat;
    QLabel* errorLabel;
    QLineEdit* pcValue;
    QLineEdit* irValue;
    QLineEdit* opcodeValue;
    QLineEdit* rValue;
    QLineEdit* xValue;
    QSlider* delaySlider;

    ControlPanel() : QWidget() {
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->setSpacing(5);

        // Initialize all member pointers
        fetchBtn = new QPushButton("Fetch");
        decodeBtn = new QPushButton("Decode");
        pcValue = new QLineEdit("0");
        irValue = new QLineEdit();
        opcodeValue = new QLineEdit();
        rValue = new QLineEdit();
        xValue = new QLineEdit();
        yValue = new QLineEdit();
        description = new QLabel();
        instrFormat = new QLabel();
        instructionInput = new QLineEdit();
        errorLabel = new QLabel();
        errorLabel->setStyleSheet("color: red;");

        // Program Counter section
        QLabel* pcLabel = new QLabel("Program Counter:");
        pcValue->setFixedHeight(20);

        // Instruction Input section
        QLabel* instrLabel = new QLabel("Enter Instructions:");
        instructionInput->setPlaceholderText("Enter 4-digit hex instruction (e.g., 1234)");

        // Opcode section
        QHBoxLayout* opcodeLayout = new QHBoxLayout();
        QLabel* opcodeLabel = new QLabel("Opcode:");
        QLabel* rLabel = new QLabel("R");
        opcodeValue->setFixedWidth(30);
        rValue->setFixedWidth(30);
        opcodeLayout->addWidget(opcodeLabel);
        opcodeLayout->addWidget(opcodeValue);
        opcodeLayout->addWidget(rLabel);
        opcodeLayout->addWidget(rValue);
        opcodeLayout->addStretch();


        // X and Y values section
        QHBoxLayout* xyLayout = new QHBoxLayout();
        QLabel* xLabel = new QLabel("X");
        QLabel* yLabel = new QLabel("Y");
        xValue->setFixedWidth(30);
        yValue->setFixedWidth(30);
        xyLayout->addWidget(xLabel);
        xyLayout->addWidget(xValue);
        xyLayout->addWidget(yLabel);
        xyLayout->addWidget(yValue);
        xyLayout->addStretch();


        // Execute section
        executeBtn = new QPushButton("Execute");

        // Control section
        QGroupBox* controlGroup = new QGroupBox("Control");
        QVBoxLayout* controlLayout = new QVBoxLayout();
        // QPushButton* runCycleBtn = new QPushButton("Run one cycle");
        runHaltBtn = new QPushButton("Run until halt");
        terminateBtn = new QPushButton("TERMINATE");


        QLabel* playSpeedLabel = new QLabel("Play Speed");

        // Create the delay slider with adjusted values
        // Using milliseconds internally (100ms = 0.1s, 3000ms = 3s)
        delaySlider = new QSlider(Qt::Horizontal);
        delaySlider->setMinimum(100);  // 0.1s
        delaySlider->setMaximum(3000); // 3.0s
        delaySlider->setValue(100);     // Default value is 0.1s
        delaySlider->setSingleStep(500); // 0.5s steps

        // Create a list of allowed values in milliseconds
        QList<int> allowedValues = {100, 500, 1000, 1500, 2000, 2500, 3000};

        // Create marks for allowed values
        delaySlider->setTickPosition(QSlider::TicksBelow);
        delaySlider->setTickInterval(500);

        // Create the number line layout
        QHBoxLayout* numberLineLayout = new QHBoxLayout();
        numberLineLayout->setSpacing(0);

        // Add labels for each allowed value
        for (int value : allowedValues) {
            float seconds = value / 1000.0f;
            QLabel* numberLabel = new QLabel(QString::number(seconds, 'f', 1) + "s");
            numberLabel->setAlignment(Qt::AlignCenter);
            numberLineLayout->addWidget(numberLabel, 1, Qt::AlignCenter);
        }

        QString sliderStyle =
            "QSlider::groove:horizontal {"
            "    border: 1px solid #404040;"
            "    height: 8px;"
            "    background: #2d2d2d;"
            "    margin: 2px 0;"
            "}"
            "QSlider::handle:horizontal {"
            "    background: #0e639c;"
            "    border: 1px solid #404040;"
            "    width: 15px;"
            "    margin: -2px 0;"
            "    border-radius: 3px;"
            "}"
            "QSlider::handle:horizontal:hover {"
            "    background: #1177bb;"
            "}"
            "QSlider::add-page:horizontal {"
            "    background: #2d2d2d;"
            "}"
            "QSlider::sub-page:horizontal {"
            "    background: #0e639c;"
            "}"
            "QSlider::tick-mark:horizontal {"
            "    background: #404040;"
            "    width: 1px;"
            "    height: 8px;"
            "    margin: 2px 0;"
            "}"
            "QSlider::tick-label:horizontal {"
            "    color: #d4d4d4;"
            "    font-size: 10px;"
            "}";

        delaySlider->setStyleSheet(sliderStyle);


        // controlLayout->addWidget(runCycleBtn);
        controlLayout->addWidget(runHaltBtn);
        controlLayout->addWidget(terminateBtn);

        // Slider
        controlLayout->addWidget(playSpeedLabel);
        controlLayout->addWidget(delaySlider);
        controlGroup->setLayout(controlLayout);
        controlLayout->addLayout(numberLineLayout);

        // Add all components to main layout
        layout->addWidget(errorLabel);
        layout->addWidget(pcLabel);
        layout->addWidget(pcValue);
        layout->addWidget(fetchBtn);
        layout->addWidget(instrLabel);

        layout->addWidget(instructionInput);
        layout->addWidget(decodeBtn);
        layout->addLayout(opcodeLayout);
        layout->addWidget(instrFormat);
        layout->addLayout(xyLayout);
        layout->addWidget(description);
        layout->addWidget(executeBtn);
        layout->addWidget(controlGroup);

        layout->addStretch();


        // Set initial state for read-only fields
        opcodeValue->setReadOnly(true);
        rValue->setReadOnly(true);
        xValue->setReadOnly(true);
        yValue->setReadOnly(true);

        // Dark theme styling for input fields
        QString inputStyle =
            "QLineEdit { "
            "   background-color: #1e1e1e; "
            "   color: #d4d4d4; "
            "   border: 1px solid #404040; "
            "   padding: 2px; "
            "} "
            "QLineEdit:disabled { "
            "   background-color: #2d2d2d; "
            "   color: #808080; "
            "}";

        // Dark theme styling for buttons
        QString buttonStyle =
            "QPushButton { "
            "   background-color: #0e639c; "
            "   color: #ffffff; "
            "   border: none; "
            "   padding: 6px; "
            "   border-radius: 3px; "
            "} "
            "QPushButton:hover { "
            "   background-color: #1177bb; "
            "} "
            "QPushButton:pressed { "
            "   background-color: #094771; "
            "} "
            "QPushButton:disabled { "
            "   background-color: #2d2d2d; "
            "   color: #808080; "
            "}";

        // Dark theme styling for labels
        QString labelStyle =
            "QLabel { "
            "   color: #d4d4d4; "
            "}";

        // Apply styles to components
        setStyleSheet(inputStyle + buttonStyle + labelStyle);

        // Set background color for the entire panel
        QPalette pal = palette();
        pal.setColor(QPalette::Window, QColor("#252526"));
        setAutoFillBackground(true);
        setPalette(pal);

        // Update error label style
        errorLabel->setStyleSheet("color: #f48771;"); // Reddish color for errors
        // Connect slider value changed signal
        connect(delaySlider, &QSlider::valueChanged, [=](int value) {
            // Snap to nearest 0.5s interval
            int interval = 500; // 0.5s in milliseconds
            int snappedValue = ((value + interval/2) / interval) * interval;

            // Ensure we stay within bounds
            if (snappedValue < 100) snappedValue = 100;
            if (snappedValue > 3000) snappedValue = 3000;

            // Only update if value is different
            if (delaySlider->value() != snappedValue) {
                delaySlider->setValue(snappedValue);
            }
        });

    }
    bool validateInstruction(const QString& instruction) {
        string inpt = instruction.toStdString();
        input.takeInputStr(inpt);
        if (instruction.length() != 4) {
            errorLabel->setText("Error: Instruction must be 4 digits");
            return false;
        }

        if (!input.isHex(inpt)) {
            errorLabel->setText("Error: Invalid hexadecimal digits");
            return false;
        }


        // Check if all characters are valid hexadecimal
        QRegExp hexMatcher("^[0-9A-Fa-f]{4}$");
        if (!hexMatcher.exactMatch(instruction)) {
            errorLabel->setText("Error: Invalid hexadecimal digits");
            return false;
        }
        errorLabel->clear();
        return true;
    }

};

class MachineLangSimulator : public QMainWindow {
public:
    MachineLangSimulator() : QMainWindow() {
        setWindowTitle("Machine Language Simulator");

        QWidget *centralWidget = new QWidget;
        QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
        mainLayout->setSpacing(10);

        // Memory Section
        QGroupBox *memoryGroup = new QGroupBox("Main Memory");
        QVBoxLayout *memoryLayout = new QVBoxLayout;
        memoryTable = new MemoryTable();

        // Memory Buttons
        QHBoxLayout *memBtnLayout = new QHBoxLayout;
        QPushButton *batchAddBtn = new QPushButton("Add File Instruction");
        QPushButton *clearMemBtn = new QPushButton("Clear Memory");
        memBtnLayout->addWidget(batchAddBtn);
        memBtnLayout->addWidget(clearMemBtn);

        QLabel *authorLabel = new QLabel("FCAI Students under supervisor Dr.Mohammed El-Ramly CS-213 ");

        memoryLayout->addWidget(memoryTable);
        memoryLayout->addLayout(memBtnLayout);
        memoryLayout->addWidget(authorLabel);
        memoryGroup->setLayout(memoryLayout);

        // CPU Section
        QVBoxLayout *cpuLayout = new QVBoxLayout;

        // Register Section
        QGroupBox *cpuGroup = new QGroupBox("Central Processing Unit");
        QVBoxLayout *registerLayout = new QVBoxLayout;
        registerTable = new RegisterTable();
        QPushButton *clearRegBtn = new QPushButton("Clear Register");
        registerLayout->addWidget(registerTable);
        registerLayout->addWidget(clearRegBtn);
        cpuGroup->setLayout(registerLayout);

        // Screen Section
        QGroupBox *screenGroup = new QGroupBox("Screen");
        QVBoxLayout *screenLayout = new QVBoxLayout;

        // Create the screen display area
        QScrollArea *screenScrollArea = new QScrollArea();
        screenScrollArea->setWidgetResizable(true);
        screenScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        screenScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        screenScrollArea->setStyleSheet(
            "screenScrollArea {"
            "   color: #d4d4d4;"
            "   font-size: 10px;"
            "   min-height: 10px;"  // Set minimum height
            "}"
            );
        // Create the screen display area
        screenLabel = new QLabel("");
        screenLabel->setStyleSheet(
            "QLabel {"
            "   color: #d4d4d4;"
            "   font-size: 14px;"
            "   min-height: 20px;"  // Set minimum height
            "}"
            );

        screenLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        screenLabel->setWordWrap(true);  // Allow text to wrap

        screenScrollArea->setWidget(screenLabel);
        screenLayout->addWidget(screenScrollArea);

        // screenLayout->addWidget(screenLabel);
        // Add some spacing
        screenLayout->addSpacing(10);

        QHBoxLayout* toggleLayout = new QHBoxLayout;
        displayModeBtn = new QPushButton("Switch to ASCII");
        displayModeBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: #0e639c;"
            "   color: white;"
            "   padding: 6px;"
            "   border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #1177bb;"
            "}"
            );
        screenLayout->addLayout(toggleLayout);

        // Create and style the clear button
        clearScreenBtn = new QPushButton("Clear");
        clearScreenBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: #0e639c;"
            "   color: white;"
            "   padding: 6px;"
            "   border-radius: 3px;"
            "   min-width: 30px;"
            "   max-width: 30px;"  // Limit maximum width
            "}"
            "QPushButton:hover {"
            "   background-color: #1177bb;"
            "}"
            );
        clearScreenBtn->setFixedWidth(30);

        // Create a container for the button and align it to the right
        QHBoxLayout *buttonLayout = new QHBoxLayout;
        displayModeBtn = new QPushButton("Switch to ASCII");
        displayModeBtn->setStyleSheet(
            "QPushButton {"
            "   background-color: #0e639c;"
            "   color: white;"
            "   padding: 6px;"
            "   border-radius: 3px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #1177bb;"
            "}"
            );

        screenGroup->setLayout(screenLayout);
        // Add the display mode button to the left
        buttonLayout->addWidget(displayModeBtn);
        // Add stretch to push Clear button to the right
        buttonLayout->addStretch();
        // Add the existing Clear button
        buttonLayout->addWidget(clearScreenBtn);

        // Add the button layout to the screen layout
        screenLayout->addLayout(buttonLayout);
        // Set minimum size for the screen group
        screenGroup->setMinimumHeight(30);
        screenGroup->setMaximumHeight(150);

        cpuLayout->addWidget(cpuGroup);
        cpuLayout->addWidget(screenGroup);

        controlPanel = new ControlPanel();
        // Add all sections to main layout
        mainLayout->addWidget(memoryGroup, 1);
        mainLayout->addLayout(cpuLayout, 1);
        mainLayout->addWidget(controlPanel, 1);

        setCentralWidget(centralWidget);

        // Set the main window background color
        QPalette pal = palette();
        pal.setColor(QPalette::Window, QColor("#1e1e1e"));
        setPalette(pal);

        // Dark theme styling for group boxes
        QString groupBoxStyle =
            "QGroupBox { "
            "   background-color: #252526; "
            "   border: 1px solid #404040; "
            "   color: #d4d4d4; "
            "   margin-top: 1.5ex; "
            "} "
            "QGroupBox::title { "
            "   subcontrol-origin: margin; "
            "   subcontrol-position: top left; "
            "   padding: 0 3px; "
            "   color: #d4d4d4; "
            "}";

        // Apply the style to the entire application
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setStyleSheet(
            groupBoxStyle +
            "QMainWindow { background-color: #1e1e1e; }"
            "QWidget { color: #d4d4d4; }"
            "QStatusBar { background-color: #007acc; color: white; }"
            "QMessageBox { background-color: #252526; }"
            "QMessageBox QLabel { color: #d4d4d4; }"
            "QMessageBox QPushButton { background-color: #0e639c; color: white; padding: 6px; border-radius: 3px; }"
            "QMessageBox QPushButton:hover { background-color: #1177bb; }"
            );

        // setFixedSize(800, 600);
        // Connect the UI components to the logic
        connect(batchAddBtn, &QPushButton::clicked, this, &MachineLangSimulator::onBatchAddClicked);
        connect(clearMemBtn, &QPushButton::clicked, this, &MachineLangSimulator::clearMemory);
        connect(clearRegBtn, &QPushButton::clicked, this, &MachineLangSimulator::onClearRegClicked);
        connect(controlPanel->fetchBtn, &QPushButton::clicked, this, &MachineLangSimulator::onFetchClicked);
        connect(controlPanel->decodeBtn, &QPushButton::clicked, this, &MachineLangSimulator::onDecodeClicked);
        connect(controlPanel->executeBtn, &QPushButton::clicked, this, &MachineLangSimulator::execute);
        connect(controlPanel->runHaltBtn, &QPushButton::clicked, this, &MachineLangSimulator::executeUntilHalt);
        connect(controlPanel->terminateBtn, &QPushButton::clicked, this, &MachineLangSimulator::onTerminateBtn);
        connect(clearScreenBtn, &QPushButton::clicked, this, &MachineLangSimulator::clearScreen);
        // Connect the button click to a slot
        connect(displayModeBtn, &QPushButton::clicked, this, [this]() {
            isAsciiMode = !isAsciiMode;  // Toggle the mode
            displayModeBtn->setText(isAsciiMode ? "Switch to HEX" : "Switch to ASCII");
        });

    }

private:
    MemoryTable* memoryTable;
    RegisterTable* registerTable;
    ControlPanel* controlPanel;

    Register registerFile;
    Memory memoryFile;
    cu controlUnit;
    Input inputProcessor;
    UI ui;

    QLabel* screenLabel;
    QPushButton* clearScreenBtn;
    QPushButton* displayModeBtn;
    bool isAsciiMode = false;  // Track the current display mode

    // QString output = "";

    // Termination Flag
    bool isTerminated = false;

    // a member variable to track the last used memory address
    int lastUsedAddress = 10;
    // member variables to track previous states
    int prevMemoryCell = -1;
    int prevRegisterCell = -1;

    struct CPUState {
        uint16_t PC = 10;
        uint16_t PC_2 = 10;
        uint8_t IR = 0;
        string currentInstruction;
        bool fetched = false;
        bool decoded = false;
    } cpuState;

    void clearScreen() {
        screenLabel->setText("");
        statusBar()->showMessage("Screen cleared", 2000);
    }

    void updateAndHighlightMemory(const vector<uint8_t>& memoryData, int currentPC, int& prevMemoryCell) {
        // Clear previous highlighting if it exists
        if (prevMemoryCell >= 0) {
            memoryTable->item(prevMemoryCell, 1)->setBackground(QColor("#1e1e1e"));
            memoryTable->item(prevMemoryCell, 2)->setBackground(QColor("#1e1e1e"));
            memoryTable->item(prevMemoryCell-1, 1)->setBackground(QColor("#1e1e1e"));
            memoryTable->item(prevMemoryCell-1, 2)->setBackground(QColor("#1e1e1e"));
        }

        // Update and highlight memory cells
        for (int i = 0; i < memoryData.size() && i < 256; ++i) {
            QString hexValue = QString("%1").arg(memoryData[i], 2, 16, QChar('0')).toUpper();
            QString binaryValue = QString("%1").arg(memoryData[i], 8, 2, QChar('0'));

            memoryTable->updateCell(i, 1, binaryValue);
            memoryTable->updateCell(i, 2, hexValue);

            // Highlight current memory cells
            if (i == currentPC || i == currentPC + 1) {
                QColor highlightColor("#1177bb");
                memoryTable->item(i, 1)->setBackground(highlightColor);
                memoryTable->item(i, 2)->setBackground(highlightColor);
                memoryTable->item(i, 1)->setForeground(Qt::white);  // White text for contrast
                memoryTable->item(i, 2)->setForeground(Qt::white);
                prevMemoryCell = i;
            }
        }
    }

    void updateAndHighlightRegisters(const vector<uint8_t>& registers, int registerToHighlight, int& prevRegisterCell) {
        // Clear previous highlighting
        if (prevRegisterCell >= 0) {
            // Reset to dark theme background color
            registerTable->item(prevRegisterCell, 1)->setBackground(QColor("#1e1e1e"));
            registerTable->item(prevRegisterCell, 2)->setBackground(QColor("#1e1e1e"));
        }

        // Update and highlight register cells
        for (int i = 0; i < 16; i++) {
            QString binaryValue = QString("%1").arg(registers[i], 8, 2, QChar('0'));
            QString hexValue = QString("%1").arg(registers[i], 2, 16, QChar('0')).toUpper();

            registerTable->updateCell(i, 1, binaryValue);
            registerTable->updateCell(i, 2, hexValue);

            // Highlight current register if it matches
            if (i == registerToHighlight) {
                QColor highlightColor("#1177bb");
                registerTable->item(i, 1)->setBackground(highlightColor);
                registerTable->item(i, 2)->setBackground(highlightColor);
                registerTable->item(i, 1)->setForeground(Qt::white);  // White text for contrast
                registerTable->item(i, 2)->setForeground(Qt::white);
                prevRegisterCell = i;
            }
        }
    }

    void clearHighlighting(int memoryCell, int registerCell) {
        if (memoryCell >= 0) {
            memoryTable->item(memoryCell, 1)->setBackground(QColor("#1e1e1e"));
            memoryTable->item(memoryCell, 2)->setBackground(QColor("#1e1e1e"));
            memoryTable->item(memoryCell-1, 1)->setBackground(QColor("#1e1e1e"));
            memoryTable->item(memoryCell-1, 2)->setBackground(QColor("#1e1e1e"));
        }
        if (registerCell >= 0) {
            registerTable->item(registerCell, 1)->setBackground(QColor("#1e1e1e"));
            registerTable->item(registerCell, 2)->setBackground(QColor("#1e1e1e"));
        }
    }

    int getRegisterFromInstruction(const string& instruction) {
        if (instruction.length() >= 4) {
            return std::stoi(instruction.substr(1, 1), nullptr, 16);
        }
        return -1;
    }

    void updateDisplays(const vector<uint8_t>& memoryData, const vector<uint8_t>& registers, bool withHighlighting) {
        // Basic update without highlighting
        for (int i = 0; i < memoryData.size() && i < 256; ++i) {
            QString hexValue = QString("%1").arg(memoryData[i], 2, 16, QChar('0')).toUpper();
            QString binaryValue = QString("%1").arg(memoryData[i], 8, 2, QChar('0'));
            memoryTable->updateCell(i, 1, binaryValue);
            memoryTable->updateCell(i, 2, hexValue);
        }

        for (int i = 0; i < 16; i++) {
            QString binaryValue = QString("%1").arg(registers[i], 8, 2, QChar('0'));
            QString hexValue = QString("%1").arg(registers[i], 2, 16, QChar('0')).toUpper();
            registerTable->updateCell(i, 1, binaryValue);
            registerTable->updateCell(i, 2, hexValue);
        }
    }

    void onFetchClicked()
    {
        // Get the program counter value from the UI
        QString pcStr = controlPanel->pcValue->text();
        bool valid;
        int pcValue = pcStr.toInt(&valid);

        if (valid) {
            // Update CPU state with the program counter value
            cpuState.PC = pcValue;
            cpuState.PC_2 = pcValue;
            lastUsedAddress = pcValue;
            // Set fetched flag to true
            cpuState.fetched = true;

            // Optional: You might want to update the UI to reflect the fetch operation
            // For example, disable the fetch button until decode is complete
            // controlPanel->fetchBtn->setEnabled(false);
            // controlPanel->decodeBtn->setEnabled(true);

            statusBar()->showMessage("Fetched Program Counter", 2000);

        } else {
            // Handle invalid program counter value
            controlPanel->errorLabel->setText("Error: Invalid program counter value");
        }
    }

    void onBatchAddClicked() {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Machine Code File"), "",
                                                        tr("Text Files (*.txt);;All Files (*)"));

        if (fileName.isEmpty())
            return;

        try {

            // Convert QString to string for the input processor
            std::string stdFileName = fileName.toStdString();

            // Process the input file
            inputProcessor.clearInput();
            inputProcessor.takeInputFile(stdFileName);

            // Get the processed memory data
            const vector<uint8_t>& memoryData = inputProcessor.returnInput();
            vector<string> inputData = inputProcessor.returnInputData();
            qInfo() << inputData.size() << "\n";

            // Calculate last used address based on number of instructions
            int startAddress = cpuState.PC;
            int currentAddress = startAddress;

            // Update the memory table display
            for (int i = 0; i < memoryData.size() && currentAddress < 256; ++i) {

                currentAddress = startAddress + i;
                if (currentAddress >= 256) break; // Prevent buffer overflow

                // Update memory file using the value from memoryData at index i
                QString hexValue = QString("%1").arg(memoryData[i], 2, 16, QChar('0')).toUpper();
                memoryFile.setMemoryCell(currentAddress, hexValue.toStdString());

                // Update memory table display
                QString binaryValue = QString("%1").arg(memoryData[i], 8, 2, QChar('0'));

                memoryTable->updateCell(currentAddress, 1, binaryValue);
                memoryTable->updateCell(currentAddress, 2, hexValue);

            }

            lastUsedAddress = (inputData.size() * 2) + startAddress;
            // Update CPU state to start from the last used address
            cpuState.PC = lastUsedAddress;

            // Show success message
            QMessageBox::information(this, tr("Success"),
                                     tr("Machine code loaded successfully."));

        } catch (const std::exception& e) {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Failed to load machine code: %1").arg(e.what()));
        }

    }

    void onDecodeClicked() {

        QString instruction = controlPanel->instructionInput->text().toUpper();

        // Validate the instruction
        if (!controlPanel->validateInstruction(instruction)) {
            return;
        }

        // Check if there's enough space in memory
        if (cpuState.PC >= 254) {  // 254 because we need 2 bytes for the instruction
            QMessageBox::warning(this, tr("Memory Full"),
                                 tr("Cannot add more instructions. Memory is full."));
            return;
        }

        // Parse the instruction
        char opcode = instruction[0].toLatin1();
        char r = instruction[1].toLatin1();
        char x = instruction[2].toLatin1();
        char y = instruction[3].toLatin1();


        // Store in memory at current PC
        QString firstByte = instruction.mid(0, 2);
        QString secondByte = instruction.mid(2, 2);

        // Update memory content
        memoryFile.setMemoryCell(cpuState.PC, firstByte.toStdString());
        memoryFile.setMemoryCell(cpuState.PC + 1, secondByte.toStdString());


        // Update memory table
        memoryTable->updateCell(cpuState.PC, 2, firstByte);    // Hex view
        memoryTable->updateCell(cpuState.PC + 1, 2, secondByte);

        // Update binary view
        memoryTable->updateCell(cpuState.PC, 1, QString("%1").arg(firstByte.toInt(nullptr, 16), 8, 2, QChar('0')));
        memoryTable->updateCell(cpuState.PC + 1, 1, QString("%1").arg(secondByte.toInt(nullptr, 16), 8, 2, QChar('0')));

        // Update the control panel displays
        controlPanel->opcodeValue->setText(QString(opcode));
        controlPanel->rValue->setText(QString(r));
        controlPanel->xValue->setText(QString(x));
        controlPanel->yValue->setText(QString(y));

        // Set instruction format label
        QString format;
        if (opcode >= '1' && opcode <= '9') {
            format = QString("%1RXY").arg(opcode);
        } else if (opcode == 'A' || opcode == 'a') {
            format = "ARXY";
        } else if (opcode == 'B' || opcode == 'b') {
            format = "BRXY";
        } else if (opcode == 'C' || opcode == 'c') {
            format = "C000";
        } else if (opcode == 'D' || opcode == 'd') {
            format = "DRXY";
        }

        controlPanel->instrFormat->setText(format);

        // Update description
        QString description = getInstructionDescription(opcode, r, x, y);
        controlPanel->description->setText(description);

        cpuState.decoded = true;
        statusBar()->showMessage("Instruction decoded and stored in memory", 2000);

        // Increment PC by 2 since each instruction takes 2 bytes
        cpuState.PC += 2;
        lastUsedAddress = cpuState.PC;  // Update the last used address

    }

    void clearMemory() {
        for (int i = 0; i < 256; i++) {
            memoryTable->item(i, 1)->setText("00000000");  // Binary
            memoryTable->item(i, 2)->setText("00");        // Hex
        }
        memoryFile.clearMemory();
        lastUsedAddress = 10;  // Reset the last used address
        cpuState.PC = 10;      // Reset the program counter
        cpuState.PC_2 = 10;      // Reset the program counter
        cpuState.currentInstruction = "";
        cpuState.fetched = false;
        cpuState.decoded = false;
        statusBar()->showMessage("Memory cleared", 2000);
    }

    void onClearRegClicked() {
        // Clear registers
        registerFile.clearRegisters();
        // Reset CPU state
        lastUsedAddress = 10;
        cpuState.PC = 10;
        cpuState.IR = 10;
        cpuState.PC_2 = 10;      // Reset the program counter
        cpuState.currentInstruction = "";
        cpuState.fetched = false;
        cpuState.decoded = false;
        // Update register table display
        for(int i = 0; i < 16; i++) {
            registerTable->item(i, 1)->setText("00000000");
            registerTable->item(i, 2)->setText("00");
        }

        statusBar()->showMessage("Registers cleared", 2000);
    }

    void onTerminateBtn()
    {
        isTerminated = true;
        statusBar()->showMessage("Program execution stopped", 2000);
    }

    QString getInstructionDescription(char opcode, char r, char x, char y) {
        switch (opcode) {
        case '1':
            return QString("Copy memory location %1%2 to register %3").arg(x).arg(y).arg(r);
        case '2':
            return QString("Load register %1 with bit pattern %2%3").arg(r).arg(x).arg(y);
        case '3':
            return QString("Copy register %1 to memory location %2%3").arg(r).arg(x).arg(y);
        case '4':
            return QString("Copy register %1 to register %2").arg(y).arg(r);
        case '5':
            return QString("Add contents of registers %1 and %2 as two's complement numbers, put result in register %3")
                .arg(x).arg(y).arg(r);
        case '6':
            return QString("Add contents of registers %1 and %2 as floating point numbers, put result in register %3")
                .arg(x).arg(y).arg(r);
        case '7':
            return QString("OR contents of registers %1 and %2, put result in register %3")
                .arg(x).arg(y).arg(r);
        case '8':
            return QString("AND contents of registers %1 and %2, put result in register %3")
                .arg(x).arg(y).arg(r);
        case '9':
            return QString("XOR contents of registers %1 and %2, put result in register %3")
                .arg(x).arg(y).arg(r);
        case 'A':
        case 'a':
            return QString("Rotate register %1 right %2 times").arg(r).arg(x);
        case 'B':
        case 'b':
            return QString("Jump to memory location %1%2 if register %3 contains 0").arg(x).arg(y).arg(r);
        case 'C':
        case 'c':
            return QString("Halt execution");
        case 'D':
        case 'd':
            return QString("Jump to memory location %1%2").arg(x).arg(y);
        default:
            return QString("Unknown instruction");
        }
    }

    void execute() {

        isTerminated = false;

        clearHighlighting(prevMemoryCell, prevRegisterCell);

        // Initialize control unit with current memory and register state
        controlUnit.fetch(memoryFile, registerFile);
        bool reachedHalt = false;
        memoryFile = controlUnit.returnMemory();

        controlUnit.fetch(memoryFile);
        ui.fetch(controlUnit);

        if (!reachedHalt && cpuState.PC_2 < lastUsedAddress) {


            // Get the current instruction (2 bytes)
            string firstByte = memoryFile.intToHexStr(memoryFile.getMemoryCell(cpuState.PC_2));
            string secondByte = memoryFile.intToHexStr(memoryFile.getMemoryCell(cpuState.PC_2 + 1));

            // Combine into 4-digit instruction
            string currentInstruction = firstByte + secondByte;

            // Parse the instruction
            char opcode = currentInstruction[0];
            char r = currentInstruction[1];
            char x = currentInstruction[2];
            char y = currentInstruction[3];
            controlPanel->opcodeValue->setText(QString(opcode));
            controlPanel->rValue->setText(QString(r));
            controlPanel->xValue->setText(QString(x));
            controlPanel->yValue->setText(QString(y));

            if (currentInstruction[0] == '3' && currentInstruction.size() == 3 && currentInstruction[2] == '0') {
                // Get the register number
                int regNum = QString(currentInstruction[1]).toInt(nullptr, 16);
                // Get the register value
                const vector<uint8_t>& registers = registerFile.returnRegisters();
                uint8_t regValue = registers[regNum];

                // Convert to string and update screen

                QString output;
                if (isAsciiMode) {
                    // ASCII mode
                    QChar asciiChar = QChar::fromLatin1(regValue);
                    output = QString(asciiChar);
                } else {
                    // Hex mode
                    output = QString::number(regValue, 16).toUpper().rightJustified(2, '0') + "\n";
                }

                screenLabel->setText(screenLabel->text() + output);

            }

            // Set instruction format label
            QString format;
            if (opcode >= '1' && opcode <= '9') {
                format = QString("%1RXY").arg(opcode);
            } else if (opcode == 'A' || opcode == 'a') {
                format = "ARXY";
            } else if (opcode == 'B' || opcode == 'b') {
                format = "BRXY";
            } else if (opcode == 'C' || opcode == 'c') {
                format = "C000";
            } else if (opcode == 'D' || opcode == 'd') {
                format = "DRXY";
            }
            controlPanel->instrFormat->setText(format);

            // Update description
            QString description = getInstructionDescription(opcode, r, x, y);
            controlPanel->description->setText(description);


            // Execute the instruction
            if (currentInstruction[0] == 'C' || currentInstruction[0] == 'c') {
                reachedHalt = true;
                QMessageBox::information(this, tr("Success"),
                                         tr("Program halted: C000 instruction encountered."));
                statusBar()->showMessage("Program halted: C000 instruction encountered", 2000);
            }

            // Execute instruction and update PC
            int tempPC = cpuState.PC_2; // Convert byte address to instruction
            bool isJump = ui.executeCommand(tempPC, currentInstruction);
            cpuState.PC_2 = tempPC;


            // Update registers and Memory display after each instruction
            registerFile = ui.returnREG();
            memoryFile = ui.returnMEM();

            const vector<uint8_t>& regi = registerFile.returnRegisters();
            const vector<uint8_t>& memoryData = memoryFile.returnMemory();

            int registerToHighlight = getRegisterFromInstruction(currentInstruction);

            updateAndHighlightMemory(memoryData, cpuState.PC_2, prevMemoryCell);
            updateAndHighlightRegisters(regi, registerToHighlight, prevRegisterCell);
            // Move to next instruction (2 bytes)
            if(!isJump)
            {
                cpuState.PC_2 += 2;
            }

            // Small delay to show execution progress
            QApplication::processEvents();
            QThread::msleep(100); // 1000ms (1sec) delay between instructions

        }

        if (!(cpuState.PC_2 < lastUsedAddress)) {
            QMessageBox::warning(this, tr("Success"),
                                 tr("Program completed: End of memory reached"));
            statusBar()->showMessage("Program completed: End of memory reached", 2000);
        }

        memoryFile = ui.returnMEM();
        registerFile = ui.returnREG();
    }

    void executeUntilHalt() {
        isTerminated = false;

        clearHighlighting(prevMemoryCell, prevRegisterCell);

        // Initialize control unit with current memory and register state

        controlUnit.fetch(memoryFile, registerFile);
        bool reachedHalt = false;
        // memoryFile = controlUnit.returnMemory();
        ui.fetch(controlUnit);

        while (!reachedHalt && !isTerminated  && cpuState.PC_2 < lastUsedAddress) {


            // Get the current instruction (2 bytes)
            string firstByte = memoryFile.intToHexStr(memoryFile.getMemoryCell(cpuState.PC_2));
            string secondByte = memoryFile.intToHexStr(memoryFile.getMemoryCell(cpuState.PC_2 + 1));

            // Combine into 4-digit instruction
            string currentInstruction = firstByte + secondByte;

            // Parse the instruction
            char opcode = currentInstruction[0];
            char r = currentInstruction[1];
            char x = currentInstruction[2];
            char y = currentInstruction[3];
            controlPanel->opcodeValue->setText(QString(opcode));
            controlPanel->rValue->setText(QString(r));
            controlPanel->xValue->setText(QString(x));
            controlPanel->yValue->setText(QString(y));

            if (currentInstruction[0] == '3' && currentInstruction.size() == 3 && currentInstruction[2] == '0') {
                // Get the register number
                int regNum = QString(currentInstruction[1]).toInt(nullptr, 16);
                // Get the register value
                const vector<uint8_t>& registers = registerFile.returnRegisters();
                uint8_t regValue = registers[regNum];

                // Convert to string and update screen

                QString output;
                if (isAsciiMode) {
                    // ASCII mode
                    QChar asciiChar = QChar::fromLatin1(regValue);
                    output = QString(asciiChar);
                } else {
                    // Hex mode
                    output = QString::number(regValue, 16).toUpper().rightJustified(2, '0') + "\n";
                }

                screenLabel->setText(screenLabel->text() + output);

            }

            // Set instruction format label
            QString format;
            if (opcode >= '1' && opcode <= '9') {
                format = QString("%1RXY").arg(opcode);
            } else if (opcode == 'A' || opcode == 'a') {
                format = "ARXY";
            } else if (opcode == 'B' || opcode == 'b') {
                format = "BRXY";
            } else if (opcode == 'C' || opcode == 'c') {
                format = "C000";
            } else if (opcode == 'D' || opcode == 'd') {
                format = "DRXY";
            }
            controlPanel->instrFormat->setText(format);

            // Update description
            QString description = getInstructionDescription(opcode, r, x, y);
            controlPanel->description->setText(description);


            // Execute the instruction
            if (currentInstruction[0] == 'C' || currentInstruction[0] == 'c') {
                reachedHalt = true;
                QMessageBox::information(this, tr("Success"),
                                         tr("Program halted: C000 instruction encountered."));
                statusBar()->showMessage("Program halted: C000 instruction encountered", 2000);
                break;
            }

            // Execute instruction and update PC
            int tempPC = cpuState.PC_2; // Convert byte address to instruction
            bool isJump = ui.executeCommand(tempPC, currentInstruction);
            cpuState.PC_2 = tempPC;

            // Update registers and Memory display after each instruction
            registerFile = ui.returnREG();
            memoryFile = ui.returnMEM();

            const vector<uint8_t>& regi = registerFile.returnRegisters();
            const vector<uint8_t>& memoryData = memoryFile.returnMemory();

            int registerToHighlight = getRegisterFromInstruction(currentInstruction);

            updateAndHighlightMemory(memoryData, cpuState.PC_2, prevMemoryCell);
            updateAndHighlightRegisters(regi, registerToHighlight, prevRegisterCell);
            // Move to next instruction (2 bytes)
            if(!isJump)
            {
                cpuState.PC_2 += 2;
            }

            if (isTerminated) {
                break; // Stop execution if termination flag is set
            }
            int delayMs = controlPanel->delaySlider->value();
            qInfo() << delayMs << "\n";
            // Small delay to show execution progress
            QApplication::processEvents();
            QThread::msleep(delayMs); // 1000ms (1sec) delay between instructions
        }


        clearHighlighting(prevMemoryCell, prevRegisterCell);
        if(isTerminated)
        {
            QMessageBox::warning(this, tr("Success"),
                                 tr("Program execution stopped"));
            statusBar()->showMessage("Program execution stopped", 2000);
        }
        else if (!reachedHalt) {
            QMessageBox::warning(this, tr("Success"),
                                 tr("Program completed: End of memory reached"));
            statusBar()->showMessage("Program completed: End of memory reached", 2000);
        }

        memoryFile = ui.returnMEM();
        registerFile = ui.returnREG();

    }

};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MachineLangSimulator window;
    window.show();
    return app.exec();
}
