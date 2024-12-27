# VOLE Machine Simulator

## Overview
This VOLE (Virtual Operation Language Environment) Machine Simulator is an educational tool that simulates a 16-bit instruction set architecture. The simulator executes VOLE machine language instructions, providing a hands-on learning environment for understanding computer architecture and assembly programming concepts.

## Features

### 1. Instruction Set Architecture
- **16-bit Instructions**: Each instruction consists of:
  - 4-bit opcode (first 4 bits)
  - 12-bit operation field (last 12 bits)
- **Hexadecimal Representation**: Instructions are represented in 4-digit hexadecimal format

#### Supported Operations
| Opcode (Hex) | Operation | Description |
|--------------|-----------|-------------|
| 0 | HALT | Stop execution |
| 1 | LOAD | Load from memory |
| 2 | STORE | Store to memory |
| 3 | ADD | Addition |
| 4 | SUB | Subtraction |
| 5 | MUL | Multiplication |
| 6 | DIV | Division |
| 7 | AND | Logical AND |
| 8 | OR | Logical OR |
| 9 | XOR | Logical XOR |
| A | NOT | Logical NOT |
| B | JMP | Unconditional Jump |
| C | JZ | Jump if Zero |
| D | JNZ | Jump if Not Zero |
| E | IN | Input |
| F | OUT | Output |

### 2. Interactive Menu System
The simulator provides an interactive menu with the following options:
- Load Program: Load VOLE machine code from a file
- Step Execution: Execute program step by step
- Run Program: Execute entire program
- Display Status: Show current system state
- Reset: Reset simulator to initial state
- Exit: Terminate simulator

### 3. Program Loading
- Support for loading programs from text files
- File format: One instruction per line in hexadecimal format
- Automatic validation of instruction format
- Error handling for invalid instructions
- Example file format:
  ```
  0x10x20x30x4
  0x30x40x50x6
  0x20x70x80x9
  0x00x00x00x0
  ```

### 4. Execution Features
- **Step-by-Step Execution**:
  - Fetch instruction to IR (Instruction Register)
  - Decode opcode and operation fields
  - Validate instruction
  - Execute instruction
  - Update system state
- **Instruction Validation**:
  - Verify opcode validity
  - Check operation field format
  - Validate register numbers
  - Verify memory addresses

### 5. System State Display
Comprehensive status display showing:
- **Registers**: Contents of all registers (R0-RF)
- **Special Registers**:
  - PC (Program Counter)
  - IR (Instruction Register)
- **Memory**: Current state of memory
- **Screen Output**: Display buffer contents
- **Flags**: Zero flag, overflow flag, etc.

## Usage

### Example Program
```
# Simple addition program (add.txt)
0x10x20x30x4    # Load value from memory address 4 to register 3
0x30x40x50x6    # Add registers 4 and 5, store in register 6
0x20x70x80x9    # Store register 7 to memory address 89
0x00x00x00x0    # Halt
```



