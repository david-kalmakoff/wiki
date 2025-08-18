# But How Do It Know? - The Basic Principles of Computers for Everyone

## Simulator

I found this simulator to allow me reproduce the circuits shown in the book.

[Digital Logic Sim](https://github.com/SebLague/Digital-Logic-Sim)

## Logic Gates

Logic gates are electrical components that make up a computer, they exist in the silicon chips and other components.

The simplest of the gates is the NAND gate. It has 2 inputs and one output. The output is always on, unless both inputs are on, then it is off.

![NAND Gate](https://pfnicholls.com/Electronics_Resources/Images/Logic_NAND_TT_Symbol.png)

The NOT gate is a combination of NAND gates to create a gate that gives the opposite output of it's input.

![NOT Gate](./imgs/not.png)

The AND gate is a combination of a NAND gate and a NOT gate. I is always off, unless both inputs are on, then it is on.

![AND Gate](./imgs/and.png)

AND gates can be combined together to require more inputs

![4AND Gate](./imgs/4and.png)

## Bit

Bits are built from NAND gates. There are 2 inputs and one output. The first input is the state we want to capture. The second input is the selector.

When the selector is switched on, the state of the first input is captured. When the second input is switched off the state of the first input is stored. The output will stay the stored value until the second input is switched on and the first input is in a different state.

![Bit of Memory](./imgs/memory.png)

## Bits, Nibbles, Bytes

Bits make up the memory of the system.

A byte is a collection of 8 ordered bits, a nibble is 4.

Bytes can be used to represent data such as [ASCII](https://en.wikipedia.org/wiki/ASCII) characters.

![Byte of Memory](./imgs/byte.png)

## Register

### Enabler

An enabler is a byte in-front of another byte that decides when the output will be made available.

![Enabler](./imgs/enabler.png)

### Register

A register is a combination of a byte and an enabler.

![Register](./imgs/register.png)

## Bus

A bus is a bundle of 8 wires connecting register inputs and outputs.

## Decoder

The number of outputs of a decoder are equal to the number of possible different input options that are available (2x4, 3x8, 4x16, 5x32, 6x64).

This 4x16 decoder is what is used for our ram to indicate a column or row to activate.

![4x16 Decoder](./imgs/4x16_decoder.png)

## Ram

### Ram Register

This is what exists at every point in the ram grid.

![Ram Register](./imgs/ram_register.png)

### 16 Ram Registers in Column

This is a column of 16 ram registers

![Ram Register 16](./imgs/ram_register_16.png)

### 16 Ram Registers Columns

This is a row of 16 ram columns built above.

![Ram Register 256](./imgs/ram_register_256.png)

### 256 Bytes Ram

This ram has an 8 bit register to indicate which of the 256 registers to read/write from/to.

That register is broken down with 2 decoders for the X and Y axis' of the register grid.

There is a enable input to allow for reading from a register and a set button to allow for writing to a register.

![256 Bytes Ram](./imgs/256bytes_ram.png)

This is an example of the ram being used. Here we are reading the register from the 0x04 address.

![Ram Example](./imgs/ram_use.png)
