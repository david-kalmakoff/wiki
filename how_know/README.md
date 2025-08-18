# But How Do It Know? - The Basic Principles of Computers for Everyone

## Logic Gates

Logic gates are electrical components that make up a computer, they exist in the silicon chips and other components.

The simplest of the gates is the NAND gate. It has 2 inputs and one output. The output is always on, unless both inputs are on, then it is off.

![NAND Gate](https://pfnicholls.com/Electronics_Resources/Images/Logic_NAND_TT_Symbol.png)

The NOT gate is a combination of NAND gates to create a gate that gives the opposite output of it's input.

![NOT Gate](https://pfnicholls.com/Electronics_Resources/Images/Logic_NAND_Equivalent_NOT.png)

The AND gate is a combination of a NAND gate and a NOT gate. I is always off, unless both inputs are on, then it is on.

![AND Gate](https://pfnicholls.com/Electronics_Resources/Images/Logic_NAND_Equivalent_AND.png)

## Bit

Bits are built from NAND gates. There are 2 inputs and one output. The first input is the state we want to capture. The second input is the selector.

When the selector is switched on, the state of the first input is captured. When the second input is switched off the state of the first input is stored. The output will stay the stored value until the second input is switched on and the first input is in a different state.

![Bit](https://i.sstatic.net/7dbEM.png)

## Bits, Nibbles, Bytes

Bits make up the memory of the system.

A byte is a collection of 8 ordered bits, a nibble is 4.

Bytes can be used to represent data such as [ASCII](https://en.wikipedia.org/wiki/ASCII) characters.

## Register

A register is a combination of a byte and an enabler.

An enabler is a byte in-front of another byte that decides when the output will be made available.

## Bus

A bus is a bundle of 8 wires connecting register inputs and outputs.

## Decoder

The number of outputs of a decoder are equal to the number of possible different input options that are available (2x4, 3x8, 4x16, 5x32, 6x64).
