# Chapter 1

## First half of the computer

### Logic gates

Logic gates are electrical components that make up a computer, they exist in the silicon chips and other components.

#### NAND Gate

The simplest of the gates is the NAND gate. It has 2 inputs and one output. The output is always on, unless both inputs are on, then it is off.

![NAND Gate](https://pfnicholls.com/Electronics_Resources/Images/Logic_NAND_TT_Symbol.png)

#### NOT Gate

The NOT gate is a combination of NAND gates to create a gate that gives the opposite output of it's input.

![NOT Gate](./imgs/not.png)

#### AND GATE

The AND gate is a combination of a NAND gate and a NOT gate. I is always off, unless both inputs are on, then it is on.

![AND Gate](./imgs/and.png)

#### 4 input AND Gate

AND gates can be combined together to require more inputs

![4AND Gate](./imgs/4and.png)

### Bit

Bits are built from NAND gates. There are 2 inputs and one output. The first input is the state we want to capture. The second input is the selector.

When the selector is switched on, the state of the first input is captured. When the second input is switched off the state of the first input is stored. The output will stay the stored value until the second input is switched on and the first input is in a different state.

![Bit of Memory](./imgs/memory.png)

### Bits, Nibbles, Bytes

Bits make up the memory of the system.

A byte is a collection of 8 ordered bits, a nibble is 4.

Bytes can be used to represent data such as [ASCII](https://en.wikipedia.org/wiki/ASCII) characters.

![Byte of Memory](./imgs/byte.png)

### Register

#### Enabler

An enabler is a byte in-front of another byte that decides when the output will be made available.

![Enabler](./imgs/enabler.png)

#### Register

A register is a combination of a byte and an enabler.

![Register](./imgs/register.png)

### Bus

A bus is a bundle of 8 wires connecting register inputs and outputs.

### Decoder

The number of outputs of a decoder are equal to the number of possible different input options that are available (2x4, 3x8, 4x16, 5x32, 6x64).

This 4x16 decoder is what is used for our ram to indicate a column or row to activate.

![4x16 Decoder](./imgs/4x16_decoder.png)

### Ram

#### Ram Register

This is what exists at every point in the ram grid.

![Ram Register](./imgs/ram_register.png)

#### 16 Ram Registers in Column

This is a column of 16 ram storage locations.

![Ram Register 16](./imgs/ram_register_16.png)

#### 16 Ram Registers Columns

This is a row of 16 storage locations columns built above.

![Ram Register 256](./imgs/ram_register_256.png)

#### 256 Bytes Ram

This ram has an 8 bit register (Memory Address Register / MAR) to indicate which of the 256 locations to read/write from/to.

The MAR is broken down with 2 decoders for the X and Y axis' of the storage location grid.

There is an enable input to allow for reading from a location and a set button to allow for writing to a location.

![256 Bytes Ram](./imgs/256bytes_ram.png)

This is an example of the ram being used. Here we are reading the memory stored at the 0x04 address.

![Ram Example](./imgs/ram_use.png)

## Other half of the computer

### OR Gate

OR gates are on if either or both inputs are on.

![OR Gate](./imgs/or.png)

XOR gate are only on if one of the inputs are on, it is off if both are on.

### XOR Gate

![XOR Gate](./imgs/xor.png)

### Shifters

Shifter shift the bytes in a register over in one direction.

#### Left Shifter

The left shifter shifts bytes left ie. `0000 1100` (12) becomes `0001 1000` (24). Multiplying the value by 2.

![Left Shifter](./imgs/left_shifter.png)

#### Right Shifter

The right shifter shifts bytes right ie. `0000 1100` (12) becomes `0000 0110` (6). Dividing the value by 2.

![Right Shifter](./imgs/right_shifter.png)

### NOTer (Inverter)

The inverter flip the bits to the opposite.

![Inverter](./imgs/inverter.png)

### ANDer

The ANDer takes 2 input bytes and ANDs each bit.

![ANDer](./imgs/ander.png)

### ORer

The ORer takes 2 input bytes and ORs each bit.

![ORer](./imgs/orer.png)

### XORer

The XORer takes 2 input bytes and XORs each bit.

![XORer](./imgs/xorer.png)

### ADDERS

#### ADDER 2 bit

Takes in 2 bits and outputs a SUM and a CARRY.

![ADDER 2 bit](./imgs/adder_2bit.png)

#### ADDER 3 bit

Takes in 3 bits and outputs a SUM and a CARRY.

![ADDER 3 bit](./imgs/adder_3bit.png)

#### ADDER 8 bit

Takes in 2 bytes and a CARRY IN then outputs a SUM byte and a CARRY OUT.

![ADDER 8 bit](./imgs/adder_8bit.png)

### Comparator

Comparators take 2 inputs and output whether or not they are equal and if a is larger than b. It also outputs an XOR result.

#### Comparator 1 bit

![Comparator 1 bit](./imgs/comparator_1_bit.png)

#### Comparator 8 bit

![Comparator 8 bit](./imgs/comparator_8_bit.png)

### Zero

The Zero takes a byte and outputs a bit that is only on when all bits in the input are zero.

![Zero](./imgs/zero.png)

### Arithmetic and Logic Unit

The ALU combines the above 7 logic operations and makes them available in one place. You use a `op` inputs to select which operation you would like to perform along with 2 input bytes and a `carry in` if needed. It will output the result of the operation `c`, if it is `zero`, the `carry out`, and the `larger` and `equal` results from comparison.

![ALU](./imgs/alu.png)

### Bus 1

![Bus 1](./imgs/bus1.png)

### Clock

#### Clock

A clock is made by connecting the output of a NOT gate to it's input. This will cause it to flip on / off forever.

![Clock](./imgs/clock.png)

#### Clock with delay

We want to create a delay in the clock to be able to space out our operations. We are delaying the clock by half a step.

![Clock Delay](./imgs/clock_delay.png)

#### Clock main

`clk s` the set clock is shorter and is used to set the register we are operating on. This is on when both our clock and delayed clock is on.

`clk e` the enable clock is longer and is used to enable the register we are operating on. This is on when either our clock or delayed clock is on.

`clk` is the main, normal timed clock.

![Clock Main](./imgs/clock_main.png)

### Stepper

![Stepper](./imgs/stepper.png)
