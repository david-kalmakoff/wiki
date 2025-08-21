# RISC-V Assembly Programming

[Website](https://riscv-programming.org/index.html)

## Motivation

After reading the 'But How Do It Know?' book I was motivated to crack this one open and dive deeper into assembly language.

I see RISC-V as having an exciting future, being an open alternative to the current architectures.

**A lot of my notes here are relative to the But How Do It Know Book**

## Chapter 1: Introduction

Processor includes 31 32-bit registers.

CPU datapath = ALU (Algebraic and Logic Unit)

Instruction Set Architecture (ISA)

32-bit instructions, instructions are stored in memory as 32-bits

Program Counter (PC) = IAR (Instruction Address Register)

IR is the same Instruction Register

### RV32I execution cycle

```
while True do
  IR <- MainMemory[PC]    # load instruction from address in PC
  PC <- PC + 4            # increment PC by 4 to move to next instruction
  ExecuteInstruction(IR)  # execute instruction loaded in IR
end
```

## Chapter 2: Data Representation

This is a refresher on number systems and ASCII.

This is also built on some more by showing how things are stored in memory.

Arrays are stored in row-major order or column-major order.

Structs are stored in order of their fields and are grouped together in memory.

### little-endian

The least significant byte is stored first in memory.

00000000 00000000 00000100 00000001

is stored:

```
000 -> 00000001
001 -> 00000100
002 -> 00000000
003 -> 00000000
```

### big-endian

The least significant byte is last first in memory.

00000000 00000000 00000100 00000001

is stored:

```
000 -> 00000000
001 -> 00000000
002 -> 00000100
003 -> 00000001
```

### Instructions

Operation code (opcode)

RISC-V Instruction:

- funct7: identify instruction
- rs2: instruction parameter
- rs1: instruction parameter
- funct3: identify instruction
- rd: instruction parameter
- opcode: identify instruction

## Chapter 3: Assembly Files
